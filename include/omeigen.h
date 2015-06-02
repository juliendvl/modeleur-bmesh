#ifndef DEF_OMEIGEN
#define DEF_OMEIGEN

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <Eigen/Dense>


/**
 * @brief Eigen/OpenMesh vectors conversion methods
 */
class OMEigen
{

public:
    /**
     * @brief Transforms a OpenMesh Vec2f vector to a Eigen Vector2f vector
     * @param p OpenMesh Vec2f vector
     * @return Eigen Vector2f vector
     */
    static Eigen::Vector2f toEigen(const OpenMesh::Vec2f &p) {
        return Eigen::Vector2f(p[0], p[1]);
    }


    /**
     * @brief Transforms a OpenMesh Vec3f vector to a Eigen Vector3f vector
     * @param p OpenMesh Vec3f vector
     * @return Eigen Vector3f vector
     */
    static Eigen::Vector3f toEigen(const OpenMesh::Vec3f &p) {
        return Eigen::Vector3f(p[0], p[1], p[2]);
    }


    /**
     * @brief Transforms a Eigen Vector2f vector to a OpenMesh Vec2f vector
     * @param p Eigen Vector2f vector
     * @return OpenMesh Vec2f vector
     */
    static OpenMesh::Vec2f toOpenMesh(const Eigen::Vector2f &p) {
        return OpenMesh::Vec2f(p[0], p[1]);
    }


    /**
     * @brief Transforms a Eigen Vector3f vector to a OpenMesh Vec3f vector
     * @param p Eigen Vector2f vector
     * @return OpenMesh Vec3f vector
     */
    static OpenMesh::Vec3f toOpenMesh(const Eigen::Vector3f &p) {
        return OpenMesh::Vec3f(p[0], p[1], p[2]);
    }
};

#endif
