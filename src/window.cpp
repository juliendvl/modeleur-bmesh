#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

#include "window.h"
#include "meshevolve.h"
#include "edgefairing.h"

///////////////////////////////////////////////////////////////////////////////
Window::Window() : QWidget() {

    this->skel = new Skeleton();
    this->level = 1;
    this->generateBetweenBalls = true;
    initGUI();

    viewer->setSkeleton(skel);
    viewer->addRenderable(skel);
}


///////////////////////////////////////////////////////////////////////////////
Window::~Window() {
    delete viewer;
}


///////////////////////////////////////////////////////////////////////////////
void Window::initGUI() {
    this->setWindowTitle("Modeleur 3D par Bmesh");

    // Create buttons

    this->loadSkeleton = new QPushButton("Load skeleton");
    this->saveSkel     = new QPushButton("Save skeleton");
    this->saveMeshB    = new QPushButton("Save mesh");
    saveMeshB->setEnabled(false);

    this->sphere1    = new QSpinBox();
    sphere1->setMinimum(0);
    this->sphere2    = new QSpinBox();
    sphere2->setMinimum(0);
    this->submitEdge = new QPushButton("Add edge");

    this->showBetween  = new QPushButton("Show inbetween-balls");
    this->sweep        = new QPushButton("Sweeping");
    this->stitch       = new QPushButton("Stitching");
    this->catmullClark = new QPushButton("Subdivide Mesh");
    catmullClark->setEnabled(false);
    this->evol         = new QPushButton("Evolve mesh");
    evol->setEnabled(false);
    this->fair         = new QPushButton("Edge fairing");
    fair->setEnabled(false);

    this->goSub  = new QPushButton("Subdivide");
    this->nbIter = new QSpinBox(this);
    nbIter->setEnabled(false);
    nbIter->setValue(1);
    nbIter->setMinimum(1);
    goSub->setEnabled(false);


    // Create viewer
    this->viewer = new Viewer();
    viewer->setParent(this);


    // Create group boxes
    this->loadSave   = new QGroupBox("Load/Save");
    QVBoxLayout *vl1 = new QVBoxLayout;
    vl1->addWidget(loadSkeleton);
    vl1->addWidget(saveSkel);
    vl1->addWidget(saveMeshB);
    loadSave->setLayout(vl1);

    this->addEdge    = new QGroupBox("Add edge");
    QVBoxLayout *vl4 = new QVBoxLayout;
    vl4->addWidget(sphere1);
    vl4->addWidget(sphere2);
    vl4->addWidget(submitEdge);
    addEdge->setLayout(vl4);

    this->stepByStep = new QGroupBox("Step by Step");
    QVBoxLayout *vl2 = new QVBoxLayout;
    vl2->addWidget(showBetween);
    vl2->addWidget(sweep);
    vl2->addWidget(stitch);
    vl2->addWidget(catmullClark);
    vl2->addWidget(evol);
    vl2->addWidget(fair);
    stepByStep->setLayout(vl2);

    this->allInOne   = new QGroupBox("All in one");
    QVBoxLayout *vl3 = new QVBoxLayout;
    vl3->addWidget(nbIter);
    vl3->addWidget(goSub);
    allInOne->setLayout(vl3);


    // Create main layout
    QVBoxLayout *vl = new QVBoxLayout();
    vl->addWidget(loadSave);
    vl->addWidget(addEdge);
    vl->addWidget(stepByStep);
    vl->addWidget(allInOne);

    QHBoxLayout *hl = new QHBoxLayout();
    hl->addLayout(vl);
    hl->addWidget(viewer);

    this->setLayout(hl);


    // Creation on event connections
    QObject::connect(showBetween, SIGNAL(clicked()), this, SLOT(changeText()));
    QObject::connect(loadSkeleton, SIGNAL(clicked()), this, SLOT(load()));
    QObject::connect(saveSkel, SIGNAL(clicked()), this, SLOT(saveSkeleton()));
    QObject::connect(submitEdge, SIGNAL(clicked()), this, SLOT(addSkeletonEdge()));
    QObject::connect(saveMeshB, SIGNAL(clicked()), this, SLOT(saveMesh()));
    QObject::connect(catmullClark, SIGNAL(clicked()), this, SLOT(subdivide()));
    QObject::connect(sweep, SIGNAL(clicked()), this, SLOT(doSweep()));
    QObject::connect(stitch, SIGNAL(clicked()), this, SLOT(doStitch()));
    QObject::connect(evol, SIGNAL(clicked()), this, SLOT(evolve()));
    QObject::connect(fair, SIGNAL(clicked()), this, SLOT(fairing()));
    QObject::connect(goSub, SIGNAL(clicked()), this, SLOT(doAll()));

}


