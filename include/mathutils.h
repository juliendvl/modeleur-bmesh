#ifndef DEF_MATHUTILS
#define DEF_MATHUTILS

#include <Eigen/Dense>

/**
 * @brief Math functions
 */
class MathUtils
{

public:
    /**
     * @brief Project a point onto a plane defined by an origin and
     *        a normal
     * @param o Origin
     * @param p Point
     * @param n Normal
     * @return  Projected point
     */
    static Eigen::Vector3f projectPoint(const Eigen::Vector3f &o,
                                        const Eigen::Vector3f &p,
                                        const Eigen::Vector3f &n);


    /**
     * @brief Project a vector onto a plane defined by a normal
     * @param v Vector to project
     * @param n Normal
     * @return Projected vector
     */
    static Eigen::Vector3f projectVector(const Eigen::Vector3f &v,
                                         const Eigen::Vector3f &n);
};

#endif
