#include "dragon.h"

#include <iostream>
#include <cstdio>
#include <stdexcept>
#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif
#include <QGLViewer/qglviewer.h>

Dragon::Dragon()
{
    srand (time(NULL));
    // R = 0.5 pour Omid
    R = 0.1;
    nbSpheresBody = 15;
    nbSpheresTail = 30;
    nbSpheresNeck = 15;
    nbSpheresPaw = 13;
    nbSpheresContourBody = 16;
    nbSpheresContourTail = 16;
    nbSpheresContourNeck = 12;
    nbSpheresContourPaw = 12;
    indexBody = 0;
    indexTail = indexBody + nbSpheresBody;
    indexNeck = indexTail + nbSpheresTail;
    indexPawLeftUp = indexNeck + nbSpheresNeck;
    indexPawRightUp = indexPawLeftUp + nbSpheresPaw;
    indexPawLeftDown = indexPawRightUp + nbSpheresPaw;
    indexPawRightDown = indexPawLeftDown + nbSpheresPaw;
    indexLastPawRightDown = indexPawRightDown + nbSpheresPaw-1;
    indexHead = indexPawRightDown + nbSpheresPaw;
    createBody(nbSpheresBody);
    createTail(nbSpheresTail);
    createNeck(nbSpheresNeck);
    createPaw(true,-70.0,nbSpheresPaw);
    createPaw(true,-110.0,nbSpheresPaw);
    createPaw(false,-70.0,nbSpheresPaw);
    createPaw(false,-110.0,nbSpheresPaw);
}

///////////////////////////////////////////////////////////////////////////////
Dragon::~Dragon() {
    for(int i = 0; i < (int)skeleton.size(); i++){
        Sphere* s = skeleton[i];
        delete s;
    }
}

///////////////////////////////////////////////////////////////////////////////
void Dragon::draw(){
    for(std::vector<Sphere*>::iterator it = skeleton.begin() ; it != skeleton.end(); it++){
        Sphere* s = *it;
        s->draw();
    }
}

/////////////////////////////////////////////////////////////////////
void Dragon::createBody(int nb){
    height = 30*R;
    skeleton.push_back(new Sphere(0,0,height,4*R,1,0,0));
    skeleton.push_back(new Sphere(skeleton[0]->getX()+2*R*nb/2,skeleton[0]->getY(),skeleton[0]->getZ(),4*R,0,0,1));
    skeleton.push_back(new Sphere(skeleton[0]->getX()+2*R*nb,skeleton[0]->getY(),skeleton[0]->getZ(),4*R,1,0,0));
}

////////////////////////////////////////////////////////////////////
void Dragon::createTail(int nb){
    float x0 = skeleton[skeleton.size()-1]->getX();
    float y0 = skeleton[skeleton.size()-1]->getY();
    float z0 = skeleton[skeleton.size()-1]->getZ();
    float r = 4*R;
    float x;
    for (int i = 1; i <= 3; i++) {
        x = x0 + 2*R*i*nb/4;
        r = r - 0.5*R;
        skeleton.push_back(new Sphere(x,
                                      y0,
                                      z0 + (x-x0)*(x-x0)/(80.0*R),
                                      r,0,0,1));
    }
    x = x0 + 2*R*nb;
    r = r - 0.5*R;
    skeleton.push_back(new Sphere(x,
                                  y0,
                                  z0 + (x-x0)*(x-x0)/(80.0*R),
                                  r,1,1,0));
}

/////////////////////////////////////////////////////////////////////
void Dragon::createNeck(int nb){
    float x0 = skeleton[0]->getX()-2*R;
    float y0 = skeleton[0]->getY();
    float z0 = skeleton[0]->getZ();
    float x;
    float r = 4*R;
    for (int i = 1; i <= 2; i++) {
        x = x0 - 2*R*i*nb/3;
        r = r - 0.5*R;
        skeleton.push_back(new Sphere(x,
                                      y0,
                                      z0 + (x-x0)*(x-x0)/(30.0*R),
                                      r,0,0,1));
    }
    x = x0 - 2*R*nb;
    r = r - 0.5*R;
    skeleton.push_back(new Sphere(x,
                                  y0,
                                  z0 + (x-x0)*(x-x0)/(30.0*R),
                                  r,1,1,0));
}

////////////////////////////////////////////////////////////////////////
void Dragon::createPaw(bool up, float angle, int nb){
    float x0,y0,z0;
    if (up) {
        x0 = skeleton[0]->getX();
        y0 = skeleton[0]->getY();
        z0 = skeleton[0]->getZ();
    } else {
        x0 = skeleton[2]->getX();
        y0 = skeleton[2]->getY();
        z0 = skeleton[2]->getZ();
    }
    float dy = 2*R*cos(M_PI/180.0*angle);
    float dz = 2*R*sin(M_PI/180.0*angle);
    skeleton.push_back(new Sphere(x0,
                                  y0+nb/2*dy,
                                  z0+nb/2*dz,
                                  3*R,0,0,1));
    skeleton.push_back(new Sphere(x0,
                                  y0+nb*dy,
                                  z0+nb*dz,
                                  2*R,1,1,0));

}
