#ifndef SKELETON_H
#define SKELETON_H

#include "renderable.h"
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
    std::vector<Sphere*> getBalls() const;
    std::vector<Segment*> getEdges() const;

    bool betweenBalls();
    void setBetweenBalls(bool draw);

    bool loadFromFile(const std::string &fileName);


private:
    std::vector<Sphere*> balls;     // Balls
    std::vector<Segment*> edges;    // Edges

    // true if we want to draw inbetween-balls; false otherwise
    bool drawBetween;


    std::vector<float> splitSpaces(const std::string &s);

};


// Useful for debug
std::ostream& operator<<(std::ostream &out, const Skeleton &s);

#endif // SKELETON_H
