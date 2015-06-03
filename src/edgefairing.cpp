#include <iostream>
#include <nlopt.hpp>
#include "edgefairing.h"
#include "curvaturetensor.h"
#include "mathutils.h"


using namespace std;
using namespace Eigen;
using OpenMesh::Vec3f;
using OpenMesh::Vec2f;


// Data structure necessary to use nlopt
struct Data
{
    Vector2f a;
    Vector2f b;
    Vector2f c;
    Vector2f d;
    Vector2f eu;
    Vector2f ev;

    Data(Vector2f a, Vector2f b, Vector2f c, Vector2f d, Vector2f eu,
         Vector2f ev) : a(a), b(b), c(c), d(d), eu(eu), ev(ev) {}
};


// Objective function used
double objFunc(const vector<double> &x, vector<double> &g, void *data) {
    (void)g;

    Vector2f vx(x[0], x[1]);
    Data *d = (Data*)data;

    float t1 = (d->a - vx).dot(d->ev);
    float t2 = (d->b - vx).dot(d->eu);
    float t3 = (d->c - vx).dot(d->ev);
    float t4 = (d->d - vx).dot(d->eu);

    return t1*t1 + t2*t2 + t3*t3 + t4*t4;
}



///////////////////////////////////////////////////////////////////////////////
EdgeFairing::EdgeFairing(Mesh &mesh) {
    this->m = mesh.getPMesh();
}


///////////////////////////////////////////////////////////////////////////////
bool EdgeFairing::fairing() {
    QMap<BMesh::VertexHandle, Vec3f> points;
    BMesh::VertexIter vit;
    BMesh::VertexVertexIter vv;

    // We iterate over all vertices
    for (vit = m->vertices_begin(); vit != m->vertices_end(); ++vit) {
        Vec3f p     = m->point(*vit);
        Vector3f ep = OMEigen::toEigen(p);
        Vector3f n  = OMEigen::toEigen(m->normal(*vit));

        // We get vertex neighbors
        vector<BMesh::VertexHandle> neighbors;
        for (vv = m->vv_iter(*vit); vv.is_valid(); ++vv)
            neighbors.push_back(*vv);

        if (neighbors.size() < 3) {
            points.insert(*vit, p);
            continue;
        }

        // We get curvature tensor
        CurvatureTensor ct(*m);
        ct.compute(*vit, neighbors);
        vector<float> curv = ct.getCurvatures();

        // If vertex is valence 4
        if ( neighbors.size() == 4 && (curv[0] != curv[1]) ) {
            vector<Vector3f> tp = ct.tangentPlane();

            // We get principal directions
            vector<Vector2f> dir = ct.getDirections();

            // We project neighbors onto the tangent plane
            vector<Vector3f> proj;
            for (unsigned int i = 0; i < neighbors.size(); ++i) {
                Vector3f tmp = OMEigen::toEigen( m->point(neighbors[i]) );
                proj.push_back( MathUtils::projectPoint(ep, tmp, n) );
            }

            // Frame transformation
            for (unsigned int i = 0; i < proj.size(); ++i) {
                Vector3f tmp = proj[i];
                for (unsigned int j = 0; j < 3; ++j)
                    proj[i][j] = (tmp-ep).dot(tp[j]);
            }

            // Init data structure
            Vector2f a(proj[0][0], proj[0][1]);
            Vector2f b(proj[1][0], proj[1][1]);
            Vector2f c(proj[2][0], proj[2][1]);
            Vector2f d(proj[3][0], proj[3][1]);
            Data data = Data(a, b, c, d, dir[0], dir[1]);

            // Init non linear optimizer
            nlopt::opt o(nlopt::LN_COBYLA, 2);
            o.set_min_objective(objFunc, &data);
            o.set_xtol_rel(1e-6);

            // Optimize
            vector<double> x(2);
            double minf;
            o.optimize(x, minf);

            // Frame transformation
            Vector3f res(x[0], x[1], 0.0);
            Matrix3f P;
            P << tp[0][0], tp[1][0], tp[2][0],
                 tp[0][1], tp[1][1], tp[2][1],
                 tp[0][2], tp[1][2], tp[2][2];

            res = P * res + ep;
            points.insert(*vit, OMEigen::toOpenMesh(res));

        }
        else {
            // Umbrella operator
            Vec3f newPt = Vec3f(0.0, 0.0, 0.0);
            for (unsigned int i = 0; i < neighbors.size(); ++i)
                newPt += m->point(neighbors[i]);
            newPt /= neighbors.size();
            Vector3f tmp = OMEigen::toEigen(newPt);

            // We project the point onto the tangent plane
            Vector3f proj = MathUtils::projectPoint(ep, tmp, n);
            points.insert(*vit, OMEigen::toOpenMesh(proj));
        }
    }

    // We update positions
    QMap<BMesh::VertexHandle, Vec3f>::iterator mit;
    for (mit = points.begin(); mit != points.end(); ++mit) {
        m->set_point(mit.key(), mit.value());
    }

    m->update_normals();

    return true;
}
