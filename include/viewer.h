#ifndef DEF_VIEWER
#define DEF_VIEWER

#include <QGLViewer/qglviewer.h>
#include <list>
#include "skeleton.h"

using namespace std;

class Renderable;


class Viewer : public QGLViewer
{

public :
    /**
     * @brief Constructor
     */
	Viewer();
	virtual ~Viewer();

    /**
     * @brief Adds a renderable object in the drawable list
     * @param r object to add
     */
	void addRenderable(Renderable *r);

    /**
     * @brief Deletes a renderable object in the drawable list
     * @param r object to delete
     */
    void delRendreable(Renderable *r);

    /**
     * @brief Updates the skeleton linked with the viewer
     * @param s skeleton
     */
    void setSkeleton(Skeleton *s);

/* Scene methods */
protected :
	/// List of the scene objects, to render, animate, ...
	list<Renderable *> renderableList;
		
	/// Create the scene and initializes rendering parameters
	virtual void init();
		
	/// Draw every objects of the scene
	virtual void draw();


    /* Viewing parameters */
	bool toogleWireframe;
	bool toogleLight;

	/// Handle keyboard events specifically
	virtual void keyPressEvent(QKeyEvent *e);

    /// Handle mouse events specifically
	virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void mousePressEvent(QMouseEvent *e);

    /// Draw every objects of the scene
    virtual QString helpString() const;


private:
    Skeleton *s;

};

#endif
