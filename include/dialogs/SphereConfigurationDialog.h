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

#ifndef SPHERECONFIGURATIONDIALOG_H_
#define SPHERECONFIGURATIONDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Check_Button.H>

#include "ConfigurationDialog.h"
#include "../widgets/QuickLabel.h"
#include "../defines.h"

#include "objects/sphere.h"

class SphereConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 300;
	static const int DEFAULT_HEIGHT = 175;

	// constructor
	SphereConfigurationDialog( sphere* theSphere );
	
	// static constructor
	static ConfigurationDialog* init( DataEntry* d ) { 
		sphere* c = dynamic_cast< sphere* > (d);
		if( c )
			return new SphereConfigurationDialog( dynamic_cast< sphere* >( d ) );
		else
			return NULL;
	}
	
	// destructor
	virtual ~SphereConfigurationDialog() { }
	
	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		SphereConfigurationDialog* ccd = (SphereConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}
	
	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		SphereConfigurationDialog* ccd = (SphereConfigurationDialog*)(data);
		ccd->CancelCallback_real( w );
	}
	
private:
	
	// the cone
	sphere* theSphere;
	
	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );
	
	// name label
	QuickLabel* texsizeLabel;
	
	// name field
	Fl_Value_Input* texsizeXField;
	Fl_Value_Input* texsizeYField;
	
	// subdivision label
	QuickLabel* subdivisionLabel;
	
	// subdivision counter
	Fl_Counter* subdivisionCounter;
	
	// flat shading check-button
	Fl_Check_Button* flatShadingButton;
	
	// smooth bounce check-button
	Fl_Check_Button* smoothBounceButton;

	// Flip-z check-button
	Fl_Check_Button* hemisphereButton;
	
	
};

#endif /*SPHERECONFIGURATIONDIALOG_H_*/
