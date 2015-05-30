#ifndef DEF_MESH
#define DEF_MESH

#include <string>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/CatmullClarkT.hh>

#include "renderable.h"


typedef OpenMesh::PolyMesh_ArrayKernelT<> BMesh;

/**
 * @brief This class is used to store an OpenMesh mesh.
 *        We also provide functions which allow to subdivide a mesh (with the
 *        Catmull-Clark algorithm), to draw a mesh and to save a mesh in a
 *        .obj file.
 */
class Mesh : public Renderable
{

public:
    /**
     * @brief Constructor
     */
    Mesh();

    /**
     * @brief Copy constructor
     * @param m Mesh to copy
     */
    Mesh(const Mesh &m);

    ~Mesh();

    /**
     * @brief Returns OpenMesh mesh
     * @return OpenMesh mesh
     */
    BMesh& getMesh();

    void draw();

    /**
     * @brief Performs Catmull-Clark algorithm to subdivide the mesh
     * @return true si the algorithm succeed; false otherwise
     */
    bool subdivide();

    /**
     * @brief Saves mesh in a .obj file
     * @param fileName Where to save the mesh
     * @return
     */
    bool saveMesh(const std::string &fileName);

private:
    BMesh *mesh;

    int getNormalIndex(const std::vector<OpenMesh::Vec3f> &v,
                       const OpenMesh::Vec3f &n);

};

#endif
