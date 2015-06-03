#include <iostream>
#include <list>
#include <QMap>
#include "meshevolve.h"
#include "curvaturetensor.h"

using namespace std;
using OpenMesh::Vec3f;


///////////////////////////////////////////////////////////////////////////////
MeshEvolve::MeshEvolve(Mesh &mesh, Skeleton *s) {
    this->m = mesh.getPMesh();
    this->s = s;
}


///////////////////////////////////////////////////////////////////////////////
bool MeshEvolve::evolve() {
    const float eps = 0.31;     // Threshold parameter
    const float dt  = 1.0;      // Fixed at first
    BMesh::VertexIter vit;
    BMesh::VertexVertexIter vv;
    list<BMesh::VertexHandle> points;

    // We only add points which need to evolve
    for (vit = m->vertices_begin(); vit != m->vertices_end(); ++vit) {
        int k = 0;
        for (vv = m->vv_iter(*vit); vv.is_valid(); ++vv)
            k++;
        if (k >= 3)
            points.push_back(*vit);
    }

    // While there are points which need to evolve
    while (points.size() != 0) {
        QMap<BMesh::VertexHandle, Vec3f> ptsMap;
        list<BMesh::VertexHandle>::iterator lit;

        for (lit = points.begin(); lit != points.end(); ++lit) {
            Vec3f p = m->point(*lit);

            // We compute the curvature tensor and get the principal curvatures
            vector<BMesh::VertexHandle> neighbors;
            for (vv = m->vv_iter(*lit); vv.is_valid(); ++vv)
                neighbors.push_back(*vv);
            CurvatureTensor ct(*m);
            ct.compute(*lit, neighbors);
            vector<float> curvatures = ct.getCurvatures();

            // We compute the motion speed function
            float f = 1.0 / (1.0 + fabs(curvatures[0]) + fabs(curvatures[1]));
            float F = scalarField(p) * f;

            // We compute the new point
            Vec3f n = scalarNormal(p);
            Vec3f newPt = p + n * F * dt;
            ptsMap.insert(*lit, newPt);
        }

        // We update positions and check if some points do not need to
        // evolve anymore
        QMap<BMesh::VertexHandle, Vec3f>::iterator mit;
        for (mit = ptsMap.begin(); mit != ptsMap.end(); ++mit) {
            m->set_point(mit.key(), mit.value());

            if ( scalarField(mit.value()) < eps )
                points.remove(mit.key());
        }
    }

    // We finally update normals
    m->update_normals();

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

    Vec3f n(-nx, -ny, -nz);

    if (n.norm() >= 1e-4)
        n.normalize();

    return n;
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
    vector<Sphere*> balls = s->getBalls();
    float res = 0.0;
    const float T = 0.2;

    // We first get all balls (key and in-between balls)
    vector< vector<Segment*> >e = s->getEdges();
    for (unsigned int i = 0; i < e.size(); ++i) {
        for (unsigned int j = 0; j < e.size(); ++j) {
            if (e[i][j] != NULL) {
                vector<Sphere*> ibb = e[i][j]->getInBetweenBalls();
                balls.insert(balls.end(), ibb.begin(), ibb.end());
            }
        }
    }

    for (unsigned int i = 0; i < balls.size(); ++i)
        res += fi(balls[i], p);
    return (res - T);
}
