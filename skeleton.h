#ifndef SKELETON_H
#define SKELETON_H

#include "renderable.h"
#include "sphere.h"
#include "segment.h"

class Skeleton : public Renderable
{
public:
    Skeleton();
    ~Skeleton();
    void init(Viewer &);
    void draw();

    void addBall(Sphere* s);
    void addEdge(Segment* sg);

    std::vector<Sphere*>& getBalls();
    std::vector<Segment*>& getEdges();
    std::vector<Sphere*> getBalls() const;
    std::vector<Segment*> getEdges() const;
    std::vector<Sphere*>& getInBetweenBalls();

    bool betweenBalls();
    void setBetweenBalls(bool draw);

    bool loadFromFile(const std::string &fileName);


private:
    std::vector<Sphere*> balls;     // Balls
    std::vector<Segment*> edges;    // Edges
    std::vector<Sphere*> inbetweensBalls;
    // true if we want to draw inbetween-balls; false otherwise
    bool drawBetween;


    std::vector<float> splitSpaces(const std::string &s);
    void interpolation();

};


// Useful for debug
std::ostream& operator<<(std::ostream &out, const Skeleton &s);

#endif // SKELETON_H
