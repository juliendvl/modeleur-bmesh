#include <iostream>
#include <list>
#include "meshevolve.h"
#include "curvaturetensor.h"

using namespace std;
using OpenMesh::Vec3f;


///////////////////////////////////////////////////////////////////////////////
MeshEvolve::MeshEvolve(Mesh &mesh, Skeleton &s) {
    this->m = mesh.getMesh();
    this->s = s;
}


///////////////////////////////////////////////////////////////////////////////
bool MeshEvolve::evolve() {
    const float EPS = 1e-3;
    const float DT  = 0.05; // Fixed at first
    BMesh::VertexIter vit;

    list<BMesh::VertexHandle> points;
    for (vit = m.vertices_begin(); vit != m.vertices_end(); ++vit)
        points.push_back(*vit);

    // While there are points which need to evolve
    while (points.size() != 0) {
        Vec3f newPt;

        list<BMesh::VertexHandle>::iterator lit;
        for (lit = points.begin(); lit != points.end(); ++lit) {
            Vec3f p  = m.point(*lit);
            Vec3f sn = scalarNormal(p);

            // We get the vertex's neighbors
            BMesh::VertexVertexIter vv;
            vector<BMesh::VertexHandle> neighbors;
            for (vv = m.vv_iter(*lit); vv.is_valid(); ++vv)
                neighbors.push_back(*vv);

            // We get the principal curvatures
            CurvatureTensor ct(m);
            ct.compute(*lit, neighbors);
            vector<float> curv = ct.getCurvatures();

            // We compute the f function
            float f = 1.0 / (1.0 + absf(curv[0]) + absf(curv[1]));
            float F = scalarField(p) * f;

            // We compute the new point
            newPt = p + F * DT * sn;

            // If the point do not need to evolve anymore
            if (scalarField(newPt) < EPS)
                points.remove(*lit);

            m.set_point(*lit, newPt);
        }
    }

    m.update_normals();
    return true;
}


///////////////////////////////////////////////////////////////////////////////
Vec3f MeshEvolve::scalarNormal(const Vec3f &p) {
    const float DP = 1e-2;

    Vec3f dx(DP, 0.0, 0.0);
    Vec3f dy(0.0, DP, 0.0);
    Vec3f dz(0.0, 0.0, DP);

    float nx = (scalarField(p + dx) - scalarField(p - dx)) / (2*DP);
    float ny = (scalarField(p + dy) - scalarField(p - dy)) / (2*DP);
    float nz = (scalarField(p + dz) - scalarField(p - dz)) / (2*DP);

    return -Vec3f(nx, ny, nz);
}


///////////////////////////////////////////////////////////////////////////////
float MeshEvolve::fi(const Sphere *s, const Vec3f &p) {
    const float ALPHA = 1.5;
    float Ri = ALPHA * s->getRadius();

    float rs = (p[0] - s->getX()) * (p[0] - s->getX())
             + (p[1] - s->getY()) * (p[1] - s->getY())
             + (p[2] - s->getZ()) * (p[2] - s->getZ());

    if (rs > Ri*Ri) {
        return 0;
    }
    else {
        float res = (1 - rs / (Ri*Ri));
        return res*res;
    }
}


///////////////////////////////////////////////////////////////////////////////
float MeshEvolve::scalarField(const Vec3f &p) {
    vector<Sphere*> balls = s.getBalls();
    float res = 0.0;
    const float T = 0.3;

    for (unsigned int i = 0; i < balls.size(); i++)
        res += fi(balls[i], p);

    return (res - T);
}



///////////////////////////////////////////////////////////////////////////////
float MeshEvolve::absf(float a) {
    return (a >= 0) ? a : (-a);
}
