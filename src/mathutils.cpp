#include <cassert>
#include "mathutils.h"

using namespace Eigen;


///////////////////////////////////////////////////////////////////////////////
Vector3f MathUtils::project(const Vector3f &o,
                            const Vector3f &p,
                            const Vector3f &n)
{
    float norm = n.norm();
    assert(n.norm() - 1 < 1e-6);

    Vector3f v = p - o;
    float f = v.dot(n);

    return (p - f*n);
}