///////////////////////////////////////////////////////////////////////////////
void Window::addSkeletonEdge() {
    int id1    = sphere1->value();
    int id2    = sphere2->value();
    int nBalls = skel->getBalls().size();

    if (nBalls == 0 || !sweep->isEnabled())
        return;

    // One ball does not exist
    QString errMess;
    if (id1 >= nBalls) {
        errMess = "Ball " + QString::number(id1) + " does not exist !";
        QMessageBox::critical(this, "Error", errMess);
        return;
    }
    if (id2 >= nBalls) {
        errMess = "Ball " + QString::number(id2) + " does not exist !";
        QMessageBox::critical(this, "Error", errMess);
        return;
    }

    // Ball ids are the same
    if (id1 == id2) {
        errMess = "Ball ids are the same !";
        QMessageBox::critical(this, "Error", errMess);
        return;
    }

    vector< vector<Segment*> > edges = skel->getEdges();

    // Edge exist yet
    if (edges[id1][id2] != NULL || edges[id2][id1] != NULL) {
        errMess = "This edge exists !";
        QMessageBox::critical(this, "Error", errMess);
        return;
    }

    // We can add the edge
    skel->addEdge(new Segment(min(id1, id2), max(id1, id2)));
    viewer->update();
    skel->setNeighbors();
}

///////////////////////////////////////////////////////////////////////////////
void Window::changeText() {
    if (skel == NULL)
        return;

    if (!skel->betweenBalls()) {
        if (generateBetweenBalls)
            skel->interpolation();
        showBetween->setText("Hide inbetween-balls");
        skel->setBetweenBalls(true);
    }
    else {
        showBetween->setText("Show inbetween-balls");
        skel->setBetweenBalls(false);
    }

    viewer->update();
}


///////////////////////////////////////////////////////////////////////////////
void Window::load() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open skeleton file",
                                           QString(), "Text file (*.txt)");

    if (fileName.isEmpty())
        return;

    // If we loaded a skeleton before, we first delete the existing one
    if (this->skel != NULL) {
        viewer->delRendreable(skel);
        delete skel;
    }
    this->skel = new Skeleton();

    // We load the skeleton
    bool ok = this->skel->loadFromFile(fileName.toStdString());
    if (!ok) {
        QMessageBox::critical(this, "Error", "Unable to open file");
        return;
    }

    // We add the new skeleton in the renderable list
    viewer->addRenderable(this->skel);
    viewer->setSkeleton(skel);
    viewer->setMouseTracking(true);
    viewer->update();

    // We change button text
    showBetween->setText("Show inbetween-balls");

    // We can allow the user to click other buttons
    saveMeshB->setEnabled(false);
    sweep->setEnabled(true);
    stitch->setEnabled(true);
    catmullClark->setEnabled(false);
    evol->setEnabled(false);
    fair->setEnabled(false);
    nbIter->setEnabled(true);
    goSub->setEnabled(true);

    this->generateBetweenBalls = true;
}


///////////////////////////////////////////////////////////////////////////////
void Window::doSweep() {
    skel->sweeping();

    saveMeshB->setEnabled(true);
    sweep->setEnabled(false);
    catmullClark->setEnabled(true);
    nbIter->setEnabled(false);
    goSub->setEnabled(false);

    // We stop mouse tracking
    viewer->setMouseTracking(false);

    // We do not need to generate again inbetween-balls
    this->generateBetweenBalls = false;

    viewer->update();
}


///////////////////////////////////////////////////////////////////////////////
void Window::doStitch() {
    // TODO: process stitching
    skel->stitching();

    if (sweep->isEnabled())
        return;

    stitch->setEnabled(false);
    catmullClark->setEnabled(true);

    viewer->update();
}


///////////////////////////////////////////////////////////////////////////////
void Window::saveSkeleton() {
    QString fileName = QFileDialog::getSaveFileName(this, "Same skeleton",
                                       QString(), "Text file (*.txt)");

    if (fileName.isEmpty())
        return;

    if (!skel->save(fileName.toStdString())) {
        QMessageBox::critical(this, "Error", "Unable to save mesh !");
        return;
    }
}



///////////////////////////////////////////////////////////////////////////////
void Window::saveMesh() {
    QString fileName = QFileDialog::getSaveFileName(this, "Same mesh",
                                       QString(), "Object file (*.obj)");

    if (fileName.isEmpty())
        return;

    if (!skel->getMesh().saveMesh(fileName.toStdString())) {
        QMessageBox::critical(this, "Error", "Unable to save mesh !");
        return;
    }
}


///////////////////////////////////////////////////////////////////////////////
void Window::subdivide() {
    if (!skel->getMesh().subdivide())
        QMessageBox::critical(this, "Error", "Subdivision failed !");

    stitch->setEnabled(false);
    catmullClark->setEnabled(false);
    evol->setEnabled(true);

    viewer->update();
}


///////////////////////////////////////////////////////////////////////////////
void Window::evolve() {
    MeshEvolve me(skel->getMesh(), skel, level);
    level++;

    if (!me.evolve())
        QMessageBox::critical(this, "Error", "Evolution failed !");

    evol->setEnabled(false);
    fair->setEnabled(true);

    viewer->update();
}


///////////////////////////////////////////////////////////////////////////////
void Window::fairing() {
    EdgeFairing ef(skel->getMesh());

    if (!ef.fairing())
        QMessageBox::critical(this, "Error", "Fairing failed !");

    fair->setEnabled(false);
    catmullClark->setEnabled(true);

    viewer->update();
}


///////////////////////////////////////////////////////////////////////////////
void Window::doAll() {
    int nbSub = nbIter->value();

    doSweep();

    for (int i = 0; i < nbSub; ++i) {
        subdivide();
        evolve();
        fairing();
    }
}
