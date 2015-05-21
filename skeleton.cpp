#include <fstream>
#include "skeleton.h"

using namespace std;

Skeleton::Skeleton()
{
}

void Skeleton::addBall(Sphere* s) {
    this->balls.push_back(s);
}

void Skeleton::addEdge(Segment* sg) {
    this->edges.push_back(sg);
}

const std::vector<Sphere*>& Skeleton::getBalls() const{
    return this->balls;
}

const std::vector<Segment*>& Skeleton::getEdges() const{
    return this->edges;
}


///////////////////////////////////////////////////////////////////////////////
bool Skeleton::loadFromFile(const std::string &fileName) {
    ifstream file(fileName.c_str(), ios::in);
    if (!file.is_open()) {
        cerr << "Unable to open file " << fileName << endl;
        return false;
    }

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
    return true;
}


vector<float> Skeleton::splitSpaces(const string &s) {
    vector<float> ret;

    string s1 = "";
    for (unsigned int i = 0; i < s.size(); i++) {
        if (s[i] == ' ' || i == s.size() - 1) {
            if (i == s.size() - 1)
                s1+=s[i];
            ret.push_back(atof(s1.c_str()));
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
