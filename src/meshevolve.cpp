#include <iostream>
#include <limits>
#include <list>
#include <QMap>
#include "meshevolve.h"
#include "curvaturetensor.h"

using namespace std;
using OpenMesh::Vec3f;


///////////////////////////////////////////////////////////////////////////////
MeshEvolve::MeshEvolve(Mesh &mesh, Skeleton *s, int level) {
    this->m = mesh.getPMesh();
    this->s = s;
    this->level = level;

    // We get all balls (key and in-between balls) to save time
    // calculation
    balls = s->getBalls();
    vector< vector<Segment*> >e = s->getEdges();
    for (unsigned int i = 0; i < e.size(); ++i) {
        for (unsigned int j = 0; j < e.size(); ++j) {
            if (e[i][j] != NULL) {
                vector<Sphere*> ibb = e[i][j]->getInBetweenBalls();
                balls.insert(balls.end(), ibb.begin(), ibb.end());
            }
        }
    }

    computeStep();
}


///////////////////////////////////////////////////////////////////////////////
bool MeshEvolve::evolve() {
    const float eps = 0.80;     // Threshold parameter
    float dt  = 1.0;            // Fixed at first
    BMesh::VertexIter vit;
    BMesh::VertexVertexIter vv;
    list<BMesh::VertexHandle> points;

    for (vit = m->vertices_begin(); vit != m->vertices_end(); ++vit) {
        int k = 0;
        for (vv = m->vv_iter(*vit); vv.is_valid(); ++vv)
            k++;
        if (k >= 3)
            points.push_back(*vit);
    }

    // While there are points which need to evolve
    while (points.size() != 0) {
        QMap<BMesh::VertexHandle, float> F;
        QMap<BMesh::VertexHandle, float>::iterator mit;
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
            F.insert(*lit, scalarField(p) * f);
        }

        // We compute dt
        float Fmax = numeric_limits<float>::min();
        for (mit = F.begin(); mit != F.end(); ++mit)
            if (mit.value() > Fmax)
                Fmax = mit.value();
        dt = 0.75 * (step / Fmax);


        // We update positions and check if some points do not need to
        // evolve anymore
        for (mit = F.begin(); mit != F.end(); ++mit) {
            Vec3f p = m->point(mit.key());
            Vec3f n = scalarNormal(p);
            Vec3f newPt = p + n * mit.value() * dt;
            m->set_point(mit.key(), newPt);

            if ( scalarField(newPt) < eps )
                points.remove(mit.key());
        }
    }

    // We finally update normals
    m->update_normals();

    return true;
}



///////////////////////////////////////////////////////////////////////////////
Vec3f MeshEvolve::scalarNormal(const Vec3f &p) {
    const float eps = 1e-2;

    Vec3f dx(eps, 0.0, 0.0);
    Vec3f dy(0.0, eps, 0.0);
    Vec3f dz(0.0, 0.0, eps);

    float nx = (scalarField(p + dx) - scalarField(p - dx)) / (2*eps);
    float ny = (scalarField(p + dy) - scalarField(p - dy)) / (2*eps);
    float nz = (scalarField(p + dz) - scalarField(p - dz)) / (2*eps);

    Vec3f n(-nx, -ny, -nz);
    cout << n << endl;

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
    float res = 0.0;
    const float T = 0.05;

    for (unsigned int i = 0; i < balls.size(); ++i)
        res += fi(balls[i], p);
    return (res - T);
}



///////////////////////////////////////////////////////////////////////////////
void MeshEvolve::computeStep() {
    float min = numeric_limits<float>::max();

    // step = min{ri} / 2^k, where k is the subdivision level
    for (unsigned int i = 0; i < balls.size(); ++i)
        if (balls[i]->getRadius() < min)
            min = balls[i]->getRadius();

    this->step = min / (1 << level);
}
