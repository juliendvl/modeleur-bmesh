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

private:
    QPushButton *loadSkeleton;
    QPushButton *showBetween;
    Viewer *viewer;

    Skeleton *skel;


};


#endif
