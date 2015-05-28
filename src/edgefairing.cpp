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
bool EdgeFairing::fairing() {
    vector<Vec3f> pts;
    BMesh::VertexIter vit;
    BMesh::VertexVertexIter vv;

    // We iterate over all vertices
    for (vit = m.vertices_begin(); vit != m.vertices_end(); ++vit) {
        Vec3f p = m.point(*vit);
        Vec3f newPt;

        // We get the vertex's neighbors
        vector<BMesh::VertexHandle> neighbors;
        for (vv = m.vv_iter(*vit); vv.is_valid(); ++vv)
            neighbors.push_back(*vv);

        // If vertex is valence 4
        if (neighbors.size() == 4) {
            // We get the projections onto the tangent plane
            vector<Vec3f> proj = projection(*vit, neighbors);

            // We get the principal directions of vertex
            CurvatureTensor ct(m);
            ct.compute(*vit, neighbors);
            vector<Vec2f> directions = ct.getOMDirections();

            // Init non linear optimizer
            Vector2f a(proj[0][0], proj[0][1]);
            Vector2f b(proj[1][0], proj[1][1]);
            Vector2f c(proj[2][0], proj[2][1]);
            Vector2f d(proj[3][0], proj[3][1]);
            Vector2f eu = OMEigen::toEigen(directions[0]);
            Vector2f ev = OMEigen::toEigen(directions[1]);
            Data data = Data(a, b, c, d, eu, ev);

            nlopt::opt o(nlopt::LN_COBYLA, 2);
            o.set_min_objective(objFunc, &data);
            o.set_xtol_rel(1e-6);

            vector<double> x(2);
            double minf;
            o.optimize(x, minf);

            // We fix the new z component to the average of projected
            // neighbors' z components
            float z = 0.0;
            for (unsigned int i = 0; i < neighbors.size(); ++i)
                z += proj[i][2];

            newPt = Vec3f(x[0], x[1], z / proj.size());
        }
        else {
            /**
              * Here, we use the umbrella operator
              * cf http://mesh.brown.edu/DGP/pdfs/Kobbelt-etal-sg98.pdf
              * at Discrete Fairing section
              **/
            for (unsigned int i = 0; i < neighbors.size(); ++i)
                newPt += m.point(neighbors[i]);
            newPt /= neighbors.size();

            // The new point must be projected onto the tangent plane
            Vec3f pos  = p;
            Vec3f n    = m.normal(*vit);
            float norm = n.norm();
            float f = ( (newPt - pos) | n ) / (norm * norm);
            newPt = newPt - f * n;
        }

        pts.push_back(newPt);
    }

    // We update vertices positions
    for (unsigned int i = 0; i < pts.size(); ++i)
        m.set_point(m.vertex_handle(i), pts[i]);

    m.update_normals();
    return true;
}


///////////////////////////////////////////////////////////////////////////////
vector<Vec3f> EdgeFairing::projection(const BMesh::VertexHandle &o,
                                      const vector<BMesh::VertexHandle> &v)
{
    vector<Vec3f> res;

    Vec3f pos  = m.point(o);
    Vec3f n    = m.normal(o);
    float norm = n.norm();

    for (unsigned int i = 0; i < v.size(); i++) {
        Vec3f p = m.point(v[i]);

        // | is the OpenMesh scalar product
        float f = ( (p - pos) | n ) / (norm * norm);
        res.push_back(p - f * n);
    }

    return res;
}


///////////////////////////////////////////////////////////////////////////////
float EdgeFairing::min(float a, float b) {
    return (a < b) ? a : b;
}

float EdgeFairing::max(float a, float b) {
    return (a < b) ? b : a;
}

