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
    for (unsigned int i = 0; i < edges.size(); i++) {
        for (unsigned int j = 0; j < edges.size(); j++) {
            Segment* sg = edges[i][j];
            if (sg != NULL) {
                for(std::vector<Sphere*>::iterator it = sg->getInBetweenBalls().begin() ; it != sg->getInBetweenBalls().end(); it++){
                    Sphere* s = *it;
                    delete s;
                }
                delete sg;
            }
        }
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
    for (unsigned int i = 0; i < edges.size(); i++) {
        for (unsigned int j = 0; j < edges.size(); j++) {
            Segment* sg = edges[i][j];
            if (sg != NULL) {
                qglviewer::Vec pos1(balls[i]->getX(), balls[i]->getY(), balls[i]->getZ());
                qglviewer::Vec pos2(balls[j]->getX(), balls[j]->getY(), balls[j]->getZ());
                Cylinder c(pos1,pos2,r_min/2.0);
                c.draw();
                if (drawBetween) {
                    for(std::vector<Sphere*>::iterator it = sg->getInBetweenBalls().begin() ; it != sg->getInBetweenBalls().end(); it++){
                        Sphere* s = *it;
                        s->draw();
                    }
                }
            }
        }
    }
}

void Skeleton::addBall(Sphere* s) {
    this->balls.push_back(s);
}

void Skeleton::addEdge(Segment* sg) {
    int i = sg->getIndex1();
    int j = sg->getIndex2();
    this->edges[i][j] = sg;
}

void Skeleton::interpolation() {
    for (unsigned int i = 0; i < edges.size(); i++) {
        for (unsigned int j = 0; j < edges.size(); j++) {
            Segment* sg = edges[i][j];
            if (sg != NULL) {
                //printf("i = %i, j = %i, sg1 = %i, sg2 = %i\n", i,j,sg->getIndex1(),sg->getIndex2());
                Sphere* s1 = balls[i];
                Sphere* s2 = balls[j];
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
                //  somme(r1+h*k) = r2 avec la somme sur k = 1..N
                float max;
                if (r1 < r2) {
                    max = (2*dist-r2+r1)/(r2+r1);
                } else {
                    max = (2*dist-r1+r2)/(r1+r2);
                }
                float h = (r2-r1)/max;
                for (int k = 1; k <= max; k++) {
                    float r = r1 + h*k;
                    sg->getInBetweenBalls().push_back(new Sphere(x1 + r*dx,
                                                                 y1 + r*dy,
                                                                 z1 + r*dz,
                                                                 r));
                    x1 = x1 + r*dx;
                    y1 = y1 + r*dy;
                    z1 = z1 + r*dz;
                }
            }
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

std::vector< std::vector<Segment*> >& Skeleton::getEdges() {
    return this->edges;
}

std::vector<Sphere*> Skeleton::getBalls() const {
    return this->balls;
}

std::vector< std::vector<Segment*> > Skeleton::getEdges() const {
    return this->edges;
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
    int nv = 0;
    while (getline(file, line)) {
        // Vertex ?
        if (line[0] == 'v') {
            nv++;
        }
    }

    this->edges = vector< vector<Segment*> >(nv);
    for (unsigned int i = 0; i < edges.size(); i++) {
        edges[i] = vector<Segment*>(nv);
    }

    file.clear();
    file.seekg(0,ios_base::beg);

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
        for (unsigned int j = 0; j < edges.size(); j++) {
            if (edges[i][j] != NULL) {
                balls[i]->addNeighbor(j);
                balls[j]->addNeighbor(i);
            }
        }
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
    std::vector< std::vector<Segment*> > e = s.getEdges();
    for (unsigned int i = 0; i < e.size(); i++) {
        for (unsigned int j = 0; j < e.size(); j++) {
            out << *(e[i][j]) << endl;
        }
    }

    return out;
}
