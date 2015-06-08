#ifndef DEF_MESH
#define DEF_MESH

#include <string>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Subdivider/Uniform/CatmullClarkT.hh>

#include "renderable.h"


typedef OpenMesh::PolyMesh_ArrayKernelT<> BMesh;

/**
 * @brief Mesh methods
 */
class Mesh
{

public:
    /**
     * @brief Default constructor
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
    BMesh* getPMesh();

    /**
     * @brief Draws the mesh
     */
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

    // Gets the normal handle
    int getNormalIndex(const std::vector<OpenMesh::Vec3f> &v,
                       const OpenMesh::Vec3f &n);

};

#endif
