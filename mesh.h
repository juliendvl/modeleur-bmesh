#ifndef DEF_MESH
#define DEF_MESH

#include <iostream>
#include <string>
#include <QVector>
#include "vect3.h"
#include "renderable.h"


class Vertex {
    public:
        Vertex();
        Vertex(Vect3 pos);
        Vertex(Vect3 pos, Vect3 n);

        Vect3& getPos();
        Vect3& getNormal();
        void setPos(Vect3 &pos);
        void setNormal(Vect3 &n);

   private:
        Vect3 pos;
        Vect3 n;

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
        Mesh(QVector<Vertex> v, QVector<Triangle> t, QVector<Quadrangle> q);

        QVector<Vertex>& getVertices();
        QVector<Triangle>& getTriangles();
        QVector<Quadrangle>& getQuads();

        void saveMesh(const std::string &file);

        void init(Viewer &);
        void draw();

    private:
        QVector<Vertex>     vertices;
        QVector<Triangle>   triangles;
        QVector<Quadrangle> quads;

        std::string doubleSlash(const std::string &line);
        std::string replaceSlash(const std::string &line);
        QVector<int> splitSpaces(const std::string &line);
};

#endif
