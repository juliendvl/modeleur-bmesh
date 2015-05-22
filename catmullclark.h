#ifndef DEF_CATMULLCLARK
#define DEF_CATMULLCLARK

#include <vector>
#include <QGLViewer/qglviewer.h>
#include "mesh.h"

struct CCFace;
struct CCEdge;

struct CCVertex
{
    Vertex pos;
    std::vector<CCFace*> faces;
    std::vector<CCEdge*> edges;
};


struct CCFace
{
    std::vector<int> vertices;
    std::vector<CCEdge*> edges;
    Vertex facePoint;
};


struct CCEdge
{
    std::vector<int> vertices;
    std::vector<CCFace*> faces;
    Vertex edgePoint;
};


class CatmullClark
{

public:

    ~CatmullClark();

    /**
     * @brief Performs Catmull-Clark's algorithm
     * @param m Mesh to subdivide
     * @return true if the alorithm succeed; false otherwise
     */
    static bool subdivide(Mesh &m);

private:
    Mesh m;
    std::vector<CCVertex*> vertices;
    std::vector<CCFace*>   faces;
    std::vector<CCEdge*>   edges;

    /**
     * @brief CatmullClark constructor
     * @param m Mesh
     */
    CatmullClark(Mesh &m);

    /**
     * @brief Inits vectors used for the algorithm
     * @return true if init succeed; false otherwise
     */
    bool init();

};

#endif
