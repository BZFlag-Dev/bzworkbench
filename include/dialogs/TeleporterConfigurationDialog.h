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

#ifndef TELEPORTERCONFIGURATIONDIALOG_H_
#define TELEPORTERCONFIGURATIONDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Check_Button.H>

#include "ConfigurationDialog.h"
#include "../widgets/QuickLabel.h"
#include "../defines.h"

#include "objects/teleporter.h"

class TeleporterConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 300;
	static const int DEFAULT_HEIGHT = 175;

	// constructor
	TeleporterConfigurationDialog( teleporter* theTele );
	
	// static constructor
	static ConfigurationDialog* init( DataEntry* d ) { 
		teleporter* c = dynamic_cast< teleporter* > (d);
		if( c )
			return new TeleporterConfigurationDialog( dynamic_cast< teleporter* >( d ) );
		else
			return NULL;
	}
	
	// destructor
	virtual ~TeleporterConfigurationDialog() { }
	
	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		TeleporterConfigurationDialog* ccd = (TeleporterConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}
	
	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		TeleporterConfigurationDialog* ccd = (TeleporterConfigurationDialog*)(data);
		ccd->CancelCallback_real( w );
	}
	
private:
	
	// the cone
	teleporter* theTele;
	
	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );
	
	// name label
	QuickLabel* texsizeLabel;
	
	// name field
	Fl_Value_Input* texsizeField;
	
	// border label
	QuickLabel* borderLabel;

	// border field
	Fl_Value_Input* borderField;
	

	
	
};

#endif /*TELEPORTERCONFIGURATIONDIALOG_H_*/
