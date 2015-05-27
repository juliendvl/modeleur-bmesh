#ifndef DEF_OMEIGEN
#define DEF_OMEIGEN

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <Eigen/Dense>


class OMEigen
{

public:
    static Eigen::Vector3f toEigen(const OpenMesh::Vec3f &p) {
        return Eigen::Vector3f(p[0], p[1], p[2]);
    }

    static OpenMesh::Vec3f toOpenMesh(const Eigen::Vector3f &p) {
        return OpenMesh::Vec3f(p[0], p[1], p[2]);
    }
};

#endif
