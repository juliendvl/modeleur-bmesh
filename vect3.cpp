#include <stdexcept>
#include <cmath>
#include "vect3.h"


///////////////////////////////////////////////////////////////////////////////
Vect3::Vect3(float vx, float vy, float vz) : vx(vx), vy(vy), vz(vz) {}

///////////////////////////////////////////////////////////////////////////////
Vect3::Vect3 (const Vect3 &v) {
    this->vx = v.x();
    this->vy = v.y();
    this->vz = v.z();
}


///////////////////////////////////////////////////////////////////////////////
float Vect3::x () const {
    return vx;
}

float Vect3::y () const {
    return vy;
}

float Vect3::z () const {
    return vz;
}


///////////////////////////////////////////////////////////////////////////////
float Vect3::norm() {
    return sqrt(vx*vx + vy*vy + vz*vz);
}

Vect3& Vect3::normalize() {
    float m = this->norm();
    (*this) /= m;
    return *this;
}


///////////////////////////////////////////////////////////////////////////////
Vect3& Vect3::operator += (const Vect3 &v) {
    this->vx += v.x();
    this->vy += v.y();
    this->vz += v.z();

    return *this;
}

Vect3& Vect3::operator -= (const Vect3 &v) {
    this->vx -= v.x();
    this->vy -= v.y();
    this->vz -= v.z();

    return *this;
}

float Vect3::operator *= (const Vect3 &v) {
    return this->vx * v.x() + this->vy * v.y() + this->vz * v.z();
}


///////////////////////////////////////////////////////////////////////////////
Vect3& Vect3::operator += (const float f) {
    this->vx += f;
    this->vy += f;
    this->vz += f;

    return *this;
}

Vect3& Vect3::operator -= (const float f) {
    this->vx -= f;
    this->vy -= f;
    this->vz -= f;

    return *this;
}

Vect3& Vect3::operator *= (const float f) {
    this->vx *= f;
    this->vy *= f;
    this->vz *= f;

    return *this;
}

Vect3& Vect3::operator /= (const float f) {
    if (f == 0.0)
        throw std::invalid_argument("Error: dividing by zero !");

    this->vx /= f;
    this->vy /= f;
    this->vz /= f;

    return *this;
}




///////////////////////////////////////////////////////////////////////////////

Vect3 operator + (const Vect3 &v, const float f) {
    Vect3 cv(v);
    return cv += f;
}

Vect3 operator + (const float f, const Vect3 &v) {
    Vect3 cv(v);
    return cv += f;
}

Vect3 operator - (const Vect3 &v, const float f) {
    Vect3 cv(v);
    return cv -= f;
}

Vect3 operator - (const float f, const Vect3 &v) {
    Vect3 cv(v);
    return cv -= f;
}

Vect3 operator * (const Vect3 &v, const float f) {
    Vect3 cv(v);
    return cv *= f;
}

Vect3 operator * (const float f, const Vect3 &v) {
    Vect3 cv(v);
    return cv *= f;
}

Vect3 operator / (const Vect3 &v, const float f) {
    Vect3 cv(v);
    return cv /= f;
}

Vect3 operator + (const Vect3 &v1, const Vect3 &v2) {
    Vect3 cv(v1);
    return cv += v2;
}

Vect3 operator - (const Vect3 &v1, const Vect3 &v2) {
    Vect3 cv(v1);
    return cv -= v2;
}

float operator * (const Vect3 &v1, const Vect3 &v2) {
    Vect3 cv(v1);
    return cv *= v2;
}

bool operator == (const Vect3 &v1, const Vect3 &v2) {
    return v1.x() == v2.x() && v1.y() == v2.y() && v1.z() == v2.z();
}

bool operator != (const Vect3 &v1, const Vect3 &v2) {
    return !(v1 == v2);
}

std::ostream& operator << (std::ostream &out, const Vect3 &v) {
    out << "x = " << v.x() << "; y = " << v.y() << "; z = " << v.z();
    return out;
}
