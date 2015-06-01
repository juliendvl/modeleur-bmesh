#include <fstream>
#include <sstream>
#include "skeleton.h"
#include "cylinder.h"

using namespace std;
using OpenMesh::Vec3f;

Skeleton::Skeleton() : drawBetween(false)
{
}


///////////////////////////////////////////////////////////////////////////////
Skeleton::Skeleton(const Skeleton &s) {
    this->balls = s.balls;
    this->edges = s.edges;
    this->drawBetween = s.drawBetween;
    this->mesh = Mesh(s.mesh);
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
        case 0:
            s->setColor(0.0, 1.0, 0.0);
            break;
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
    for (unsigned int i = 0; i < pointsMesh.size(); i++) {
        pointsMesh[i]->draw();
    }

    mesh.draw();
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
                Sphere* s1 = balls[i];
                Sphere* s2 = balls[j];
                float r1 = s1->getRadius();
                float r2 = s2->getRadius();
                float x1 = s1->getX();
                float y1 = s1->getY();
                float z1 = s1->getZ();
                float dx = s2->getX()-x1;
                float dy = s2->getY()-y1;
                float dz = s2->getZ()-z1;
                float dist = sqrt(dx*dx + dy*dy + dz*dz);
                // If there is an intersection, don't draw the inbetweenballs
                if (r1 + r2 > dist) {
                    continue;
                }
                dx = dx/dist;
                dy = dy/dist;
                dz = dz/dist;
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
    for (unsigned int i = 0; i < balls.size(); i++) {
        Sphere* s = balls[i];
        // If this is a joint node
        if (s->valence() > 2) {
            s->setSweeped(true);
            for (unsigned int compt = 0 ; compt < s->getNeighbors().size(); compt++) {
                sweepVoisin(i,s->getNeighbors()[compt]);
            }
        }
    }
    BMesh& m = mesh.getMesh();
    m.request_vertex_normals();
    m.request_face_normals();
    m.update_normals();
}

