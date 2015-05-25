#include <iostream>
#include "edgefairing.h"


using namespace std;
using OpenMesh::Vec3f;


///////////////////////////////////////////////////////////////////////////////
EdgeFairing::EdgeFairing(Mesh &mesh) {
    this->m = mesh.getMesh();
}


///////////////////////////////////////////////////////////////////////////////
bool EdgeFairing::fairing(Mesh &mesh) {

    // We iterate over all vertices
    BMesh::VertexIter vit;
    for (vit = m.vertices_begin(); vit != m.vertices_end(); ++vit) {

        // We get the vertex's neighbors
        vector<Vec3f> neighbors;
        BMesh::VertexVertexIter vv = m.vv_iter(*vit);
        for (; vv.is_valid(); ++vv)
            neighbors.push_back(m.point(*vv));

        // if vertex is valence 4
        if (neighbors.size() == 4) {
            vector<Vec3f> proj = projection(*vit, neighbors);
        }
        else {

        }
    }


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
        float f = ((v[i] - pos) | n) / (norm * norm);
        res.push_back(v[i] - f * n);
    }

    return res;
}
