#include "cylinder.h"
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

Cylinder::Cylinder(qglviewer::Vec& pos1, qglviewer::Vec& pos2, float radius)
{
    this->pos1 = pos1;
    this->pos2 = pos2;
    this->r = radius;
}

void Cylinder::draw() {
    glPushMatrix();
    glColor3f(1.0,1.0,1.0);
    GLUquadric *p = gluNewQuadric();
    gluQuadricDrawStyle(p,GLU_FILL);
    float height = sqrt((pos1[0]-pos2[0])*(pos1[0]-pos2[0]) + (pos1[1]-pos2[1])*(pos1[1]-pos2[1]) + (pos1[2]-pos2[2])*(pos1[2]-pos2[2]));
    // On positionne un cylindre au niveau de pos1
    glTranslatef(pos1[0],pos1[1],pos1[2]);
    // On calcule l'axe de rotation
    float x1 = (pos1[0]+pos2[0])/2.0-pos1[0];
    float y1 = (pos1[1]+pos2[1])/2.0-pos1[1];
    float z1 = (pos1[2]+height+pos2[2])/2.0-pos1[2];
    // On positionne le cylindre selon l'axe défini par les deux positions
    glRotatef(180.0,x1,y1,z1);
    // On dessine le cylindre
    gluCylinder(p, this->r, this->r, height, 50, 50);
    glPopMatrix();
}
