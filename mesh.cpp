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



///////////////////////////////////////////////////////////////////////////////
/// VERTEX CLASS
///////////////////////////////////////////////////////////////////////////////

Vertex::Vertex() {
    this->pos = Vect3();
    this->n = Vect3();
}

Vertex::Vertex(Vect3 pos) : pos(pos) {
    this->n = Vect3();
}

Vertex::Vertex(Vect3 pos, Vect3 n) : pos(pos), n(n) {}

Vect3& Vertex::getPos() {
    return pos;
}

Vect3& Vertex::getNormal() {
    return n;
}

void Vertex::setPos(Vect3 &pos) {
    this->pos = pos;
}

void Vertex::setNormal(Vect3 &n) {
    this->n = n;
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


Mesh::Mesh() {
    this->vertices  = QVector<Vertex>();
    this->triangles = QVector<Triangle>();
    this->quads     = QVector<Quadrangle>();
}

Mesh::Mesh(QVector<Vertex> v, QVector<Triangle> t, QVector<Quadrangle> q) {
    this->vertices  = v;
    this->triangles = t;
    this->quads     = q;
}

void Mesh::init(Viewer &v) {
    (void)v;
}

void Mesh::draw() {
}





