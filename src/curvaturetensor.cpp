#include <iostream>
#include <Eigen/Eigenvalues>
#include "include/curvaturetensor.h"
#include "mathutils.h"

using namespace std;
using namespace Eigen;
using OpenMesh::Vec2f;


///////////////////////////////////////////////////////////////////////////////
CurvatureTensor::CurvatureTensor(const BMesh &m) : m(m) {}



///////////////////////////////////////////////////////////////////////////////
bool CurvatureTensor::compute(const BMesh::VertexHandle &p,
                              const vector<BMesh::VertexHandle> &neighbors)
{
    this->p = p;
    this->neighbors = neighbors;
    curvatures.clear();
    directions.clear();

    Vector3f px = OMEigen::toEigen(m.point(p));
    Vector3f nx = OMEigen::toEigen(m.normal(p));

    // We construct the tangent plane
    tp = getTangentPlane(px, nx);

    // We only need 2 neighbors to compute Weingarten matrix
    Vector3f n0 = OMEigen::toEigen(m.normal(neighbors[1]));
    Vector3f n1 = OMEigen::toEigen(m.normal(neighbors[2]));
    Vector3f p0 = OMEigen::toEigen(m.point(neighbors[1]));
    Vector3f p1 = OMEigen::toEigen(m.point(neighbors[2]));


    float n0u = (n0 - nx).dot(tp[0]);
    float n0v = (n0 - nx).dot(tp[1]);
    float n1u = (n1 - nx).dot(tp[0]);
    float n1v = (n1 - nx).dot(tp[1]);

    float e0u = (p0 - px).dot(tp[0]);
    float e0v = (p0 - px).dot(tp[1]);
    float e1u = (p1 - px).dot(tp[0]);
    float e1v = (p1 - px).dot(tp[1]);


    // We construct the linear system
    Matrix4f A;
    A << e0u, e0v,   0,   0,
         e1u, e1v,   0,   0,
         0  , 0  , e0u, e0v,
         0  , 0  , e1u, e1v;

    Vector4f B;
    B << n0u, n1u, n0v, n1v;

    // We solve the linear system
    Vector4f X = A.jacobiSvd(ComputeFullU | ComputeFullV).solve(B);

    // We construct the Weingarten matrix
    Matrix2f W;
    W << X(0), X(1), X(2), X(3);
    //cout << W << endl << endl;

    // We set curvatures and directions
    EigenSolver<Matrix2f> es(W);

    Vector2cf eval = es.eigenvalues();
    curvatures.push_back(eval[0].real());
    curvatures.push_back(eval[1].real());

    Matrix2cf evec = es.eigenvectors();
    directions.push_back(Vector2f(evec(0,0).real(), evec(0,1).real()));
    directions.push_back(Vector2f(evec(1,0).real(), evec(1,1).real()));

    directions[0].normalize();
    directions[1].normalize();

    return true;
}



///////////////////////////////////////////////////////////////////////////////
vector<float> CurvatureTensor::getCurvatures() const {
    return this->curvatures;
}



///////////////////////////////////////////////////////////////////////////////
vector<Vector2f> CurvatureTensor::getDirections() const {
    return this->directions;
}


///////////////////////////////////////////////////////////////////////////////
vector<Vector3f> CurvatureTensor::getTangentPlane(const Vector3f &px,
                                                  const Vector3f &nx)
{
    vector<Vector3f> res;

    // First, we must project ont neighbor in the tangent plane
    Vector3f pnbor = OMEigen::toEigen(m.point(neighbors[0]));
    pnbor = MathUtils::project(px, pnbor, nx);

    // "X" axis
    Vector3f xAxis = pnbor - px;
    xAxis.normalize();
    res.push_back(xAxis);

    // "Y" axis
    Vector3f yAxis = nx.cross(xAxis);
    yAxis.normalize();
    res.push_back(yAxis);

    res.push_back(nx);

    return res;
}



///////////////////////////////////////////////////////////////////////////////
vector<Vector3f> CurvatureTensor::tangentPlane() const {
    return tp;
}








