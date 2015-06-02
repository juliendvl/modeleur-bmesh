#ifndef DEF_EDGEFAIRING
#define DEF_EDGEFAIRING

#include <vector>
#include "mesh.h"
#include "omeigen.h"

typedef OpenMesh::PolyMesh_ArrayKernelT<> BMesh;

/**
 * @brief Edge Fairing (step 5 of the B-Mesh algorithm)
 */
class EdgeFairing
{

public:
    /**
     * @brief Constructor
     * @param mesh Mesh to work with
     */
    EdgeFairing(Mesh &mesh);

    /**
     * @brief Processes edge fairing
     * @return true if edge fairing succeed; false otherwise
     */
    bool fairing();

private:
    BMesh *m;

    std::vector<Eigen::Vector3f> projection(const BMesh::VertexHandle &o,
                  const std::vector<BMesh::VertexHandle> &v);
};


#endif
