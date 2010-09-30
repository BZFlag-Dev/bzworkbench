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

#ifndef TEXTUREMATRIXCONFIGURATIONDIALOG_H_
#define TEXTUREMATRIXCONFIGURATIONDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Value_Input.H>

#include "ConfigurationDialog.h"
#include "widgets/QuickLabel.h"
#include "widgets/TexcoordWidget.h"
#include "defines.h"

#include "objects/texturematrix.h"

#include <list>

class TextureMatrixConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 300;
	static const int DEFAULT_HEIGHT = 300;

	// constructor
	TextureMatrixConfigurationDialog( texturematrix* theTexmat );

	// static constructor
	static ConfigurationDialog* init( DataEntry* d ) {
		texturematrix* c = dynamic_cast< texturematrix* > (d);
		if( c )
			return new TextureMatrixConfigurationDialog( dynamic_cast< texturematrix* >( d ) );
		else
			return NULL;
	}

	// destructor
	virtual ~TextureMatrixConfigurationDialog() { }

	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		TextureMatrixConfigurationDialog* ccd = (TextureMatrixConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}

	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		TextureMatrixConfigurationDialog* ccd = (TextureMatrixConfigurationDialog*)(data);
		ccd->CancelCallback_real( w );
	}


private:

	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );

	texturematrix* theTexmat;

	QuickLabel* nameLabel;
	Fl_Input* nameInput;

	QuickLabel* scaleLabel;
	TexcoordWidget* scaleInput;

	QuickLabel* scaleFreqLabel;
	TexcoordWidget* scaleFreqInput;

	QuickLabel* spinLabel;
	Fl_Value_Input* spinInput;

	QuickLabel* shiftLabel;
	TexcoordWidget* shiftInput;

	QuickLabel* centerLabel;
	TexcoordWidget* centerInput;

	QuickLabel* fixedScaleLabel;
	TexcoordWidget* fixedScaleInput;

	QuickLabel* fixedSpinLabel;
	Fl_Value_Input* fixedSpinInput;

	QuickLabel* fixedShiftLabel;
	TexcoordWidget* fixedShiftInput;

	QuickLabel* fixedCenterLabel;
	TexcoordWidget* fixedCenterInput;


};

#endif /* TEXTUREMATRIXCONFIGURATIONDIALOG_H_ */
