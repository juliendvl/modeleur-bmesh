#include <iostream>
#include "edgefairing.h"


using namespace std;
using OpenMesh::Vec3f;


///////////////////////////////////////////////////////////////////////////////
bool EdgeFairing::fairing() {

    vector<Vec3f> pts;

    // We iterate over all vertices
    BMesh::VertexIter vit;
    for (vit = m.vertices_begin(); vit != m.vertices_end(); ++vit) {
        Vec3f newPt = m.point(*vit);

        // We get the vertex's neighbors
        vector<Vec3f> neighbors;
        BMesh::VertexVertexIter vv;
        for (vv = m.vv_iter(*vit); vv.is_valid(); ++vv)
            neighbors.push_back(m.point(*vv));

        // if vertex is valence 4
        if (neighbors.size() == 4) {
            vector<Vec3f> proj = projection(*vit, neighbors);

            Vec3f a = proj[0];
            Vec3f b = proj[1];
            Vec3f c = proj[2];
            Vec3f d = proj[3];


            Vec3f n = m.normal(*vit);
            Vec3f normal = ((c - a) % n);
            normal = normal.normalize();

            float t = ( (a - b) | normal ) / ( (d - b) | normal );
            newPt = b + (d - b) * max(0, min(1,t));
        }
        else {
            /**
              * Here, we use the umbrella operator:
              * cf http://mesh.brown.edu/DGP/pdfs/Kobbelt-etal-sg98.pdf
              * at Discrete Fairing part
              */
            newPt = Vec3f();
            for (unsigned int i = 0; i < neighbors.size(); i++)
                newPt += neighbors[i];
            newPt /= neighbors.size();

            // Then we project the vertex onto the original tangent plane
            Vec3f pos  = m.point(*vit);
            Vec3f n    = m.normal(*vit);
            float norm = n.norm();
            float f = ( (newPt - pos) | n ) / (norm * norm);
            newPt = newPt - f * n;
        }

        pts.push_back(newPt);
    }

    // We update points
    for (unsigned int i = 0; i < pts.size(); i++)
        m.set_point(m.vertex_handle(i), pts[i]);

    // Normal update
    m.update_normals();

    return true;
}


///////////////////////////////////////////////////////////////////////////////
vector<Vec3f> EdgeFairing::projection(const BMesh::VertexHandle &o,
                                      const vector<Vec3f> &v)
{
    vector<Vec3f> res;

    Vec3f pos  = m.point(o);
    Vec3f n    = m.normal(o);
    float norm = n.norm();

    for (unsigned int i = 0; i < v.size(); i++) {
        // | is the OpenMesh scalar product
        float f = ( (v[i] - pos) | n ) / (norm * norm);
        res.push_back(v[i] - f * n);
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

