#ifndef DEF_CURVATURETENSOR
#define DEF_CURVATURETENSOR

#include <vector>
#include "omeigen.h"

typedef OpenMesh::PolyMesh_ArrayKernelT<> BMesh;

/**
 * @brief Compute the curvature tensor in a point p
 */
class CurvatureTensor
{

public:
    /**
     * @brief Constructor
     * @param m Mesh to work with
     */
    CurvatureTensor(const BMesh &m);

    /**
     * @brief Computes the Weingarten matrix, and sets its eigenvalues and
     *        eigenvectors
     * @param p Point where you need to compute the matrix
     * @param neighbors p neighbors (necessary to compute W)
     * @return true if process succeed; false otherwise
     */
    bool compute(const BMesh::VertexHandle &p,
                 const std::vector<BMesh::VertexHandle> &neighbors);

    /**
     * @brief Gets eigenvalues of W. compute MUST be called before
     *        getCurvatures
     * @return principal curtavures at point p
     */
    std::vector<float> getCurvatures() const;

    /**
     * @brief Gets eigenvectors of W (in Eigen format).
     *        compute MUST be called before getEigenDirections
     * @return principal directions at point p
     */
    std::vector<Eigen::Vector2f> getEigenDirections() const;

    /**
     * @brief Gets eigenvectors of W (in OpenMesh format).
     *        compute MUST be called before getOMDirections
     * @return principal directions at point p
     */
    std::vector<OpenMesh::Vec2f> getOMDirections() const;

private:
    BMesh m;
    BMesh::VertexHandle p;
    std::vector<BMesh::VertexHandle> neighbors;

    std::vector<float> curvatures;
    std::vector<Eigen::Vector2f> directions;

    std::vector<Eigen::Vector3f> getTangentPlane();
};

#endif
