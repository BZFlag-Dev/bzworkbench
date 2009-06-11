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

#ifndef BOXCONFIGURATIONDIALOG_H_
#define BOXCONFIGURATIONDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Choice.H>

#include "ConfigurationDialog.h"
#include "widgets/QuickLabel.h"
#include "defines.h"

#include "objects/box.h"

class BoxConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 300;
	static const int DEFAULT_HEIGHT = 175;

	// constructor
	BoxConfigurationDialog( ::box* theBox );

	// static constructor
	static ConfigurationDialog* init( DataEntry* d ) {
		::box* c = dynamic_cast< ::box* > (d);
		if( c )
			return new BoxConfigurationDialog( dynamic_cast< ::box* >( d ) );
		else
			return NULL;
	}

	// destructor
	virtual ~BoxConfigurationDialog() { }

	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		BoxConfigurationDialog* ccd = (BoxConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}

	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		BoxConfigurationDialog* ccd = (BoxConfigurationDialog*)(data);
		ccd->CancelCallback_real( w );
	}

	// face callbacks
	static void XPCallback( Fl_Widget* w, void* data ) {
		BoxConfigurationDialog* ccd = (BoxConfigurationDialog*)(data);
		ccd->XPCallback_real( w );
	}

	static void XNCallback( Fl_Widget* w, void* data ) {
		BoxConfigurationDialog* ccd = (BoxConfigurationDialog*)(data);
		ccd->XNCallback_real( w );
	}

	static void YPCallback( Fl_Widget* w, void* data ) {
		BoxConfigurationDialog* ccd = (BoxConfigurationDialog*)(data);
		ccd->YPCallback_real( w );
	}

	static void YNCallback( Fl_Widget* w, void* data ) {
		BoxConfigurationDialog* ccd = (BoxConfigurationDialog*)(data);
		ccd->YNCallback_real( w );
	}

	static void ZPCallback( Fl_Widget* w, void* data ) {
		BoxConfigurationDialog* ccd = (BoxConfigurationDialog*)(data);
		ccd->ZPCallback_real( w );
	}

	static void ZNCallback( Fl_Widget* w, void* data ) {
		BoxConfigurationDialog* ccd = (BoxConfigurationDialog*)(data);
		ccd->ZNCallback_real( w );
	}

private:

	// the cone
	::box* theBox;

	// set the selected face
	void setFace( int face );
	void saveFace( int face );

	// values
	double texsizeX[6];
	double texsizeY[6];
	double texoffsetX[6];
	double texoffsetY[6];
	bool drivethrough[6];
	bool shootthrough[6];
	bool ricochet[6];
	int selectedFace;


	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );
	void XPCallback_real( Fl_Widget* w );
	void XNCallback_real( Fl_Widget* w );
	void YPCallback_real( Fl_Widget* w );
	void YNCallback_real( Fl_Widget* w );
	void ZPCallback_real( Fl_Widget* w );
	void ZNCallback_real( Fl_Widget* w );

	// box
	Fl_Box* flbox;

	// face label
	QuickLabel* faceLabel;

	// face choice
	Fl_Choice* faceChoice;

	// texsize label
	QuickLabel* texsizeLabel;

	// texsize value inputs
	Fl_Value_Input* texsizeXInput;
	Fl_Value_Input* texsizeYInput;

	// texoffset label
	QuickLabel* texoffsetLabel;

	// texoffset value inputs
	Fl_Value_Input* texoffsetXInput;
	Fl_Value_Input* texoffsetYInput;

	// Drive through check-button
	Fl_Check_Button* drivethroughButton;

	// Shoot through check-button
	Fl_Check_Button* shootthroughButton;

	// ricochet check-button
	Fl_Check_Button* ricochetButton;
};

#endif /*CONECONFIGURATIONDIALOG_H_*/
