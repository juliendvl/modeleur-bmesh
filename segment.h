#ifndef SEGMENT_H
#define SEGMENT_H

class Segment
{
public:
    Segment(int index1, int index2);
    int getIndex1();
    int getIndex2();

private:
    int index1;
    int index2;
};

#endif // SEGMENT_H
