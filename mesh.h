#ifndef DEF_MESH
#define DEF_MESH

#include <iostream>
#include <string>
#include <vector>
#include <QGLViewer/qglviewer.h>
#include "renderable.h"


class Vertex {
    public:
        Vertex();
        Vertex(qglviewer::Vec pos);
        Vertex(qglviewer::Vec &pos, qglviewer::Vec &n);

        qglviewer::Vec& getPos();
        qglviewer::Vec& getNormal();
        void setPos(qglviewer::Vec &pos);
        void setNormal(qglviewer::Vec n);

   private:
        qglviewer::Vec pos;
        qglviewer::Vec normal;

        void draw();
};


class Triangle {
    public:
        Triangle(int a = 0, int b = 0, int c = 0);

        int a();
        int b();
        int c();
        void setA(int a);
        void setB(int b);
        void setC(int c);

    private:
        int ta, tb, tc;    // We only store vertices indexes (in the vertice
                           // vector included in Mesh, to save memory space)
};


class Quadrangle {
    public:
        Quadrangle(int a = 0, int b = 0, int c = 0, int d = 0);

        int a();
        int b();
        int c();
        int d();
        void setA(int a);
        void setB(int b);
        void setC(int c);
        void setD(int d);

    private:
        int qa, qb, qc, qd;
};



class Mesh : public Renderable {
    public:
        Mesh();

        std::vector<Vertex>& getVertices();
        std::vector<qglviewer::Vec>& getNormals();
        std::vector<Triangle>& getTriangles();
        std::vector<Quadrangle>& getQuads();

        bool saveMesh(const std::string &fileName);

        void draw();

    private:
        std::vector<Vertex>     vertices;
        std::vector<qglviewer::Vec> normals;
        std::vector<Triangle>   triangles;
        std::vector<Quadrangle> quads;

};

#endif
