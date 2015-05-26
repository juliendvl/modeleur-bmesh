#include <fstream>
#include <sstream>
#include "skeleton.h"
#include "cylinder.h"

using namespace std;

Skeleton::Skeleton() : drawBetween(false)
{
}

Skeleton::~Skeleton() {
    for(std::vector<Sphere*>::iterator it = balls.begin() ; it != balls.end(); it++){
        Sphere* s = *it;
        delete s;
    }
    for(std::vector<Segment*>::iterator it = edges.begin() ; it != edges.end(); it++){
        Segment* sg = *it;
        delete sg;
    }
    for(std::vector<Sphere*>::iterator it = inbetweensBalls.begin() ; it != inbetweensBalls.end(); it++){
        Sphere* s = *it;
        delete s;
    }
}

void Skeleton::init(Viewer &) {
    this->interpolation();
}

void Skeleton::draw() {
    float r_min = balls[0]->getRadius();
    float r;
    for(vector<Sphere*>::iterator it = balls.begin(); it != balls.end(); it++) {
        Sphere* s = *it;

        // Warning: some balls may be similar but identified with different
        // indexes ! So you could see some end nodes which seem to be
        // joint or connection nodes
        switch (s->valence()) {
            case 1:
                s->setColor(1.0, 1.0, 0.0);
            break;
            case 2:
                s->setColor(0.0, 0.0, 1.0);
            break;
            default:
                s->setColor(1.0, 0.0, 0.0);
            break;
        }

        s->draw();
        r = s->getRadius();
        if (r < r_min) {
            r_min = r;
        }
    }
    for(std::vector<Segment*>::iterator it = edges.begin() ; it != edges.end(); it++){
        Segment* sg = *it;
        qglviewer::Vec pos1(balls[sg->getIndex1()]->getX(), balls[sg->getIndex1()]->getY(), balls[sg->getIndex1()]->getZ());
        qglviewer::Vec pos2(balls[sg->getIndex2()]->getX(), balls[sg->getIndex2()]->getY(), balls[sg->getIndex2()]->getZ());
        Cylinder c(pos1,pos2,r_min/2.0);
        c.draw();
    }

    if (drawBetween) {
        for(std::vector<Sphere*>::iterator it = inbetweensBalls.begin() ; it != inbetweensBalls.end(); it++){
            Sphere* s = *it;
            s->draw();
        }
    }
}

void Skeleton::addBall(Sphere* s) {
    this->balls.push_back(s);
}

void Skeleton::addEdge(Segment* sg) {
    this->edges.push_back(sg);
}

void Skeleton::interpolation() {
    for(std::vector<Segment*>::iterator it = edges.begin() ; it != edges.end(); it++){
        Segment* sg = *it;
        Sphere* s1 = balls[sg->getIndex1()];
        Sphere* s2 = balls[sg->getIndex2()];
        float x1 = s1->getX();
        float y1 = s1->getY();
        float z1 = s1->getZ();
        float dx = s2->getX()-x1;
        float dy = s2->getY()-y1;
        float dz = s2->getZ()-z1;
        float dist = sqrt(dx*dx + dy*dy + dz*dz);
        dx = dx/dist;
        dy = dy/dist;
        dz = dz/dist;
        float r1 = s1->getRadius();
        float r2 = s2->getRadius();
        // si r1 < r2, on a comme égalités :
        //  r1 + max*h = r2 et
        //  somme(r1+h*i) = r2 avec la somme sur i = 1..N
        float max;
        if (r1 < r2) {
            max = (2*dist-r2+r1)/(r2+r1);
        } else {
            max = (2*dist-r1+r2)/(r1+r2);
        }
        float h = (r2-r1)/max;
        for (int i = 1; i <= max; i++) {
            float r = r1 + h*i;
            this->inbetweensBalls.push_back(new Sphere(x1 + r*dx,
                                                       y1 + r*dy,
                                                       z1 + r*dz,
                                                       r));
            x1 = x1 + r*dx;
            y1 = y1 + r*dy;
            z1 = z1 + r*dz;
        }
    }
}

