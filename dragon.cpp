#include "dragon.h"
#include "cylinder.h"
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
    skel = new Skeleton();
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
    skel->interpolation();
}

///////////////////////////////////////////////////////////////////////////////
Dragon::~Dragon() {
    for(std::vector<Sphere*>::iterator it = skel->getBalls().begin() ; it != skel->getBalls().end(); it++){
        Sphere* s = *it;
        delete s;
    }
    for(std::vector<Segment*>::iterator it = skel->getEdges().begin() ; it != skel->getEdges().end(); it++){
        Segment* sg = *it;
        delete sg;
    }
    for(std::vector<Sphere*>::iterator it = skel->getInBetweenBalls().begin() ; it != skel->getInBetweenBalls().end(); it++){
        Sphere* s = *it;
        delete s;
    }
}

///////////////////////////////////////////////////////////////////////////////
void Dragon::draw(){
    std::vector<Sphere*>& balls = skel->getBalls();
    for(std::vector<Sphere*>::iterator it = balls.begin() ; it != balls.end(); it++){
        Sphere* s = *it;
        s->draw();
    }
    for(std::vector<Segment*>::iterator it = skel->getEdges().begin() ; it != skel->getEdges().end(); it++){
        Segment* sg = *it;
        qglviewer::Vec pos1(balls[sg->getIndex1()]->getX(), balls[sg->getIndex1()]->getY(), balls[sg->getIndex1()]->getZ());
        qglviewer::Vec pos2(balls[sg->getIndex2()]->getX(), balls[sg->getIndex2()]->getY(), balls[sg->getIndex2()]->getZ());
        Cylinder c(pos1,pos2,R/2.0);
        c.draw();
    }
    for(std::vector<Sphere*>::iterator it = skel->getInBetweenBalls().begin() ; it != skel->getInBetweenBalls().end(); it++){
        Sphere* s = *it;
        s->draw();
    }
}

/////////////////////////////////////////////////////////////////////
void Dragon::createBody(int nb){
    height = 30*R;
    skel->getBalls().push_back(new Sphere(0,0,height,4*R,1,0,0));
    skel->getBalls().push_back(new Sphere(skel->getBalls()[0]->getX()+2*R*nb/2,skel->getBalls()[0]->getY(),skel->getBalls()[0]->getZ(),4*R,0,0,1));
    skel->getEdges().push_back(new Segment(0,1));
    skel->getBalls().push_back(new Sphere(skel->getBalls()[0]->getX()+2*R*nb,skel->getBalls()[0]->getY(),skel->getBalls()[0]->getZ(),4*R,1,0,0));
    skel->getEdges().push_back(new Segment(1,2));
}

////////////////////////////////////////////////////////////////////
void Dragon::createTail(int nb){
    int i0 = skel->getBalls().size()-1;
    float x0 = skel->getBalls()[i0]->getX();
    float y0 = skel->getBalls()[i0]->getY();
    float z0 = skel->getBalls()[i0]->getZ();
    float r = 4*R;
    float x;
    for (int i = 1; i <= 3; i++) {
        x = x0 + 2*R*i*nb/4;
        r = r - 0.5*R;
        skel->getBalls().push_back(new Sphere(x,
                                              y0,
                                              z0 + (x-x0)*(x-x0)/(80.0*R),
                                              r,0,0,1));
        skel->getEdges().push_back(new Segment(i0+i-1,i0+i));
    }
    x = x0 + 2*R*nb;
    r = r - 0.5*R;
    skel->getBalls().push_back(new Sphere(x,
                                          y0,
                                          z0 + (x-x0)*(x-x0)/(80.0*R),
                                          r,1,1,0));
    skel->getEdges().push_back(new Segment(skel->getBalls().size()-2,skel->getBalls().size()-1));
}

/////////////////////////////////////////////////////////////////////
void Dragon::createNeck(int nb){
    float x0 = skel->getBalls()[0]->getX()-2*R;
    float y0 = skel->getBalls()[0]->getY();
    float z0 = skel->getBalls()[0]->getZ();
    float x;
    float r = 4*R;
    int i0 = skel->getBalls().size()-1;
    for (int i = 1; i <= 2; i++) {
        x = x0 - 2*R*i*nb/3;
        r = r - 0.5*R;
        skel->getBalls().push_back(new Sphere(x,
                                              y0,
                                              z0 + (x-x0)*(x-x0)/(30.0*R),
                                              r,0,0,1));
        if (i == 1) {
            skel->getEdges().push_back(new Segment(0,i0+1));
        } else {
            skel->getEdges().push_back(new Segment(i0+i-1,i0+i));
        }
    }
    x = x0 - 2*R*nb;
    r = r - 0.5*R;
    skel->getBalls().push_back(new Sphere(x,
                                          y0,
                                          z0 + (x-x0)*(x-x0)/(30.0*R),
                                          r,1,1,0));
    skel->getEdges().push_back(new Segment(skel->getBalls().size()-2,skel->getBalls().size()-1));
}

////////////////////////////////////////////////////////////////////////
void Dragon::createPaw(bool up, float angle, int nb){
    float x0,y0,z0;
    if (up) {
        x0 = skel->getBalls()[0]->getX();
        y0 = skel->getBalls()[0]->getY();
        z0 = skel->getBalls()[0]->getZ();
    } else {
        x0 = skel->getBalls()[2]->getX();
        y0 = skel->getBalls()[2]->getY();
        z0 = skel->getBalls()[2]->getZ();
    }
    float dy = 2*R*cos(M_PI/180.0*angle);
    float dz = 2*R*sin(M_PI/180.0*angle);
    skel->getBalls().push_back(new Sphere(x0,
                                          y0+nb/2*dy,
                                          z0+nb/2*dz,
                                          3*R,0,0,1));
    if (up) {
        skel->getEdges().push_back(new Segment(0,skel->getBalls().size()-1));
    } else {
        skel->getEdges().push_back(new Segment(2,skel->getBalls().size()-1));
    }
    skel->getBalls().push_back(new Sphere(x0,
                                          y0+nb*dy,
                                          z0+nb*dz,
                                          2*R,1,1,0));
    skel->getEdges().push_back(new Segment(skel->getBalls().size()-2,skel->getBalls().size()-1));

}
