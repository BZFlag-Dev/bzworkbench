/* BZWorkbench
 * Copyright (c) 1993 - 2010 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef SELECTHANDLER_H_
#define SELECTHANDLER_H_

// forward declarations
class Renderable;
class View;

#include "BZEventHandler.h"

#include <osgViewer/Viewer>
#include <osgGA/MatrixManipulator>

#include <osg/NodeVisitor>
#include <osg/Matrix>

#include <string>
#include <map>

class selectHandler : public BZEventHandler {

public:

	// constructor
	selectHandler( View* view, osgGA::MatrixManipulator* baseManipulator );
	~selectHandler() {}

	// get the name
	static std::string getName() { return std::string("selectHandler"); }

	// handle an event
	bool handle( const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa );

	// pick out a Renderable
    bool pickObject(View* viewer, const osgGA::GUIEventAdapter& ea);

    // pick the selector
    bool pickSelector( View* viewer, const osgGA::GUIEventAdapter& ea);

    // configure an object
    bool configureObject( View* viewer, const osgGA::GUIEventAdapter& ea);

    // drag the selector
    bool dragSelector( View* viewer, const osgGA::GUIEventAdapter& ea);

    // rotate (i.e. spin) the selector (i.e. if the appropriate key is pressed)
    bool rotateSelector( View* viewer, const osgGA::GUIEventAdapter& ea);

    // scale the selector (i.e. if the appropriate key is pressed)
    bool scaleSelector( View* viewer, const osgGA::GUIEventAdapter& ea);

    // shear the selector (i.e. if the appropriate key is pressed)
    bool shearSelector( View* viewer, const osgGA::GUIEventAdapter& ea);

    // shift the selector (i.e. if the appropriate key is pressed)
    bool shiftSelector( View* viewer, const osgGA::GUIEventAdapter& ea);

	// sets the lastSelected variable to NULL (should be called if an bz2object is deleted)
	void clearLastSelected();

private:
    // the last Renderable to be selected
    Renderable* lastSelected;

    // data associated with the last renderable
    void* lastSelectedData;

    // difference in x and y in mouse position (normalized)
    double dx, dy;

    // previous x and y values
    double prev_x, prev_y;

    // previous event type
    unsigned int prevEvent;

    // the base camera manipulator (needed for extracting camera orientation)
    osgGA::MatrixManipulator* cameraManipulator;

	// variables for snapping
	osg::Vec3 translateSnap;
	osg::Vec3 scaleSnap;
	float rotateSnap;
};

#endif /*SELECTHANDLER_H_*/
