#include <QKeyEvent>

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include "viewer.h"
#include "renderable.h"

using namespace qglviewer;


///////////////////////////////////////////////////////////////////////////////
Viewer::Viewer() {}


///////////////////////////////////////////////////////////////////////////////
Viewer::~Viewer() {
	list<Renderable *>::iterator it;
	for (it = renderableList.begin(); it != renderableList.end(); ++it) {
 		delete(*it);
	}
	renderableList.clear();
}


///////////////////////////////////////////////////////////////////////////////
void Viewer::addRenderable(Renderable *r) {
        renderableList.push_back(r);
}


///////////////////////////////////////////////////////////////////////////////
void Viewer::delRendreable(Renderable *r) {
    renderableList.remove(r);
}


///////////////////////////////////////////////////////////////////////////////
void Viewer::setSkeleton(Skeleton *s) {
    this->s = s;
}


///////////////////////////////////////////////////////////////////////////////
void Viewer::init() {
    // glut initialisation (mandatory)
    int dum = 0;
  	glutInit(&dum, NULL);

	//=== VIEWING PARAMETERS
	restoreStateFromFile();   // Restore previous viewer state.

	toogleWireframe = false;  // filled faces
	toogleLight = true;       // light on
	help();                   // display help

	if (toogleLight == true)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
	
    setSceneRadius(10.0f);
    setMouseTracking(true);

	list<Renderable *>::iterator it;
	for (it = renderableList.begin(); it != renderableList.end(); ++it) {
	        (*it)->init(*this);
	}

    glClearColor(255, 255, 255, 1);
}



///////////////////////////////////////////////////////////////////////////////
void Viewer::draw() {
	// draw every objects in renderableList
	list<Renderable *>::iterator it;
	for(it = renderableList.begin(); it != renderableList.end(); ++it) {
		(*it)->draw();
	}

    if (s->getMesh().getMesh().n_vertices() != 0)
        return;

    startScreenCoordinatesSystem();
    glDisable(GL_LIGHTING);
    glColor3f(0.0, 0.0, 0.0);

    vector<Sphere*> balls = s->getBalls();
    for (unsigned int i = 0; i < balls.size(); ++i) {
        Sphere *s = balls[i];
        Vec spos = camera()->projectedCoordinatesOf(s->position());
        float r = s->getRadius();
        drawText(spos[0] + r + 2, spos[1] + r + 2, QString::number(i),
                 QFont("Liberation Sans", 14));
    }

    glEnable(GL_LIGHTING);
    stopScreenCoordinatesSystem();
}


///////////////////////////////////////////////////////////////////////////////
void Viewer::mousePressEvent(QMouseEvent *e) {
    Qt::KeyboardModifiers modifiers = e->modifiers();

    if (modifiers == Qt::ShiftModifier) {
        s->addBall(new Sphere(5.0, 5.0, 5.0, 1));

        // We update edge matrix
        vector< vector<Segment*> > &edges = s->getEdges();
        edges.push_back(vector<Segment*>(s->getBalls().size()));
        for (unsigned int i = 0; i < edges.size() - 1; ++i)
            edges[i].push_back(NULL);
    }

    QGLViewer::mousePressEvent(e);
}


///////////////////////////////////////////////////////////////////////////////
void Viewer::mouseMoveEvent(QMouseEvent *e) {
	// all renderables may respond to key events
	list<Renderable *>::iterator it;
	for(it = renderableList.begin(); it != renderableList.end(); ++it) {
	  (*it)->mouseMoveEvent(e, *this);
	}
	
	// default QGLViewer behaviour
	QGLViewer::mouseMoveEvent(e);
	updateGL();
}


///////////////////////////////////////////////////////////////////////////////
void Viewer::keyPressEvent(QKeyEvent *e) {
	// Get event modifiers key
	const Qt::KeyboardModifiers modifiers = e->modifiers();

	// all renderables may respond to key events
	list<Renderable *>::iterator it;
	for(it = renderableList.begin(); it != renderableList.end(); ++it) {
	  (*it)->keyPressEvent(e, *this);
	}

	if ((e->key()==Qt::Key_W) && (modifiers==Qt::NoButton)) {
	// events with modifiers: CTRL+W, ALT+W, ... to handle separately
		toogleWireframe = !toogleWireframe;
		if (toogleWireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	} else if ((e->key()==Qt::Key_L) && (modifiers==Qt::NoButton)) {
		toogleLight = !toogleLight;
		if (toogleLight == true)
			glEnable(GL_LIGHTING);
		else
			glDisable(GL_LIGHTING);       
	// ... and so on with all events to handle here!

	} else {
		// if the event is not handled here, process it as default
		QGLViewer::keyPressEvent(e);
	}
	updateGL();
}



///////////////////////////////////////////////////////////////////////////////
QString Viewer::helpString() const {
    QString text("<h1>Modeleur 3d par B-Mesh</h1><br>");
    text += "Julien Daval / Omid Ghorreshi<br/>";
    text += "Projet de spécialité de deuxième année Ensimag<br/>";
    text += "Mai/Juin 2015";

    text += "<h2>Principe</h2>";
    text += "<p>Ce programme permet de générer le maillage d'une forme 3D ";
    text += "uniquement en spécifiant des sphères clés et les liens entre ";
    text += "ces spères clés</p>";

    text += "<h2>Fonctionnement</h2>";

    text += "Le programme propose un certain nombre d'intéractions:";
    text += "<ul>";
    text += "<li><b>Load Skeleton: </b>Charge un squelette (au format .txt);</li>";
    text += "<li><b>Save Mesh: </b>Sauvegarde le maillage généré (au format .obj);</li>";
    text += "<li><b>Step by Step: </b>Permet de réaliser les différentes étapes de \
             l'algorithme une par une;</li>";
    text += "<li><b>All in One: </b>Générer tout d'un seul coup. On peut régler le \
             nombre de subdivisions.</li>";
    text += "</ul>";

    text += "Les différents raccourcis disponibles sont fournis dans les ";
    text += "<b>Keyboard</b> et <b>Mouse</b> de cette aide.";

    text += "<h2>Comment créer un squelette ?</h2>";
    text += "<ul>";
    text += "<li><b>Ajouter une sphère: </b>Shift + clic</li>";
    text += "<li><b>Déplacer une sphère: </b>placer le curseur près du centre \
             de la sphère et clic droit</li>";
    text += "<li><b>Augmenter/Diminuer le rayon: </b>Shift + molette près du \
             centre de la sphère</b></li>";
    text += "<li><b>Ajouter une arete: </b>Utiliser le menu <b>Add edge</b></li>";
    text += "</ul>";

    return text;
}

