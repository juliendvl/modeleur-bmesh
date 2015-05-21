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

    const std::vector<Sphere*>& getBalls() const;
    const std::vector<Segment*>& getEdges() const;


private:
    std::vector<Sphere*> balls;
    std::vector<Segment*> edges;

};

#endif // SKELETON_H
