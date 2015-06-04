#ifndef CYLINDER_H
#define CYLINDER_H

#include <QGLViewer/qglviewer.h>
#include "renderable.h"


class Cylinder : public Renderable
{

public:
    /**
     * @brief Constructor
     * @param pos1 Bottom base
     * @param pos2 Top base
     * @param radius Radius
     */
    Cylinder(qglviewer::Vec& pos1, qglviewer::Vec& pos2, float radius);

    /**
     * @brief Draws the cylinder
     */
    void draw();

private:
    qglviewer::Vec pos1;
    qglviewer::Vec pos2;
    float r;
};

#endif // CYLINDER_H
