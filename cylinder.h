#ifndef CYLINDER_H
#define CYLINDER_H

#include "QGLViewer/qglviewer.h"
#include "renderable.h"

class Cylinder : public Renderable
{
public:
    Cylinder(qglviewer::Vec& pos1, qglviewer::Vec& pos2, float radius);
    void draw();

private:
    qglviewer::Vec pos1;
    qglviewer::Vec pos2;
    float r;
};

#endif // CYLINDER_H
