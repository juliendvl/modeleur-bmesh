#ifndef SEGMENT_H
#define SEGMENT_H

#include <iostream>
#include <sphere.h>

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

typedef OpenMesh::PolyMesh_ArrayKernelT<> BMesh;

class Segment
{
public:
    Segment(int index1, int index2);
    int getIndex1() const;
    int getIndex2() const;
    std::vector<Sphere*> getInBetweenBalls() const;
    std::vector<Sphere*>& getInBetweenBalls();
    std::vector<BMesh::Point>& getPoints();

    void addInBetweenBall(Sphere* s);
    void addPoint(BMesh::Point p);

private:
    int index1;
    int index2;
    std::vector<Sphere*> inbetweenballs;
    std::vector<BMesh::Point> pointsMesh;
};

std::ostream& operator<<(std::ostream &out, const Segment &s);

#endif // SEGMENT_H
