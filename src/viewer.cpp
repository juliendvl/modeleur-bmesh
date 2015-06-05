#include <QKeyEvent>

#ifndef __APPLE__
#include <GL/glut.h>
#else
#include <GLUT/glut.h>
#endif

#include "viewer.h"
#include "renderable.h"

Viewer::Viewer() {
}

Viewer::~Viewer()
{
	list<Renderable *>::iterator it;
	for (it = renderableList.begin(); it != renderableList.end(); ++it) {
 		delete(*it);
	}
	renderableList.clear();
}

void Viewer::addRenderable(Renderable *r)
{
        renderableList.push_back(r);
}

void Viewer::delRendreable(Renderable *r) {
    renderableList.remove(r);
}

void Viewer::init()
{
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

	list<Renderable *>::iterator it;
	for (it = renderableList.begin(); it != renderableList.end(); ++it) {
	        (*it)->init(*this);
	}

    glClearColor(255, 255, 255, 1);
}


void Viewer::draw()
{  
	// draw every objects in renderableList
	list<Renderable *>::iterator it;
	for(it = renderableList.begin(); it != renderableList.end(); ++it) {
		(*it)->draw();
	}
}


void Viewer::animate()
{
	// animate every objects in renderableList
	list<Renderable *>::iterator it;
	for(it = renderableList.begin(); it != renderableList.end(); ++it) {
		(*it)->animate();
	}
	
	// this code might change if some rendered objets (stored as
	// attributes) need to be specifically updated with common
	// attributes, like real CPU time (?)
}


void Viewer::mouseMoveEvent(QMouseEvent *e)
{
	// all renderables may respond to key events
	list<Renderable *>::iterator it;
	for(it = renderableList.begin(); it != renderableList.end(); ++it) {
	  (*it)->mouseMoveEvent(e, *this);
	}
	
	// default QGLViewer behaviour
	QGLViewer::mouseMoveEvent(e);
	updateGL();
}

void Viewer::keyPressEvent(QKeyEvent *e)
{
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


QString Viewer::helpString() const
{
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

    return text;
}

