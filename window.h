#ifndef DEF_WINDOW
#define DEF_WINDOW

#include <QWidget>
#include <QPushButton>
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
    void changeText();
    void load();
    void save();
    void subdivide();

private:
    QPushButton *loadSkeleton;
    QPushButton *showBetween;
    QPushButton *saveMesh;
    QPushButton *catmullClark;
    Viewer *viewer;

    Skeleton *skel;
    Mesh *mesh;

    void initGUI();

};


#endif
