#include <iostream>
#include "meshevolve.h"

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

    vector<bool> compute(m.n_vertices());
    for (unsigned int i = 0; i < compute.size(); i++)
        compute[i] = true;
    int nbCompute = compute.size();

    while (nbCompute != 0) {
        BMesh::VertexIter vit;
        for (vit = m.vertices_begin(); vit != m.vertices_end(); ++vit) {
            Vec3f p = m.point(*vit);
            Vec3f newPt;
            if (compute[(*vit).idx()]) {
                Vec3f nt = scalarNormal(p);
            }

            // If newPt is in the scalar field
            if (scalarField(newPt) < EPS) {
                compute[(*vit).idx()] = false;
                nbCompute--;
            }
        }
    }

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

    return Vec3f(nx, ny, nz);
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
