#include "catmullclark.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////////
CatmullClark::~CatmullClark() {
    for (unsigned int i = 0; i < vertices.size(); i++)
        delete vertices[i];
    for (unsigned int i = 0; i < faces.size(); i++)
        delete faces[i];
    for (unsigned int i = 0; i < edges.size(); i++)
        delete edges[i];
}


///////////////////////////////////////////////////////////////////////////////
bool CatmullClark::subdivide(Mesh &m) {
    CatmullClark cc(m);

    if (!cc.init())
        return false;

    return true;
}


///////////////////////////////////////////////////////////////////////////////
CatmullClark::CatmullClark(Mesh &m) {
    this->m = m;
    this->vertices = vector<CCVertex*>();
    this->faces    = vector<CCFace*>();
    this->edges    = vector<CCEdge*>();
}


///////////////////////////////////////////////////////////////////////////////
bool CatmullClark::init() {
    // Copy vertices
    vector<Vertex> &v = m.getVertices();
    for (unsigned int i = 0; i < v.size(); i++) {
        CCVertex *vx = new CCVertex;
        vx->pos = v[i];
        vertices.push_back(vx);
    }

    // Copy faces
    vector<Triangle> t = m.getTriangles();


    return true;
}
