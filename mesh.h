#ifndef DEF_MESH
#define DEF_MESH

#include <iostream>
#include <string>
#include <vector>
#include <QGLViewer/qglviewer.h>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/CatmullClarkT.hh>

#include "renderable.h"


typedef OpenMesh::PolyMesh_ArrayKernelT<> BMesh;
class Mesh : public Renderable
{

public:
    Mesh();
    ~Mesh();

    void draw();
    bool subdivide();
    bool saveMesh(const std::string &fileName);

private:
    BMesh *mesh;

};

#endif
