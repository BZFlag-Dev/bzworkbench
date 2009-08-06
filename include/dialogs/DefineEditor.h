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

#ifndef DEFINEEDITOR_H_
#define DEFINEEDITOR_H_

#include "Fl_Dialog.h"
#include "FL/Fl_Multi_Browser.H"
#include "FL/Fl_Button.H"

#include "model/Model.h"
#include "widgets/QuickLabel.h"

class DefineEditor : public Fl_Dialog {

public:

	// constructor
	DefineEditor( Model* model );

	// destructor
	virtual ~DefineEditor() { }

	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		DefineEditor* ccd = (DefineEditor*)(data);
		ccd->OKCallback_real( w );
	}

	static void RemoveCallback( Fl_Widget* w, void* data ) {
		DefineEditor* ccd = (DefineEditor*)(data);
		ccd->RemoveCallback_real( w );
	}

	static void EditCallback( Fl_Widget* w, void* data ) {
		DefineEditor* ccd = (DefineEditor*)(data);
		ccd->EditCallback_real( w );
	}

protected:
	Model* model;

	// define label
	QuickLabel* defineLabel;

	// define browser
	Fl_Multi_Browser* defineBrowser;

	// define buttons
	Fl_Button* removeButton;
	Fl_Button* editButton;

private:
	void refreshDefineList();

	// real callbacks
	void OKCallback_real( Fl_Widget* w );

	void RemoveCallback_real( Fl_Widget* w );
	void EditCallback_real( Fl_Widget* w );
};

#endif /* PHYSICSEDITOR_H_ */
