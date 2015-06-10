#ifndef SEGMENT_H
#define SEGMENT_H

#include <iostream>
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

#include "sphere.h"

typedef OpenMesh::PolyMesh_ArrayKernelT<> BMesh;


/**
 * @brief Edge used for the skeleton
 */
class Segment
{
public:
    /**
     * @brief Constructor
     * @param index1 Vertex index of the first ball
     * @param index2 Vertex index of the second ball
     */
    Segment(int index1, int index2);

    /**
     * @brief  Returns the vertex index of the first ball
     * @return Vertex index of the first ball
     */
    int getIndex1() const;

    /**
     * @brief  Returns the vertex index of the second ball
     * @return Vertex index of the second ball
     */
    int getIndex2() const;

    /**
     * @brief  Returns inbetweenballs between the two balls composong the
     *         edge
     * @return inbetweenballs between the two balls composong the edge
     */
    std::vector<Sphere*> getInBetweenBalls() const;

    /**
     * @brief  Returns inbetweenballs between the two balls composong the
     *         edge
     * @return inbetweenballs between the two balls composong the edge
     */
    std::vector<Sphere*>& getInBetweenBalls();
    std::vector<Sphere*>& getInBetweenBallsInverse();

    // A COMMENTER
    std::vector<BMesh::Point>& getPoints();

    // A COMMENTER
    void addInBetweenBall(Sphere* s);

    // A COMMENTER
    void addPoint(BMesh::Point p);

private:
    int index1;
    int index2;
    std::vector<Sphere*> inbetweenballs;
    std::vector<Sphere*> inbetweenballsInverse;
    std::vector<BMesh::Point> pointsMesh;
};

std::ostream& operator<<(std::ostream &out, const Segment &s);

#endif // SEGMENT_H
