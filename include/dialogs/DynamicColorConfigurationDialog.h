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

#ifndef DYNAMICCOLORCONFIGURATIONDIALOG_H_
#define DYNAMICCOLORCONFIGURATIONDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Scroll.H>

#include "ConfigurationDialog.h"
#include "widgets/QuickLabel.h"
#include "widgets/ColorCommandWidget.h"
#include "defines.h"

#include "objects/dynamicColor.h"

#include <list>

class DynamicColorConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 530;
	static const int DEFAULT_HEIGHT = 400;

	// constructor
	DynamicColorConfigurationDialog( dynamicColor* theDyncol );

	// static constructor
	static ConfigurationDialog* init( DataEntry* d ) {
		dynamicColor* c = dynamic_cast< dynamicColor* > (d);
		if( c )
			return new DynamicColorConfigurationDialog( dynamic_cast< dynamicColor* >( d ) );
		else
			return NULL;
	}

	// destructor
	virtual ~DynamicColorConfigurationDialog() { }

	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		DynamicColorConfigurationDialog* ccd = (DynamicColorConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}

	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		DynamicColorConfigurationDialog* ccd = (DynamicColorConfigurationDialog*)(data);
		ccd->CancelCallback_real( w );
	}

	static void AddCallback( Fl_Widget* w, void* data ) {
		DynamicColorConfigurationDialog* ccd = (DynamicColorConfigurationDialog*)(data);
		ccd->AddCallback_real( w );
	}

	static void RemoveCallback( Fl_Widget* w, void* data ) {
		DynamicColorConfigurationDialog* ccd = (DynamicColorConfigurationDialog*)(data);
		ccd->RemoveCallback_real( w );
	}

	static void EditCallback( Fl_Widget* w, void* data ) {
		DynamicColorConfigurationDialog* ccd = (DynamicColorConfigurationDialog*)(data);
		ccd->EditCallback_real( w );
	}


private:

	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );
	void AddCallback_real( Fl_Widget* w );
	void RemoveCallback_real( Fl_Widget* w );
	void EditCallback_real( Fl_Widget* w );

	// the cone
	dynamicColor* theDyncol;

	QuickLabel* nameLabel;
	Fl_Input* nameInput;

	QuickLabel* colorLabel;
	QuickLabel* commandLabel;
	QuickLabel* paramsLabel;

	Fl_Scroll* scrollArea;

	Fl_Button* addButton;
	Fl_Button* removeButton;
	Fl_Button* editButton;
	Fl_Button* upButton;
	Fl_Button* downButton;

	list<ColorCommandWidget*> commandWidgets;

};

#endif /* DYNAMICCOLORCONFIGURATIONDIALOG_H_ */
