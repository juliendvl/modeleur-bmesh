#ifndef DEF_EDGEFAIRING
#define DEF_EDGEFAIRING

#include <vector>
#include "mesh.h"

typedef OpenMesh::PolyMesh_ArrayKernelT<> BMesh;


class EdgeFairing
{

public:
    bool fairing(Mesh &mesh);

    EdgeFairing(Mesh &mesh);

private:
    BMesh m;

    std::vector<OpenMesh::Vec3f> projection(const BMesh::VertexHandle &o,
                  const std::vector<OpenMesh::Vec3f> &v);


};


#endif
