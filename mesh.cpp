#include <iostream>
#include <fstream>
#include <stdexcept>
#ifndef __APPLE__
    #include <GL/glew.h>
    #include <GL/glut.h>
#else
    #include <GLUT/glut.h>
#endif
#include "mesh.h"


using namespace std;
using qglviewer::Vec;



///////////////////////////////////////////////////////////////////////////////
/// VERTEX CLASS
///////////////////////////////////////////////////////////////////////////////

Vertex::Vertex() {
    this->pos = Vec();
    this->normal = Vec();
}

Vertex::Vertex(Vec pos) : pos(pos) {
    this->normal = Vec();
}

Vertex::Vertex(Vec &pos, Vec &n) : pos(pos), normal(n) {}

Vec& Vertex::getPos() {
    return pos;
}

Vec& Vertex::getNormal() {
    return normal;
}

void Vertex::setPos(Vec &pos) {
    this->pos = pos;
}

void Vertex::setNormal(Vec n) {
    this->normal = n;
}

void Vertex::draw() {

}



///////////////////////////////////////////////////////////////////////////////
/// TRIANGLE CLASS
///////////////////////////////////////////////////////////////////////////////

Triangle::Triangle(int a, int b, int c) : ta(a), tb(b), tc(c) {}

int Triangle::a() {
    return ta;
}

int Triangle::b() {
    return tb;
}

int Triangle::c() {
    return tc;
}

void Triangle::setA(int a) {
    this->ta = a;
}

void Triangle::setB(int b) {
    this->tb = b;
}

void Triangle::setC(int c) {
    this->tc = c;
}



///////////////////////////////////////////////////////////////////////////////
/// QUADRANGLE CLASS
///////////////////////////////////////////////////////////////////////////////

Quadrangle::Quadrangle(int a, int b, int c, int d) : qa(a), qb(b), qc(c), qd(d) {}

int Quadrangle::a() {
    return qa;
}

int Quadrangle::b() {
    return qb;
}

int Quadrangle::c() {
    return qc;
}

int Quadrangle::d() {
    return qd;
}

void Quadrangle::setA(int a) {
    this->qa = a;
}

void Quadrangle::setB(int b) {
    this->qb = b;
}

void Quadrangle::setC(int c) {
    this->qc = c;
}

void Quadrangle::setD(int d) {
    this->qd = d;
}




///////////////////////////////////////////////////////////////////////////////
/// MESH CLASS
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
Mesh::Mesh() {
    this->vertices  = vector<Vertex>();
    this->triangles = vector<Triangle>();
    this->quads     = vector<Quadrangle>();
}


///////////////////////////////////////////////////////////////////////////////
vector<Vertex>& Mesh::getVertices() {
    return vertices;
}

vector<Vec>& Mesh::getNormals() {
    return normals;
}

vector<Triangle>& Mesh::getTriangles() {
    return triangles;
}

vector<Quadrangle>& Mesh::getQuads() {
    return quads;
}


///////////////////////////////////////////////////////////////////////////////
void Mesh::draw() {
}


////////////////////////////////////////////////////////////////////////////////
bool Mesh::saveMesh(const string &fileName) {
    ofstream file(fileName.c_str(), ios::out | ios::trunc);
    if (!file.is_open())
        return false;

    file << "#File automatically generated" << endl;
    file << endl << "o object" << endl << endl;

    // Saving vertices
    file << "#Vertices" << endl;
    for (unsigned int i = 0; i < vertices.size(); i++) {
        Vec &v = vertices[i].getPos();
        file << "v " << v[0] << " " << v[1] << " " << v[2] << endl;
    }

    // Saving normals
    file << endl << "#Normals" << endl;
    for (unsigned int i = 0; i < normals.size(); i++) {
        Vec v = normals[i];
        file << "vn " << v[0] << " " << v[1] << " " << v[2] << endl;
    }

    // Saving triangle faces
    file << endl << "#Triangle faces" << endl;
    for (unsigned int i = 0; i < triangles.size(); i++) {
        Triangle t = triangles[i];

        file << t.a() << "//" << vertices[t.a()].getNormal() << " ";
        file << t.b() << "//" << vertices[t.b()].getNormal() << " ";
        file << t.c() << "//" << vertices[t.c()].getNormal() << " " << endl;
    }

    // Saving quad faces
    file << endl << "#Quad faces" << endl;
    for (unsigned int i = 0; i < quads.size(); i++) {
        Quadrangle q = quads[i];

        file << q.a() << "//" << vertices[q.a()].getNormal() << " ";
        file << q.b() << "//" << vertices[q.b()].getNormal() << " ";
        file << q.c() << "//" << vertices[q.c()].getNormal() << " ";
        file << q.d() << "//" << vertices[q.d()].getNormal() << " " << endl;
    }

    file.close();
    return true;
}