void Skeleton::sweepVoisin(int origin, int neighbor) {
    BMesh& m = mesh.getMesh();
    std::vector<BMesh::VertexHandle> vhandle;
    std::vector<BMesh::VertexHandle> vhandle2;
    // We save the four last points that we will need
    // to connect the mesh
    BMesh::VertexIter vit = m.vertices_end();
    for (int count = 0; count < 4; ++count) {
        --vit;
    }
    for (int count = 0; count < 4; ++count) {
        vhandle2.push_back(*vit);
        ++vit;
    }
    Sphere* o = balls[origin];
    Sphere* n = balls[neighbor];
    float x1,y1,z1;
    float cx,cy,cz;
    float nx,ny,nz;
    float r;
    Vec3f axe_x,axe_y,axe_z;
    Vec3f axe2_x, axe3_x, axe3_y, axe3_z;
    Vec3f axe_Z(0,0,1);
    // If it hasn't been sweeped
    if (!n->getSweeped()) {
        //cout << "o = " << origin << " et n = " << neighbor << endl;
        //cout << "valo = " << o->valence() << " valn = " << n->valence() << endl;
        // If it is an end or connection node, it is sweeped
        if ((n->valence() == 1) || (n->valence() == 2)) {
            n->setSweeped(true);
        }

        nx = n->getX();
        ny = n->getY();
        nz = n->getZ();
        x1 = nx - o->getX();
        y1 = ny - o->getY();
        z1 = nz - o->getZ();
        axe_x = Vec3f(x1,y1,z1);
        axe_x.normalize();
        if ((x1 == 0) && (y1 == 0)) {
            if (z1 > 0) {
                axe_y = Vec3f(1,0,0);
                axe_z = Vec3f(0,1,0);
            } else {
                axe_y = Vec3f(1,0,0);
                axe_z = Vec3f(0,-1,0);
            }
        } else {
            axe_y = (axe_Z % axe_x);
            axe_y.normalize();

            axe_z = (axe_x % axe_y);
            axe_z.normalize();
        }

        Segment* sg = edges[origin][neighbor];
        if (sg == NULL) {
            sg = edges[neighbor][origin];
        }

        std::vector<Sphere*> inbetweenballs = sg->getInBetweenBalls();
        if (inbetweenballs.size() > 2) {
            Sphere* b = inbetweenballs[1];
            cx = b->getX();
            cy = b->getY();
            cz = b->getZ();
            r = b->getRadius();

            for (int k = 0; k <= 3; k++) {
                vhandle.push_back(m.add_vertex(BMesh::Point(cx + 2*r*(cos(M_PI/4.0*(2*k+1))*axe_y[0] + sin(M_PI/4.0*(2*k+1))*axe_z[0]),
                                                            cy + 2*r*(cos(M_PI/4.0*(2*k+1))*axe_y[1] + sin(M_PI/4.0*(2*k+1))*axe_z[1]),
                                                            cz + 2*r*(cos(M_PI/4.0*(2*k+1))*axe_y[2] + sin(M_PI/4.0*(2*k+1))*axe_z[2]))));
            }

            // If the origin is a connection node, we buill the mesh
            // between the two edges of the connection node
            if (o->valence() == 2) {
                for (int count = 4; count > 0; --count) {
                    vhandle2.push_back(vhandle[vhandle.size()-count]);
                }
                createFaces(vhandle2, false);
            }

            for (unsigned int count = 3; count < inbetweenballs.size()-1; count = count + 2) {
                b = inbetweenballs[count];
                cx = b->getX();
                cy = b->getY();
                cz = b->getZ();
                r = b->getRadius();
                for (int k = 0; k <= 3; k++) {
                    vhandle.push_back(m.add_vertex(BMesh::Point(cx + 2*r*(cos(M_PI/4.0*(2*k+1))*axe_y[0] + sin(M_PI/4.0*(2*k+1))*axe_z[0]),
                                                                cy + 2*r*(cos(M_PI/4.0*(2*k+1))*axe_y[1] + sin(M_PI/4.0*(2*k+1))*axe_z[1]),
                                                                cz + 2*r*(cos(M_PI/4.0*(2*k+1))*axe_y[2] + sin(M_PI/4.0*(2*k+1))*axe_z[2]))));
                }

                createFaces(vhandle,false);
            }

            // If it is an end or connection node, we build
            // the mesh on the node
            if ((n->valence() == 1)) {
                r = n->getRadius();
                for (int k = 0; k <= 3; k++) {
                    vhandle.push_back(m.add_vertex(BMesh::Point(nx + 2*r*(cos(M_PI/4.0*(2*k+1))*axe_y[0] + sin(M_PI/4.0*(2*k+1))*axe_z[0]),
                                                                ny + 2*r*(cos(M_PI/4.0*(2*k+1))*axe_y[1] + sin(M_PI/4.0*(2*k+1))*axe_z[1]),
                                                                nz + 2*r*(cos(M_PI/4.0*(2*k+1))*axe_y[2] + sin(M_PI/4.0*(2*k+1))*axe_z[2]))));
                }
                createFaces(vhandle,false);
            } else if (n->valence() == 2) {
                Sphere* n2;
                if (n->getNeighbors()[0] == origin) {
                    n2 = balls[n->getNeighbors()[1]];
                } else {
                    n2 = balls[n->getNeighbors()[0]];
                }
                axe2_x = Vec3f(n2->getX() - nx, n2->getY() - ny, n2->getZ() - nz);
                axe2_x.normalize();

                axe3_x = Vec3f((axe_x[0] + axe2_x[0])/2.0,
                        (axe_x[1] + axe2_x[1])/2.0,
                        (axe_x[2] + axe2_x[2])/2.0);
                axe3_x.normalize();

                if ((axe3_x[0] == 0) && (axe3_x[1] == 0)) {
                    if (axe3_x[2] > 0) {
                        axe3_y = Vec3f(1,0,0);
                        axe3_z = Vec3f(0,1,0);
                    } else {
                        axe3_y = Vec3f(1,0,0);
                        axe3_z = Vec3f(0,-1,0);
                    }
                } else {
                    axe3_y = (axe_Z % axe3_x);
                    axe3_y.normalize();

                    axe3_z = (axe3_x % axe3_y);
                    axe3_z.normalize();
                }

                r = n->getRadius();
                for (int k = 0; k <= 3; k++) {
                    vhandle.push_back(m.add_vertex(BMesh::Point(nx + 2*r*(cos(M_PI/4.0*(2*k+1))*axe3_y[0] + sin(M_PI/4.0*(2*k+1))*axe3_z[0]),
                                                                ny + 2*r*(cos(M_PI/4.0*(2*k+1))*axe3_y[1] + sin(M_PI/4.0*(2*k+1))*axe3_z[1]),
                                                                nz + 2*r*(cos(M_PI/4.0*(2*k+1))*axe3_y[2] + sin(M_PI/4.0*(2*k+1))*axe3_z[2]))));
                }
                createFaces(vhandle,false);

            }

        } else {
            // If it is an end or connection node, we build
            // the mesh on the node
            if ((n->valence() == 1)) {
                r = n->getRadius();
                for (int k = 0; k <= 3; k++) {
                    vhandle.push_back(m.add_vertex(BMesh::Point(nx + 2*r*(cos(M_PI/4.0*(2*k+1))*axe_y[0] + sin(M_PI/4.0*(2*k+1))*axe_z[0]),
                                                                ny + 2*r*(cos(M_PI/4.0*(2*k+1))*axe_y[1] + sin(M_PI/4.0*(2*k+1))*axe_z[1]),
                                                                nz + 2*r*(cos(M_PI/4.0*(2*k+1))*axe_y[2] + sin(M_PI/4.0*(2*k+1))*axe_z[2]))));
                }
            } else if (n->valence() == 2) {
                Sphere* n2;
                if (n->getNeighbors()[0] == origin) {
                    n2 = balls[n->getNeighbors()[1]];
                } else {
                    n2 = balls[n->getNeighbors()[0]];
                }
                axe2_x = Vec3f(n2->getX()-nx, n2->getY()-ny, n2->getZ()-nz);
                axe2_x.normalize();

                axe3_x = Vec3f((axe_x[0] + axe2_x[0])/2.0,
                        (axe_x[1] + axe2_x[1])/2.0,
                        (axe_x[2] + axe2_x[2])/2.0);
                axe3_x.normalize();

                if ((axe3_x[0] == 0) && (axe3_x[1] == 0)) {
                    if (axe3_x[2] > 0) {
                        axe3_y = Vec3f(1,0,0);
                        axe3_z = Vec3f(0,1,0);
                    } else {
                        axe3_y = Vec3f(1,0,0);
                        axe3_z = Vec3f(0,-1,0);
                    }
                } else {
                    axe3_y = (axe_Z % axe3_x);
                    axe3_y.normalize();

                    axe3_z = (axe3_x % axe3_y);
                    axe3_z.normalize();
                }

                r = n->getRadius();
                for (int k = 0; k <= 3; k++) {
                    vhandle.push_back(m.add_vertex(BMesh::Point(nx + 2*r*(cos(M_PI/4.0*(2*k+1))*axe3_y[0] + sin(M_PI/4.0*(2*k+1))*axe3_z[0]),
                                                                ny + 2*r*(cos(M_PI/4.0*(2*k+1))*axe3_y[1] + sin(M_PI/4.0*(2*k+1))*axe3_z[1]),
                                                                nz + 2*r*(cos(M_PI/4.0*(2*k+1))*axe3_y[2] + sin(M_PI/4.0*(2*k+1))*axe3_z[2]))));
                }
            }
            if ((n->valence() == 1) || (n->valence() == 2)) {
                // If the origin is a connection node, we buill the mesh
                // between the two edges of the connection node
                if (o->valence() == 2) {
                    for (int count = 4; count > 0; --count) {
                        vhandle2.push_back(vhandle[vhandle.size()-count]);
                    }
                    createFaces(vhandle2, false);
                }
            }
        }

        // If it is an end node, we close the mesh
        if (n->valence() == 1) {
            cx = nx + r*axe_x[0];
            cy = ny + r*axe_x[1];
            cz = nz + r*axe_x[2];
            for (int k = 0; k <= 3; k++) {
                vhandle.push_back(m.add_vertex(BMesh::Point(cx + 2*r*(cos(M_PI/4.0*(2*k+1))*axe_y[0] + sin(M_PI/4.0*(2*k+1))*axe_z[0]),
                                                            cy + 2*r*(cos(M_PI/4.0*(2*k+1))*axe_y[1] + sin(M_PI/4.0*(2*k+1))*axe_z[1]),
                                                            cz + 2*r*(cos(M_PI/4.0*(2*k+1))*axe_y[2] + sin(M_PI/4.0*(2*k+1))*axe_z[2]))));
            }
            createFaces(vhandle, true);

        } else if (n->valence() == 2) {
            // Research of the other neighbor of the connection node
            if (n->getNeighbors()[0] == origin) {
                sweepVoisin(neighbor, n->getNeighbors()[1]);
            } else {
                sweepVoisin(neighbor, n->getNeighbors()[0]);
            }
        }

    }
}

