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

    bool loadFromFile(const std::string &fileName);


private:
    std::vector<Sphere*> balls;
    std::vector<Segment*> edges;

    std::vector<float> splitSpaces(const std::string &s);

};


// Useful for debug
std::ostream& operator<<(std::ostream &out, const Skeleton &s);

#endif // SKELETON_H
