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
    QString text("<h2>Modeleur 3D par Bmesh</h2><br>");
    text += "Projet de specialite de deuxieme annee Ensimag<br>";
    text += "Mai - Juin 2015<br>";
    text += "Adapte de l'article <i>B-Mesh: A Fast Modeling System for Base ";
    text += "Meshes of 3D Articulated Shapes</i><br>";

    return text;
}

