#ifndef SEGMENT_H
#define SEGMENT_H

#include <iostream>

class Segment
{
public:
    Segment(int index1, int index2);
    int getIndex1() const;
    int getIndex2() const;

private:
    int index1;
    int index2;
};

std::ostream& operator<<(std::ostream &out, const Segment &s);

#endif // SEGMENT_H
