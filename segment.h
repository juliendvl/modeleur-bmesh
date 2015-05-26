#ifndef SEGMENT_H
#define SEGMENT_H

#include <iostream>
#include <sphere.h>

class Segment
{
public:
    Segment(int index1, int index2);
    int getIndex1() const;
    int getIndex2() const;
    std::vector<Sphere*> getInBetweenBalls() const;
    std::vector<Sphere*>& getInBetweenBalls();

    void addInBetweenBall(Sphere* s);

private:
    int index1;
    int index2;
    std::vector<Sphere*> inbetweenballs;
};

std::ostream& operator<<(std::ostream &out, const Segment &s);

#endif // SEGMENT_H
