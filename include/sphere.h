#ifndef DEF_SPHERE
#define DEF_SPHERE

#include <QGLViewer/manipulatedFrame.h>
#include <vector>

#include "renderable.h"
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
# include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
# include <CGAL/Delaunay_triangulation_3.h>
# include <CGAL/Triangulation_vertex_base_with_info_3.h>

typedef OpenMesh::PolyMesh_ArrayKernelT<> BMesh;
typedef CGAL::Epick kernel;
typedef CGAL::Triangulation_vertex_base_with_info_3<BMesh::Point,kernel > vertex_base;
typedef CGAL::Triangulation_data_structure_3< vertex_base > triangulation_data_structure;
typedef CGAL::Delaunay_triangulation_3< kernel, triangulation_data_structure > triangulation;
typedef triangulation::Vertex_handle vhandle;

/**
 * @brief Ball used for the skeleton
 */
class Sphere : public qglviewer::ManipulatedFrame
{

public:
    /**
     * @brief Creates a ball with a position, a radius and a color
     * @param x       x position
     * @param y       y position
     * @param z       z position
     * @param radius  radius
     * @param r       red component
     * @param g       green component
     * @param b       blue component
     */
    Sphere(float x = 0.0, float y = 0.0, float z = 0.0, float radius = 0.0,
           float r = 1.0, float g = 1.0, float b = 1.0);

    /**
     * @brief Draws the ball
     */
    void draw();

    /**
     * @brief Returns the radius
     * @return ball radius
     */
    float getRadius() const;

    /**
     * @brief Returns the x position of the ball
     * @return x position
     */
    float getX() const;

    /**
     * @brief Returns the y position of the ball
     * @return y position
     */
    float getY() const;

    /**
     * @brief Returns the z position of the ball
     * @return z position
     */
    float getZ() const;

    /**
     * @brief Indicates if the ball has been sweeped
     * @return true if the ball has been sweeped; false otherwise
     */
    bool getSweeped() const;

    /**
     * @brief Returns the neighbors of the ball (in the skeleton)
     * @return Ball neighbors
     */
    std::vector<int>  getNeighbors() const;
    std::vector<int>& getNeighbors();

    triangulation& getTriangulation();

    std::vector<std::vector<vhandle> >& getTriangles();
    std::vector<std::vector<BMesh::Point> >& getGroupPoints();

    /**
     * @brief Sets the radius of the ball
     * @param radius new radius
     */
    void setRadius(float radius);

    /**
     * @brief Sets the x position of the ball
     * @param x new x position
     */
    void setX(float x);

    /**
     * @brief Sets the y position of the ball
     * @param y new y position
     */
    void setY(float y);

    /**
     * @brief Sets the z position of the ball
     * @param z new z position
     */
    void setZ(float z);

    /**
     * @brief Sets the center of the sphere
     * @param pos new position
     */
    void setCenter(const qglviewer::Vec &pos);

    /**
     * @brief Sets the color of the ball
     * @param r new red component
     * @param g new green component
     * @param b new blue component
     */
    void setColor(float r, float g, float b);

    /**
     * @brief Changes the sweep state of the ball
     * @param b new state
     */
    void setSweeped(bool b);

    /**
     * @brief Adds a neighbor
     * @param n neighbor index
     */
    void addNeighbor(int n);

    /**
     * @brief Valence of the ball (i.e. the number of neighbors)
     * @return valence of the ball
     */
    int valence();

    /**
     * @brief Affectation operator
     * @param s Ball to copy
     */
    void operator=(const Sphere& s);

    bool inTheSameGroup(BMesh::Point& p1, BMesh::Point& p2, int& first);

    void inGroup(BMesh::Point& p, int& numGroup, int& numPoint);


protected:
    /**
     * @brief Wheel event override
     * @param event  wheel event
     * @param camera camera
     */
    virtual void wheelEvent(QWheelEvent* const event,
                            qglviewer::Camera* const camera);


private:
    float radius;
    float x;
    float y;
    float z;

    float r;
    float g;
    float b;

    bool sweeped;

    std::vector<int> neighbors;
    std::vector<BMesh::Point> points;
    triangulation tr;
    std::vector<std::vector<vhandle> > triangles;
    std::vector<std::vector<BMesh::Point> > groupPoints;
};

/**
 * @brief Displays the sphere (format: x y z radius)
 * @param out output
 * @param s   ball to display
 * @return
 */
std::ostream& operator<<(std::ostream &out, const Sphere &s);

bool operator==(BMesh::Point& p1, BMesh::Point& p2);

#endif
