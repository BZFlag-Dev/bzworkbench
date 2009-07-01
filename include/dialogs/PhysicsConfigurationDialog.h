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

#ifndef PHYSICSCONFIGURATIONDIALOG_H_
#define PHYSICSCONFIGURATIONDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Value_Input.H>

#include "ConfigurationDialog.h"
#include "widgets/QuickLabel.h"
#include "widgets/Point3DWidget.h"
#include "defines.h"

#include "objects/physics.h"

#include <list>

class PhysicsConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 300;
	static const int DEFAULT_HEIGHT = 150;

	// constructor
	PhysicsConfigurationDialog( physics* theTexmat );

	// static constructor
	static ConfigurationDialog* init( DataEntry* d ) {
		physics* c = dynamic_cast< physics* > (d);
		if( c )
			return new PhysicsConfigurationDialog( dynamic_cast< physics* >( d ) );
		else
			return NULL;
	}

	// destructor
	virtual ~PhysicsConfigurationDialog() { }

	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		PhysicsConfigurationDialog* ccd = (PhysicsConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}

	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		PhysicsConfigurationDialog* ccd = (PhysicsConfigurationDialog*)(data);
		ccd->CancelCallback_real( w );
	}


private:

	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );

	physics* thePhysics;

	QuickLabel* nameLabel;
	Fl_Input* nameInput;

	QuickLabel* linearLabel;
	Point3DWidget* linearInput;

	QuickLabel* angularLabel;
	Point3DWidget* angularInput;

	QuickLabel* slideLabel;
	Fl_Value_Input* slideInput;

	QuickLabel* deathMessageLabel;
	Fl_Input* deathMessageInput;
};

#endif /* PHYSICSCONFIGURATIONDIALOG_H_ */
