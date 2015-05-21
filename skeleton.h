#ifndef SKELETON_H
#define SKELETON_H

#include "sphere.h"
#include "segment.h"

class Skeleton
{
public:
    Skeleton();

    void addBall(Sphere* s);
    void addEdge(Segment* sg);

    std::vector<Sphere*>& getBalls();
    std::vector<Segment*>& getEdges();


private:
    std::vector<Sphere*> balls;
    std::vector<Segment*> edges;

};

#endif // SKELETON_H