void Skeleton::createFaces(std::vector<BMesh::VertexHandle>& vhandle, bool endNode) {
    BMesh& m = mesh.getMesh();
    std::vector<BMesh::VertexHandle> face_vhandles;
    int last = vhandle.size()-1;
    // Face du dessus
    face_vhandles.clear();
    face_vhandles.push_back(vhandle[last-7]);
    face_vhandles.push_back(vhandle[last-6]);
    face_vhandles.push_back(vhandle[last-2]);
    face_vhandles.push_back(vhandle[last-3]);
    m.add_face(face_vhandles);

    // Face devant
    face_vhandles.clear();
    face_vhandles.push_back(vhandle[last-6]);
    face_vhandles.push_back(vhandle[last-5]);
    face_vhandles.push_back(vhandle[last-1]);
    face_vhandles.push_back(vhandle[last-2]);
    m.add_face(face_vhandles);

    // Face derriere
    face_vhandles.clear();
    face_vhandles.push_back(vhandle[last-7]);
    face_vhandles.push_back(vhandle[last-3]);
    face_vhandles.push_back(vhandle[last]);
    face_vhandles.push_back(vhandle[last-4]);
    m.add_face(face_vhandles);

    // Face dessous
    face_vhandles.clear();
    face_vhandles.push_back(vhandle[last-5]);
    face_vhandles.push_back(vhandle[last-4]);
    face_vhandles.push_back(vhandle[last]);
    face_vhandles.push_back(vhandle[last-1]);
    m.add_face(face_vhandles);

    if (endNode) {
        // Face au bout
        face_vhandles.clear();
        face_vhandles.push_back(vhandle[last-3]);
        face_vhandles.push_back(vhandle[last-2]);
        face_vhandles.push_back(vhandle[last-1]);
        face_vhandles.push_back(vhandle[last]);
        m.add_face(face_vhandles);
    }
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

    /*for (unsigned int i = 0; i < balls.size(); i++) {
        Sphere* s = balls[i];
        if (balls[i]->valence() == 1) {
            cout << "end = " << i << " " << s->getX() << " " << s->getY() << " " << s->getZ() << endl;
        } else if (balls[i]->valence() == 2) {
            cout << "connection = " << i << endl;
        }
    }*/

    return true;
}


///////////////////////////////////////////////////////////////////////////////
Mesh& Skeleton::getMesh() {
    return mesh;
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
            if (e[i][j] != NULL)
                out << *(e[i][j]) << endl;
        }
    }

    return out;
}
