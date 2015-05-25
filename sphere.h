#ifndef DEF_SPHERE
#define DEF_SPHERE

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include "renderable.h"
#include <QGLViewer/qglviewer.h>

class Sphere : public Renderable
{
    public:
        /** Constructeurs **/
        Sphere();
        Sphere(float x, float y, float z, float radius, float r=1.0, float g=1.0, float b=1.0);

        void draw();

        /** Accesseurs / Mutateurs **/
        float getRadius() const;
        float getX() const;
        float getY() const;
        float getZ() const;
        bool getSweeped() const;
        std::vector<int> getNeighbors() const;

        void setRadius(float radius);
        void setX(float x);
        void setY(float y);
        void setZ(float z);

        void setColor(float r, float g, float b);
        void setSweeped(bool b);

        void addNeighbor(int n);
        int valence();

        void operator=(const Sphere& s);

	private:
		float radius;                 // Rayon
        float x;                      // Position de la sphère
        float y;
        float z;

        // Couleurs
        float r;
        float g;
        float b;

        bool sweeped;

        std::vector<int> neighbors;
};

std::ostream& operator<<(std::ostream &out, const Sphere &s);

#endif
