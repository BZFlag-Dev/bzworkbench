/* BZWorkbench
 * Copyright (c) 1993 - 2008 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef PHYSICSEDITOR_H_
#define PHYSICSEDITOR_H_

#include "Fl_Dialog.h"
#include "FL/Fl_Multi_Browser.H"
#include "FL/Fl_Button.H"

#include "model/Model.h"
#include "widgets/QuickLabel.h"

class PhysicsEditor : public Fl_Dialog {

public:

	// constructor
	PhysicsEditor( Model* model );

	// destructor
	virtual ~PhysicsEditor() { }

	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		PhysicsEditor* ccd = (PhysicsEditor*)(data);
		ccd->OKCallback_real( w );
	}

	static void PhysicsAddCallback( Fl_Widget* w, void* data ) {
		PhysicsEditor* ccd = (PhysicsEditor*)(data);
		ccd->PhysicsAddCallback_real( w );
	}

	static void PhysicsRemoveCallback( Fl_Widget* w, void* data ) {
		PhysicsEditor* ccd = (PhysicsEditor*)(data);
		ccd->PhysicsRemoveCallback_real( w );
	}

	static void PhysicsEditCallback( Fl_Widget* w, void* data ) {
		PhysicsEditor* ccd = (PhysicsEditor*)(data);
		ccd->PhysicsEditCallback_real( w );
	}

protected:
	Model* model;

	// material label
	QuickLabel* physicsLabel;

	// material browser
	Fl_Multi_Browser* physicsBrowser;

	// material buttons
	Fl_Button* physicsAddButton;
	Fl_Button* physicsRemoveButton;
	Fl_Button* physicsEditButton;

private:
	void refreshPhysicsList();

	// real callbacks
	void OKCallback_real( Fl_Widget* w );

	void PhysicsAddCallback_real( Fl_Widget* w );
	void PhysicsRemoveCallback_real( Fl_Widget* w );
	void PhysicsEditCallback_real( Fl_Widget* w );
};

#endif /* PHYSICSEDITOR_H_ */
