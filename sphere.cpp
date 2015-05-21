#include "sphere.h"
#include <stdexcept>



///////////////////////////////////////////////////////////////////////////////
Sphere::Sphere(float x, float y, float z, float radius, float r, float g, float b) {
    this->x = x;
    this->y = y;
    this->z = z;
    if (radius < 0)
        std::invalid_argument("Radius must not be negative");
    this->radius = radius;
    this->r = r;
    this->g = g;
    this->b = b;
}


///////////////////////////////////////////////////////////////////////////////
void Sphere::draw() {
    glPushMatrix();
    glTranslatef(this->x, this->y, this->z);
    glColor3f(this->r, this->g, this->b);
    glutSolidSphere(this->radius, 10, 10);
    glPopMatrix();
}

///////////////////////////////////////////////////////////////////////////////
float Sphere::getRadius() {
    return this->radius;
}


///////////////////////////////////////////////////////////////////////////////
float Sphere::getX() {
    return this->x;
}


///////////////////////////////////////////////////////////////////////////////
float Sphere::getY() {
    return this->y;
}


///////////////////////////////////////////////////////////////////////////////
float Sphere::getZ() {
    return this->z;
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
void Sphere::operator=(const Sphere& s) {
    this->x = s.x;
    this->y = s.y;
    this->z = s.z;
    this->radius = s.radius;
}

void Sphere::setColor(float r, float g, float b) {
    this->r = r;
    this->g = g;
    this->b = b;
}
