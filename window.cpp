#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>

#include "window.h"
#include "dragon.h"


///////////////////////////////////////////////////////////////////////////////
Window::Window() : QWidget() {
    this->setWindowTitle("Modeleur 3D par Bmesh");

    this->loadSkeleton = new QPushButton("Load skeleton", this);
    this->showBetween  = new QPushButton("Show inbetween-balls", this);
    this->saveMesh     = new QPushButton("Save mesh", this);

    QVBoxLayout *vl = new QVBoxLayout();
    vl->addWidget(loadSkeleton);
    vl->addWidget(saveMesh);
    vl->addWidget(showBetween);

    this->viewer = new Viewer();
    viewer->setParent(this);

    //Dragon *d = new Dragon();
    //viewer->addRenderable(d);

    QHBoxLayout *hl = new QHBoxLayout();
    hl->addLayout(vl);
    hl->addWidget(viewer);

    this->setLayout(hl);

    QObject::connect(showBetween, SIGNAL(clicked()), this, SLOT(changeText()));
    QObject::connect(loadSkeleton, SIGNAL(clicked()), this, SLOT(load()));
    QObject::connect(saveMesh, SIGNAL(clicked()), this, SLOT(save()));

    this->skel = NULL;
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
}



///////////////////////////////////////////////////////////////////////////////
void Window::save() {
    QString fileName = QFileDialog::getSaveFileName(this, "Same mesh",
                                       QString(), "Object file (*.obj)");
    QMessageBox::information(this, "Info", "Not yet implemented !");
}
