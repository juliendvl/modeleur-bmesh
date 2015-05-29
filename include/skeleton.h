#ifndef SKELETON_H
#define SKELETON_H

#include "renderable.h"
#include "sphere.h"
#include "segment.h"
#include "mesh.h"

/* Classe du squelette */

class Skeleton : public Renderable
{
public:
    Skeleton();
    Skeleton(const Skeleton &s);
    ~Skeleton();
    void init(Viewer &);
    void draw();

    void sweeping();
    void addBall(Sphere* s);
    void addEdge(Segment* sg);

    std::vector<Sphere*>& getBalls();
    std::vector< std::vector<Segment*> >& getEdges();
    std::vector<Sphere*> getBalls() const;
    std::vector< std::vector<Segment*> > getEdges() const;

    bool betweenBalls();
    void setBetweenBalls(bool draw);

    bool loadFromFile(const std::string &fileName);

    Mesh& getMesh();


private:
    std::vector<Sphere*> balls;     // Balls
    std::vector< std::vector<Segment*> > edges;    // Edges
    // true if we want to draw inbetween-balls; false otherwise
    bool drawBetween;
    Mesh mesh;

    void setNeighbors();
    std::vector<float> splitSpaces(const std::string &s);
    void interpolation();
    void sweepVoisin(int origin, int neighbor);
    void createFaces(Segment* sg);
    void createFaces(std::vector<BMesh::VertexHandle>& vhandle, bool endNode);

    std::vector<Sphere*> pointsMesh;

};


// Useful for debug
std::ostream& operator<<(std::ostream &out, const Skeleton &s);

#endif // SKELETON_H
