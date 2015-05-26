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

std::vector<Sphere*> Segment::getInBetweenBalls() const {
    return this->inbetweenballs;
}

std::vector<Sphere*>& Segment::getInBetweenBalls() {
    return this->inbetweenballs;
}

void Segment::addInBetweenBall(Sphere* s) {
    inbetweenballs.push_back(s);
}

std::ostream& operator<<(std::ostream &out, const Segment &s) {
    out << "edge " << s.getIndex1() << " " << s.getIndex2();
    return out;
}
