#ifndef DEF_MESHEVOLVE
#define DEF_MESHEVOLVE

#include "mesh.h"
#include "skeleton.h"
#include "sphere.h"

typedef OpenMesh::PolyMesh_ArrayKernelT<> BMesh;


/**
 * @brief Mesh Evolution (step 4 of the B-Mesh algorithm)
 */
class MeshEvolve
{

public:
    /**
     * @brief Constructor
     * @param mesh Mesh to work with
     * @param s    Skeleton to work with
     */
    MeshEvolve(Mesh &mesh, Skeleton *s);

    /**
     * @brief  Processes mesh evolution
     * @return true if mesh evolution suceed; false otherwise
     */
    bool evolve();

private:
    BMesh m;
    Skeleton *s;

    OpenMesh::Vec3f scalarNormal(const OpenMesh::Vec3f &p);
    float fi(const Sphere *s, const OpenMesh::Vec3f &p);
    float scalarField(const OpenMesh::Vec3f &p);

    float absf(float a);
};

#endif
