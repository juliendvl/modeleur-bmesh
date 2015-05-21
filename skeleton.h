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

    bool loadFromFile(const std::string &fileName);


private:
    std::vector<Sphere*> balls;
    std::vector<Segment*> edges;

    std::vector<float> splitSpaces(const std::string &s);

};


// Useful for debug
std::ostream& operator<<(std::ostream &out, const Skeleton &s);

#endif // SKELETON_H
