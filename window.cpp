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

    QVBoxLayout *vl = new QVBoxLayout();
    vl->addWidget(loadSkeleton);
    vl->addWidget(showBetween);

    this->viewer = new Viewer();
    viewer->setParent(this);

    Dragon *d = new Dragon();
    viewer->addRenderable(d);

    QHBoxLayout *hl = new QHBoxLayout();
    hl->addLayout(vl);
    hl->addWidget(viewer);

    this->setLayout(hl);

    QObject::connect(showBetween, SIGNAL(clicked()), this, SLOT(changeText()));
    QObject::connect(loadSkeleton, SIGNAL(clicked()), this, SLOT(load()));

    this->skel = new Skeleton();
}



///////////////////////////////////////////////////////////////////////////////
void Window::changeText() {
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
    if (this->skel != NULL)
        delete skel;

    this->skel = new Skeleton();

    bool ok = this->skel->loadFromFile(fileName.toStdString());
    if (!ok) {
        QMessageBox::critical(this, "Error", "Unable to open file");
        return;
    }

    std::cout << *skel << std::endl;
}
