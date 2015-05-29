#ifndef DEF_MESHEVOLVE
#define DEF_MESHEVOLVE

#include "mesh.h"
#include "skeleton.h"
#include "sphere.h"

typedef OpenMesh::PolyMesh_ArrayKernelT<> BMesh;

class MeshEvolve
{

public:
    MeshEvolve(Mesh &mesh, Skeleton *s);

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
