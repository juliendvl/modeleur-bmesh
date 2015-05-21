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

std::vector<Sphere*>& Skeleton::getBalls() {
    return this->balls;
}

std::vector<Segment*>& Skeleton::getEdges() {
    return this->edges;
}
