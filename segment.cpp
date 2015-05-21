#include "segment.h"

Segment::Segment(int index1, int index2) {
    this->index1 = index1;
    this->index2 = index2;
}

int Segment::getIndex1() const {
    return this->index1;
}

int Segment::getIndex2() const {
    return this->index2;
}


std::ostream& operator<<(std::ostream &out, const Segment &s) {
    out << "edge " << s.getIndex1() << " " << s.getIndex2();
    return out;
}
