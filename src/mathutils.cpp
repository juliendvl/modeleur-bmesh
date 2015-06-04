#include <cassert>
#include "mathutils.h"

using namespace Eigen;


///////////////////////////////////////////////////////////////////////////////
Vector3f MathUtils::projectPoint(const Vector3f &o,
                                 const Vector3f &p,
                                 const Vector3f &n)
{
    assert(n.norm() - 1 < 1e-6);

    Vector3f v = p - o;
    float f = v.dot(n);

    return (p - f*n);
}


///////////////////////////////////////////////////////////////////////////////
Vector3f MathUtils::projectVector(const Vector3f &v, const Vector3f &n)
{
    assert(n.norm() - 1 < 1e-6);
    return (v - v.dot(n) * n);
}

