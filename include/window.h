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
    /**
     * @brief Constructor
     */
    Window();

    ~Window();

public slots:
    /**
     * @brief Loads a skeleton (.txt format)
     */
    void load();

    /**
     * @brief Save the mesh generated (.obj format)
     */
    void save();

    /**
     * @brief Changes the button text when the user clicks the Show/Hide inbetween
     *        balls
     */
    void changeText();

    /**
     * @brief Process sweeping
     */
    void doSweep();

    /**
     * @brief Process stitching
     */
    void doStitch();

    /**
     * @brief Process Catmull-Clark subdivision
     */
    void subdivide();

    /**
     * @brief Process the mesh evolution
     */
    void evolve();

    /**
     * @brief Process edge fairing
     */
    void fairing();

    /**
     * @brief Process the mesh generation.
     *        This method processes all steps which are necessary (sweeping,
     *        stitching, subdivision, evolution and fairing)
     */
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

    void initGUI();

};


#endif
