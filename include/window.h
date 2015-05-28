#ifndef DEF_WINDOW
#define DEF_WINDOW

#include <QWidget>
#include <QGroupBox>
#include <QPushButton>
#include <QSpinBox>

#include "viewer.h"
#include "skeleton.h"
#include "mesh.h"



class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    ~Window();

public slots:
    void load();
    void save();
    void changeText();
    void doSweep();
    void doStitch();
    void subdivide();
    void evolve();
    void fairing();
    void doAll();

private:
    QGroupBox *loadSave;
    QGroupBox *stepByStep;
    QGroupBox *allInOne;

    QPushButton *loadSkeleton;
    QPushButton *saveMesh;

    QPushButton *showBetween;
    QPushButton *sweep;
    QPushButton *stitch;
    QPushButton *catmullClark;
    QPushButton *evol;
    QPushButton *fair;

    QSpinBox *nbIter;
    QPushButton *goSub;

    Viewer *viewer;

    Skeleton *skel;
    Mesh *mesh;

    void initGUI();

};


#endif
