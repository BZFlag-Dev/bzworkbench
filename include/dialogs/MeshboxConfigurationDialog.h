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

#ifndef MESHBOXCONFIGURATIONDIALOG_H_
#define MESHBOXCONFIGURATIONDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Check_Button.H>

#include "ConfigurationDialog.h"
#include "../widgets/QuickLabel.h"
#include "../defines.h"

#include "objects/meshbox.h"

class MeshboxConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 400;
	static const int DEFAULT_HEIGHT = 75;

	// constructor
	MeshboxConfigurationDialog( meshbox* theMeshbox );
	
	// static constructor
	static ConfigurationDialog* init( DataEntry* d ) { 
		meshbox* c = dynamic_cast< meshbox* > (d);
		if( c )
			return new MeshboxConfigurationDialog( dynamic_cast< meshbox* >( d ) );
		else
			return NULL;
	}
	
	// destructor
	virtual ~MeshboxConfigurationDialog() { }
	
	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		MeshboxConfigurationDialog* ccd = (MeshboxConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}
	
	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		MeshboxConfigurationDialog* ccd = (MeshboxConfigurationDialog*)(data);
		ccd->CancelCallback_real( w );
	}
	
private:
	
	// the cone
	meshbox* theMeshbox;
	
	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );
	
	// name label
	QuickLabel* texsizeLabel;
	
	// name field
	Fl_Value_Input* texsizeXField;
	Fl_Value_Input* texsizeYField;
	Fl_Value_Input* texsizeZField;
	Fl_Value_Input* texsizeWField;
	
};

#endif /*MESHBOXCONFIGURATIONDIALOG_H_*/
