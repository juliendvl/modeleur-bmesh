#ifndef SKELETON_H
#define SKELETON_H

#include "renderable.h"
#include "sphere.h"
#include "segment.h"
#include "mesh.h"
using namespace std;
typedef triangulation::Vertex_handle vhandle;
typedef triangulation::Point point;
/**
 * @brief Defines a skeleton
 */
class Skeleton : public Renderable
{

public:
    /**
     * @brief Default constructor
     */
    Skeleton();

    /**
     * @brief Copy constructor
     * @param s skeleton to copy
     */
    Skeleton(const Skeleton &s);

    ~Skeleton();

    /**
     * @brief Adds a ball in the skeleton
     * @param s Ball to add
     */
    void addBall(Sphere* s);

    /**
     * @brief Adds an edge in the skeleton
     * @param sg edge to add
     */
    void addEdge(Segment* sg);

    // A COMMENTER
    bool betweenBalls();

    /**
     * @brief Draws the skeleton
     */
    void draw();

    /**
     * @brief  Returns balls composing the skeleton (nodes)
     * @return balls composing the skeleton
     */
    std::vector<Sphere*>& getBalls();
    std::vector<Sphere*> getBalls() const;

    /**
     * @brief  Returns edges composing the skeleton
     * @return edges composing the skeleton
     */
    std::vector< std::vector<Segment*> >& getEdges();
    std::vector< std::vector<Segment*> > getEdges() const;

    /**
     * @brief  Returns the mesh linked with the skeleton
     * @return OpenMesh mesh
     */
    Mesh& getMesh();

    /**
     * @brief Generates inbetween-balls
     */
    void interpolation();

    /**
     * @brief Loads a skeleton from a .txt file
     * @param fileName file to load
     * @return true if loading succeed; false otherwise
     */
    bool loadFromFile(const std::string &fileName);

    /**
     * @brief Saves a skeleton
     * @param fileName file to save
     * @return true if save succeed; false otherwise
     */
    bool save(const std::string &fileName);

    /**
     * @brief Indicates if we want to draw inbetween-balls
     * @param draw true if we want to draw inbetween-balls; false otherwise
     */
    void setBetweenBalls(bool draw);

    /**
     * @brief Updates balls neighbors
     */
    void setNeighbors();

    /**
     * @brief Performs sweeping
     */
    void sweeping();

    /**
     * @brief Performs stitching
     */
    void stitching();

private:
    std::vector<Sphere*> balls;                 // Balls
    std::vector< std::vector<Segment*> > edges; // Edges

    // true if we want to draw inbetween-balls; false otherwise
    bool drawBetween;

    Mesh mesh;

    // PRIVATE METHODS
    void createFaces(std::vector<BMesh::VertexHandle>& vhandle, bool endNode);
    std::vector<float> splitSpaces(const std::string &s);
    void sweepVoisin(int origin, int neighbor);
    void orientate(Sphere* s, list<vector<vhandle> >& triangles, vector<vhandle>& vh);
    BMesh::Point toBMeshPoint(const point& p);

};


// Useful for debug
std::ostream& operator<<(std::ostream &out, const Skeleton &s);

#endif // SKELETON_H
