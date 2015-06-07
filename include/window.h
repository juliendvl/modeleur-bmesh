/**
 * @brief Main Window (GUI)
 * */

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
     * @brief Adds an edge in the skeleton
     */
    void addSkeletonEdge();

    /**
     * @brief Changes the button text when the user clicks the Show/Hide inbetween
     *        balls
     */
    void changeText();

    /**
     * @brief Process the mesh generation.
     *        This method processes all steps which are necessary (sweeping,
     *        stitching, subdivision, evolution and fairing)
     */
    void doAll();

    /**
     * @brief Process stitching
     */
    void doStitch();

    /**
     * @brief Process sweeping
     */
    void doSweep();

    /**
     * @brief Process the mesh evolution
     */
    void evolve();

    /**
     * @brief Process edge fairing
     */
    void fairing();

    /**
     * @brief Loads a skeleton (.txt format)
     */
    void load();

    /**
     * @brief Save the mesh generated (.obj format)
     */
    void saveMesh();

    /**
     * @brief Saves a skeleton
     */
    void saveSkeleton();

    /**
     * @brief Process Catmull-Clark subdivision
     */
    void subdivide();


private:
    QGroupBox *loadSave;
    QGroupBox *addEdge;
    QGroupBox *stepByStep;
    QGroupBox *allInOne;

    QPushButton *loadSkeleton;
    QPushButton *saveSkel;
    QPushButton *saveMeshB;

    QSpinBox *sphere1;
    QSpinBox *sphere2;
    QPushButton *submitEdge;

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

    int level;

    bool generateBetweenBalls;

    // Creates buttons, group boxes, layouts...
    void initGUI();

};


#endif