void Skeleton::sweeping() {
    /*for (std::vector<Segment*>::iterator it = balls.begin() ; it != balls.end(); it++) {
        Sphere* s = *it;
        float x1,y1,z1,x2,y2,z2,x3,y3,z3,dist1,dist2;
        // If this is a joint node
        if (s->valence() > 2) {
            for (std::vector<int>::iterator it2 = s->getNeighbors().begin() ; it2 != s->getNeighbors().end(); it2++) {
                int index = *it2;
                Sphere* neighbor = balls[index];
                // If it hasn't get sweeped
                if (!neighbor->getSweeped()) {
                    // If it is an end node
                    if (neighbor->valence() == 1) {
                        x1 = neighbor->getX() - s->getX();
                        y1 = neighbor->getY() - s->getY();
                        z1 = neighbor->getZ() - s->getZ();
                        dist1 = sqrt(x1*x1 + y1*y1 + z1*z1);
                        x1 = x1/dist1;
                        y1 = y1/dist1;
                        z1 = z1/dist1;
                        if ((x1 == 0) && (y1 == 0)) {
                            x2 = 1;
                            y2 = 0;
                            z2 = 0;

                            x3 = 0;
                            y3 = 1;
                            z3 = 0;
                        } else {
                            dist2 = sqrt(x1*x1 + y1*y1);
                            x2 = -y1/dist2;
                            y2 = x1/dist2;
                            z2 = 0;

                            x3 = -x1*z1/dist2;
                            y3 = y1*z1/dist2;
                            z3 = (x1*x1 + y1*y1)/dist2;
                        }
                    }
                }
            }
        }
    }*/
}

std::vector<Sphere*>& Skeleton::getBalls() {
    return this->balls;
}

std::vector<Segment*>& Skeleton::getEdges() {
    return this->edges;
}

std::vector<Sphere*> Skeleton::getBalls() const {
    return this->balls;
}

std::vector<Segment*> Skeleton::getEdges() const {
    return this->edges;
}

std::vector<Sphere*>& Skeleton::getInBetweenBalls() {
    return this->inbetweensBalls;
}


///////////////////////////////////////////////////////////////////////////////
bool Skeleton::betweenBalls() {
    return drawBetween;
}

void Skeleton::setBetweenBalls(bool draw) {
    this->drawBetween = draw;
}



///////////////////////////////////////////////////////////////////////////////
bool Skeleton::loadFromFile(const std::string &fileName) {
    ifstream file(fileName.c_str(), ios::in);
    if (!file.is_open())
        return false;

    string line;
    while (getline(file, line)) {
        // Vertex ?
        if (line[0] == 'v') {
            vector<float> v = splitSpaces(line.substr(2));

            addBall(new Sphere(v[0], v[1], v[2], v[3]));
        }
        else if (line[0] == 'e') {  // Edge ?
            vector<float> v = splitSpaces(line.substr(2));
            addEdge(new Segment(v[0], v[1]));
        }
    }

    file.close();

    setNeighbors();

    return true;
}


///////////////////////////////////////////////////////////////////////////////
void Skeleton::setNeighbors() {
    for (unsigned int i = 0; i < edges.size(); i++) {
        int i1 = edges[i]->getIndex1();
        int i2 = edges[i]->getIndex2();

        balls[i1]->addNeighbor(i2);
        balls[i2]->addNeighbor(i1);
    }
}


vector<float> Skeleton::splitSpaces(const string &s) {
    vector<float> ret;

    string s1 = "";
    for (unsigned int i = 0; i < s.size(); i++) {
        if (s[i] == ' ' || i == s.size() - 1) {
            if (i == s.size() - 1)
                s1+=s[i];

            stringstream ss;
            ss.str(s1);

            float f;
            ss >> f;

            ret.push_back(f);
            s1 = "";
        }
        else
            s1 += s[i];
    }
    return ret;
}


///////////////////////////////////////////////////////////////////////////////
std::ostream& operator<<(std::ostream &out, const Skeleton &s) {
    // Display vertices
    vector<Sphere*> sp = s.getBalls();
    for (unsigned int i = 0; i < sp.size(); i++)
        out << *(sp[i]) << endl;

    // Display edges
    out << endl;
    vector<Segment*> e = s.getEdges();
    for (unsigned int i = 0; i < e.size(); i++)
        out << *(e[i]) << endl;

    return out;
}
