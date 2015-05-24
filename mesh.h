#ifndef DEF_MESH
#define DEF_MESH

#include <string>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/CatmullClarkT.hh>

#include "renderable.h"


typedef OpenMesh::PolyMesh_ArrayKernelT<> BMesh;
class Mesh : public Renderable
{

public:
    Mesh();
    ~Mesh();

    BMesh& getMesh();

    void draw();

    bool subdivide();
    bool saveMesh(const std::string &fileName);

private:
    BMesh *mesh;

    int getNormalIndex(const std::vector<OpenMesh::Vec3f> &v,
                       const OpenMesh::Vec3f &n);

};

#endif
