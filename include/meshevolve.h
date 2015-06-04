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
     * @param level Subdivision level
     */
    MeshEvolve(Mesh &mesh, Skeleton *s, int level = 1);

    /**
     * @brief  Processes mesh evolution
     * @return true if mesh evolution suceed; false otherwise
     */
    bool evolve();

private:
    BMesh *m;
    Skeleton *s;
    int level;
    float step;

    std::vector<Sphere*> balls;

    // Computes n = -grad(I)/||grad(I)||, where I is a scalar field
    OpenMesh::Vec3f scalarNormal(const OpenMesh::Vec3f &p);

    // Computes fi (as defined in the article
    float fi(const Sphere *s, const OpenMesh::Vec3f &p);

    // Computes I (as defined in the article)
    float scalarField(const OpenMesh::Vec3f &p);

    // Computes step (as defined in the article)
    void computeStep();

};

#endif
