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

#ifndef ARCCONFIGURATIONDIALOG_H_
#define ARCCONFIGURATIONDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Counter.H>
#include <FL/Fl_Check_Button.H>

#include "ConfigurationDialog.h"
#include "../widgets/QuickLabel.h"
#include "../defines.h"

#include "objects/arc.h"

class ArcConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 400;
	static const int DEFAULT_HEIGHT = 175;

	// constructor
	ArcConfigurationDialog( arc* theArc );
	
	// static constructor
	static ConfigurationDialog* init( DataEntry* d ) { 
		arc* c = dynamic_cast< arc* > (d);
		if( c )
			return new ArcConfigurationDialog( dynamic_cast< arc* >( d ) );
		else
			return NULL;
	}
	
	// destructor
	virtual ~ArcConfigurationDialog() { }
	
	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		ArcConfigurationDialog* ccd = (ArcConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}
	
	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		ArcConfigurationDialog* ccd = (ArcConfigurationDialog*)(data);
		ccd->CancelCallback_real( w );
	}
	
private:
	
	// the cone
	arc* theArc;
	
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
	
	// subdivision label
	QuickLabel* subdivisionLabel;
	
	// subdivision counter
	Fl_Counter* subdivisionCounter;
	
	// angle sweep label
	QuickLabel* sweepAngleLabel;
	
	// angle sweep counter
	Fl_Input* sweepAngle;
	static void sweepAngle_cb(Fl_Widget *o, void* data);

	// ratio label
	QuickLabel* ratioLabel;
	
	// ratio counter
	Fl_Input* ratio;
	static void ratio_cb(Fl_Widget *o, void* data);
	
	// flat shading check-button
	Fl_Check_Button* flatShadingButton;
	
	// smooth bounce check-button
	Fl_Check_Button* smoothBounceButton;
	
};

#endif /*ARCCONFIGURATIONDIALOG_H_*/
