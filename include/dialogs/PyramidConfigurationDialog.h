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

#ifndef PYRAMIDCONFIGURATIONDIALOG_H_
#define PYRAMIDCONFIGURATIONDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Choice.H>

#include "ConfigurationDialog.h"
#include "widgets/QuickLabel.h"
#include "defines.h"

#include "objects/pyramid.h"

class PyramidConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 300;
	static const int DEFAULT_HEIGHT = 215;

	// constructor
	PyramidConfigurationDialog( pyramid* thePyr );

	// static constructor
	static ConfigurationDialog* init( DataEntry* d ) {
		pyramid* c = dynamic_cast< pyramid* > (d);
		if( c )
			return new PyramidConfigurationDialog( dynamic_cast< pyramid* >( d ) );
		else
			return NULL;
	}

	// destructor
	virtual ~PyramidConfigurationDialog() { }

	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		PyramidConfigurationDialog* ccd = (PyramidConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}

	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		PyramidConfigurationDialog* ccd = (PyramidConfigurationDialog*)(data);
		ccd->CancelCallback_real( w );
	}

	// face callbacks
	static void XPCallback( Fl_Widget* w, void* data ) {
		PyramidConfigurationDialog* ccd = (PyramidConfigurationDialog*)(data);
		ccd->XPCallback_real( w );
	}

	static void XNCallback( Fl_Widget* w, void* data ) {
		PyramidConfigurationDialog* ccd = (PyramidConfigurationDialog*)(data);
		ccd->XNCallback_real( w );
	}

	static void YPCallback( Fl_Widget* w, void* data ) {
		PyramidConfigurationDialog* ccd = (PyramidConfigurationDialog*)(data);
		ccd->YPCallback_real( w );
	}

	static void YNCallback( Fl_Widget* w, void* data ) {
		PyramidConfigurationDialog* ccd = (PyramidConfigurationDialog*)(data);
		ccd->YNCallback_real( w );
	}

	static void ZNCallback( Fl_Widget* w, void* data ) {
		PyramidConfigurationDialog* ccd = (PyramidConfigurationDialog*)(data);
		ccd->ZNCallback_real( w );
	}

private:

	// the cone
	pyramid* thePyr;

	// set the selected face
	void setFace( int face );
	void saveFace( int face );

	// values
	double texsizeX[5];
	double texsizeY[5];
	double texoffsetX[5];
	double texoffsetY[5];
	bool drivethrough[5];
	bool shootthrough[5];
	bool ricochet[5];
	int selectedFace;


	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );
	void XPCallback_real( Fl_Widget* w );
	void XNCallback_real( Fl_Widget* w );
	void YPCallback_real( Fl_Widget* w );
	void YNCallback_real( Fl_Widget* w );
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

	// flip-z check-button
	Fl_Check_Button* flipzButton;
};

#endif /*PYRAMIDCONFIGURATIONDIALOG_H_*/
