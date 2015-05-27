#ifndef DEF_CURVATURETENSOR
#define DEF_CURVATURETENSOR

#include <vector>
#include "omeigen.h"

typedef OpenMesh::PolyMesh_ArrayKernelT<> BMesh;


class CurvatureTensor
{

public:
    CurvatureTensor(const BMesh &m);

    bool compute(const BMesh::VertexHandle &p,
                 const std::vector<BMesh::VertexHandle> &neighbors);

    std::vector<float> getCurvatures() const;
    std::vector<Eigen::Vector2f> getDirections() const;

private:
    BMesh m;
    BMesh::VertexHandle p;
    std::vector<BMesh::VertexHandle> neighbors;

    std::vector<float> curvatures;
    std::vector<Eigen::Vector2f> directions;

    std::vector<Eigen::Vector3f> getTangentPlane();
};

#endif
