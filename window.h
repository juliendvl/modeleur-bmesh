#ifndef DEF_WINDOW
#define DEF_WINDOW

#include <QWidget>
#include <QPushButton>
#include "viewer.h"
#include "skeleton.h"

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

public slots:
    void changeText();
    void load();
    void save();

private:
    QPushButton *loadSkeleton;
    QPushButton *showBetween;
    QPushButton *saveMesh;
    Viewer *viewer;

    Skeleton *skel;


};


#endif
