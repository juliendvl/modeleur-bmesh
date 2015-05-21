#include "skeleton.h"

Skeleton::Skeleton()
{
}

void Skeleton::addBall(Sphere* s) {
    this->balls.push_back(s);
}

void Skeleton::addEdge(Segment* sg) {
    this->edges.push_back(sg);
}

const std::vector<Sphere*>& Skeleton::getBalls() const{
    return this->balls;
}

const std::vector<Segment*>& Skeleton::getEdges() const{
    return this->edges;
}
