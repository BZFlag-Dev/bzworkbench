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

#ifndef MESHPYRCONFIGURATIONDIALOG_H_
#define MESHPYRCONFIGURATIONDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Check_Button.H>

#include "ConfigurationDialog.h"
#include "../widgets/QuickLabel.h"
#include "../defines.h"

#include "objects/meshpyr.h"

class MeshpyrConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 300;
	static const int DEFAULT_HEIGHT = 100;

	// constructor
	MeshpyrConfigurationDialog( meshpyr* _theMeshpyr );
	
	// static constructor
	static ConfigurationDialog* init( DataEntry* d ) { 
		meshpyr* c = dynamic_cast< meshpyr* > (d);
		if( c )
			return new MeshpyrConfigurationDialog( dynamic_cast< meshpyr* >( d ) );
		else
			return NULL;
	}
	
	// destructor
	virtual ~MeshpyrConfigurationDialog() { }
	
	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		MeshpyrConfigurationDialog* ccd = (MeshpyrConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}
	
	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		MeshpyrConfigurationDialog* ccd = (MeshpyrConfigurationDialog*)(data);
		ccd->CancelCallback_real( w );
	}
	
private:
	
	// the cone
	cone* theMeshpyr;
	
	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );
	
	// name label
	QuickLabel* texsizeLabel;
	
	// name field
	Fl_Value_Input* texsizeXField;
	Fl_Value_Input* texsizeYField;

	// Flip-z check-button
	Fl_Check_Button* flipzButton;
	
	
};

#endif /*MESHPYRCONFIGURATIONDIALOG_H_*/
