#include <iostream>
#include <stdexcept>
#include <QWheelEvent>
#ifndef __APPLE__
    #include <GL/glut.h>
#else
    #include <GLUT/glut.h>
#endif

#include "sphere.h"


using namespace std;
using namespace qglviewer;


///////////////////////////////////////////////////////////////////////////////
Sphere::Sphere(float x, float y, float z, float radius,
               float r, float g, float b) : sweeped(false)
{
    this->x = x;
    this->y = y;
    this->z = z;

    if (radius < 0)
        throw std::invalid_argument("Radius must not be negative");
    this->radius = radius;

    this->r = r;
    this->g = g;
    this->b = b;

    this->neighbors = std::vector<int>();

    // Sets initial position of the frame linked to the sphere
    setZoomSensitivity(0.0);
    setPosition(x, y, z);
}


///////////////////////////////////////////////////////////////////////////////
void Sphere::draw() {
    glPushMatrix();

    // Updates sphere position, according to frame position
    setCenter(position());
    glTranslatef(x, y, z);
    glColor3f(this->r, this->g, this->b);
    glutSolidSphere(this->radius, 20, 20);

    glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////
float Sphere::getRadius() const {
    return this->radius;
}


///////////////////////////////////////////////////////////////////////////////
float Sphere::getX() const {
    return this->x;
}


///////////////////////////////////////////////////////////////////////////////
float Sphere::getY() const {
    return this->y;
}


///////////////////////////////////////////////////////////////////////////////
float Sphere::getZ() const {
    return this->z;
}

////////////////////////////////////////////////////////////////////////////////
bool Sphere::getSweeped() const {
    return this->sweeped;
}

///////////////////////////////////////////////////////////////////////////////
std::vector<int> Sphere::getNeighbors() const {
    return this->neighbors;
}

///////////////////////////////////////////////////////////////////////////////
std::vector<int>& Sphere::getNeighbors() {
    return this->neighbors;
}

///////////////////////////////////////////////////////////////////////////////
void Sphere::setRadius(float radius) {
    if (radius < 0)
        std::invalid_argument("Radius must not be negative");
	this->radius = radius;
}


///////////////////////////////////////////////////////////////////////////////
void Sphere::setX(float x) {
    this->x = x;
}


///////////////////////////////////////////////////////////////////////////////
void Sphere::setY(float y) {
    this->y = y;
}


///////////////////////////////////////////////////////////////////////////////
void Sphere::setZ(float z) {
    this->z = z;
}

///////////////////////////////////////////////////////////////////////////////
void Sphere::setCenter(const Vec &pos) {
    this->x = pos[0];
    this->y = pos[1];
    this->z = pos[2];
}

///////////////////////////////////////////////////////////////////////////////
void Sphere::setSweeped(bool b) {
    this->sweeped = b;
}

///////////////////////////////////////////////////////////////////////////////
void Sphere::addNeighbor(int n) {
    neighbors.push_back(n);
}


///////////////////////////////////////////////////////////////////////////////
int Sphere::valence() {
    return neighbors.size();
}


///////////////////////////////////////////////////////////////////////////////
void Sphere::operator=(const Sphere& s) {
    this->x = s.x;
    this->y = s.y;
    this->z = s.z;
    this->radius = s.radius;
}

///////////////////////////////////////////////////////////////////////////////
void Sphere::setColor(float r, float g, float b) {
    this->r = r;
    this->g = g;
    this->b = b;
}

///////////////////////////////////////////////////////////////////////////////
void Sphere::wheelEvent(QWheelEvent* const event, Camera* const camera) {
    Qt::KeyboardModifiers modifiers = event->modifiers();

    if (modifiers == Qt::ShiftModifier) {
        int step = event->delta();
        float coef = step / 100.0;
        if (step < 0)
            coef = 1.0 / (-coef);

        radius *= coef;
    }
}


///////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream &out, const Sphere &s) {
    out << "x = " << s.getX() << "; y = " << s.getY();
    out << "; z = " << s.getZ() << "; r = " << s.getRadius();
    return out;
}
