#include "segment.h"

Segment::Segment(int index1, int index2) {
    this->index1 = index1;
    this->index2 = index2;
}

int Segment::getIndex1() {
    return this->index1;
}

int Segment::getIndex2() {
    return this->index2;
}
