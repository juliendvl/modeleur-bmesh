#ifndef DRAGON_H
#define DRAGON_H

#include "renderable.h"
#include "sphere.h"
#include "skeleton.h"

class Dragon : public Renderable
{
public:
    Dragon();
    ~Dragon();
    void draw();

private:
    void createBody(int nb);
    void createTail(int nb);
    void createNeck(int nb);
    void createPaw(bool up, float angle, int nb);

    Skeleton* skel;
    int nbSpheresContourBody;
    int nbSpheresContourTail;
    int nbSpheresContourNeck;
    int nbSpheresContourPaw;
    int nbSpheresBody;
    int nbSpheresTail;
    int nbSpheresNeck;
    int nbSpheresPaw;
    int indexBody;
    int indexTail;
    int indexNeck;
    int indexPawLeftUp;
    int indexPawRightUp;
    int indexPawLeftDown;
    int indexPawRightDown;
    int indexLastPawRightDown;
    int indexHead;
    int indexJawUp;
    int indexJawDown;
    int indexEyeLeft;
    int indexEyeRight;
    int indexNoseLeft;
    int indexNoseRight;
    float R;
    float height;
};

#endif // DRAGON_H
