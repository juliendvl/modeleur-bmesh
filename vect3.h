#ifndef DEF_VECT3
#define DEF_VECT3

#include <iostream>

class Vect3
{
    public:
        Vect3 (float vx = 0.0, float vy = 0.0, float vz = 0.0);
        Vect3 (const Vect3 &v);

        float x() const;
        float y() const;
        float z() const;

        float norm();
        Vect3& normalize();

        Vect3& operator += (const Vect3 &v);
        Vect3& operator -= (const Vect3 &v);
        float  operator *= (const Vect3 &v);

        Vect3& operator += (const float f);
        Vect3& operator -= (const float f);
        Vect3& operator *= (const float f);
        Vect3& operator /= (const float f);


    private:
        float vx;
        float vy;
        float vz;
};


Vect3 operator + (const Vect3 &v, const float f);
Vect3 operator + (const float f, const Vect3 &v);

Vect3 operator - (const Vect3 &v, const float f);
Vect3 operator - (const float f, const Vect3 &v);

Vect3 operator * (const Vect3 &v, const float f);
Vect3 operator * (const float f, const Vect3 &v);

Vect3 operator / (const Vect3 &v, const float f);

Vect3 operator + (const Vect3 &v1, const Vect3 &v2);
Vect3 operator - (const Vect3 &v1, const Vect3 &v2);
float operator * (const Vect3 &v1, const Vect3 &v2);

bool operator == (const Vect3 &v1, const Vect3 &v2);
bool operator != (const Vect3 &v1, const Vect3 &v2);

std::ostream& operator << (std::ostream &out, const Vect3 &v);

#endif
