#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

#include "window.h"

///////////////////////////////////////////////////////////////////////////////
Window::Window() : QWidget() {

    initGUI();

    //Dragon *d = new Dragon();
    //viewer->addRenderable(d);

    this->skel = NULL;
    this->mesh = new Mesh();
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
    this->saveMesh     = new QPushButton("Save mesh");

    this->showBetween  = new QPushButton("Show inbetween-balls");
    this->sweep        = new QPushButton("Sweeping");
    sweep->setEnabled(false);
    this->stitch       = new QPushButton("Stitching");
    stitch->setEnabled(false);
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
    vl1->addWidget(saveMesh);
    loadSave->setLayout(vl1);

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
    vl->addWidget(stepByStep);
    vl->addWidget(allInOne);

    QHBoxLayout *hl = new QHBoxLayout();
    hl->addLayout(vl);
    hl->addWidget(viewer);

    this->setLayout(hl);


    // Creation on event connections
    QObject::connect(showBetween, SIGNAL(clicked()), this, SLOT(changeText()));
    QObject::connect(loadSkeleton, SIGNAL(clicked()), this, SLOT(load()));
    QObject::connect(saveMesh, SIGNAL(clicked()), this, SLOT(save()));
    QObject::connect(catmullClark, SIGNAL(clicked()), this, SLOT(subdivide()));
    QObject::connect(sweep, SIGNAL(clicked()), this, SLOT(doSweep()));
    QObject::connect(stitch, SIGNAL(clicked()), this, SLOT(doStitch()));
    QObject::connect(evol, SIGNAL(clicked()), this, SLOT(evolve()));
    QObject::connect(fair, SIGNAL(clicked()), this, SLOT(fairing()));
    QObject::connect(goSub, SIGNAL(clicked()), this, SLOT(doAll()));

}

///////////////////////////////////////////////////////////////////////////////
void Window::changeText() {
    if (skel == NULL)
        return;

    if (!skel->betweenBalls()) {
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

    bool ok = this->skel->loadFromFile(fileName.toStdString());
    if (!ok) {
        QMessageBox::critical(this, "Error", "Unable to open file");
        return;
    }

    this->skel->init(*viewer);
    viewer->addRenderable(this->skel);
    showBetween->setText("Show inbetween-balls");
    viewer->update();

    // We can allow the user to click other buttons
    sweep->setEnabled(true);
    stitch->setEnabled(false);
    catmullClark->setEnabled(false);
    evol->setEnabled(false);
    fair->setEnabled(false);
    nbIter->setEnabled(true);
    goSub->setEnabled(true);
}


///////////////////////////////////////////////////////////////////////////////
void Window::doSweep() {
    // TODO: process sweeping

    sweep->setEnabled(false);
    nbIter->setEnabled(false);
    goSub->setEnabled(false);
    stitch->setEnabled(true);
}


///////////////////////////////////////////////////////////////////////////////
void Window::doStitch() {
    // TODO: process stitching

    stitch->setEnabled(false);
    catmullClark->setEnabled(true);
}

///////////////////////////////////////////////////////////////////////////////
void Window::save() {
    QString fileName = QFileDialog::getSaveFileName(this, "Same mesh",
                                       QString(), "Object file (*.obj)");
    QMessageBox::information(this, "Info", "Not yet implemented !");
}


///////////////////////////////////////////////////////////////////////////////
void Window::subdivide() {
    if (!mesh->subdivide())
        QMessageBox::critical(this, "Error", "Subdivision failed !");

    catmullClark->setEnabled(false);
    evol->setEnabled(true);
}


///////////////////////////////////////////////////////////////////////////////
void Window::evolve() {
    // TODO: process mesh evolution

    evol->setEnabled(false);
    fair->setEnabled(true);
}


///////////////////////////////////////////////////////////////////////////////
void Window::fairing() {
    // TODO: process edge fairing

    fair->setEnabled(false);
    sweep->setEnabled(true);
    nbIter->setEnabled(true);
    goSub->setEnabled(true);
}


///////////////////////////////////////////////////////////////////////////////
void Window::doAll() {
    int nbSub = nbIter->value();
}
