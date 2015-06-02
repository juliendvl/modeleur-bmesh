#include <iostream>
#include <nlopt.hpp>
#include "edgefairing.h"
#include "include/curvaturetensor.h"


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
    vector<Vec3f> points;
    BMesh::VertexIter vit;
    BMesh::VertexVertexIter vv;

    // We iterate over all vertices
    for (vit = m->vertices_begin(); vit != m->vertices_end(); ++vit) {
        Vec3f p     = m->point(*vit);
        Vector3f ep = OMEigen::toEigen(p);
        Vec3f newPt = Vec3f(0.0, 0.0, 0.0);

        // We get vertex neighbors
        vector<BMesh::VertexHandle> neighbors;
        for (vv = m->vv_iter(*vit); vv.is_valid(); ++vv)
            neighbors.push_back(*vv);

        if (neighbors.size() < 3) {
            points.push_back(p);
            continue;
        }

        CurvatureTensor ct(*m);
        ct.compute(*vit, neighbors);
        vector<float> curv = ct.getCurvatures();

        // If vertex is valence 4
        if (neighbors.size() == 4 && (curv[0] != curv[1])) {
            // We get principal directions
            vector<Vector2f> directions = ct.getEigenDirections();

            // We project neighbors onto the tangent plane
            vector<Vector3f> proj = projection(*vit, neighbors);

            Matrix3f InvP = ct.passInvMatrix();
            for (unsigned int i = 0; i < proj.size(); ++i) {
                proj[i] -= ep;
                proj[i] = InvP * proj[i];
            }

            // Init data structure
            Vector2f a(proj[0][0], proj[0][1]);
            Vector2f b(proj[1][0], proj[1][1]);
            Vector2f c(proj[2][0], proj[2][1]);
            Vector2f d(proj[3][0], proj[3][1]);
            Vector2f ev = directions[0];
            Vector2f eu = directions[1];
            Data data = Data(a, b, c, d, eu, ev);

            // Init non linear optimizer
            nlopt::opt o(nlopt::LN_COBYLA, 2);
            o.set_min_objective(objFunc, &data);
            o.set_xtol_rel(1e-6);

            // Optimize
            vector<double> x(2);
            double minf;
            o.optimize(x, minf);

            //cout << x[0] << " " << x[1] << endl;

            Vector3f tmp(x[0], x[1], 0.0);
            Matrix3f P = ct.passMatrix();
            tmp = P * tmp;

            //newPt = p;
            newPt = OMEigen::toOpenMesh(tmp) + p;
        }
        else {
            /**
             * Umbrella operator
             * */
            for (unsigned int i = 0; i < neighbors.size(); ++i)
                newPt += m->point(neighbors[i]);
            newPt /= neighbors.size();

            Vec3f n = m->normal(*vit);
            float norm = n.norm();

            float f = ( (newPt - p) | n ) / (norm * norm);
            newPt = newPt - f * n;
        }

        points.push_back(newPt);
    }

    // We update vertices positions
    int i = 0;
    for (vit = m->vertices_begin(); vit != m->vertices_end(); ++vit) {
        m->set_point(*vit, points[i]);
        i++;
    }

    return true;
}


///////////////////////////////////////////////////////////////////////////////
vector<Vector3f> EdgeFairing::projection(const BMesh::VertexHandle &o,
                                      const vector<BMesh::VertexHandle> &v)
{
    vector<Vector3f> res;

    Vec3f pos  = m->point(o);
    Vec3f n    = m->normal(o);
    float norm = n.norm();

    for (unsigned int i = 0; i < v.size(); i++) {
        Vec3f p = m->point(v[i]);

        // | is the OpenMesh scalar product
        float f = ( (p - pos) | n ) / (norm * norm);
        res.push_back(OMEigen::toEigen(p - f * n));
    }

    return res;
}
