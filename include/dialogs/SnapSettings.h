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

#ifndef SNAPSETTINGS_H_
#define SNAPSETTINGS_H_

#include "Fl_Dialog.h"

#include "FL/Fl_Value_Input.H"

#include "model/Model.h"
#include "widgets/QuickLabel.h"
#include "windows/MainWindow.h"

class SnapSettings : public Fl_Dialog {

public:

	// constructor
	SnapSettings( MainWindow* mw );

	// destructor
	virtual ~SnapSettings() { }

	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		SnapSettings* ccd = (SnapSettings*)(data);
		ccd->OKCallback_real( w );
	}

	// Cancel callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		SnapSettings* ccd = (SnapSettings*)(data);
		ccd->CancelCallback_real( w );
	}

	

protected:
	MainWindow* parent;

	// translate snap size
	QuickLabel* translateLabel;
	Fl_Value_Input* translateInput;

	// translate snap size
	QuickLabel* rotateLabel;
	Fl_Value_Input* rotateInput;

	// translate snap size
	QuickLabel* scaleLabel;
	Fl_Value_Input* scaleInput;

private:
	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );

};

#endif