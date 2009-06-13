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

#ifndef BASECONFIGURATIONDIALOG_H_
#define BASECONFIGURATIONDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Check_Browser.H>
#include <FL/Fl_Multi_Browser.H>
#include <FL/Fl_Choice.H>

#include "ConfigurationDialog.h"
#include "widgets/QuickLabel.h"
#include "defines.h"

#include "objects/base.h"

class BaseConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 300;
	static const int DEFAULT_HEIGHT = 125;

	// constructor
	BaseConfigurationDialog( base* theBase );

	// static constructor
	static ConfigurationDialog* init( DataEntry* d ) {
		base* c = dynamic_cast< base* > (d);
		if( c )
			return new BaseConfigurationDialog( dynamic_cast< base* >( d ) );
		else
			return NULL;
	}

	// destructor
	virtual ~BaseConfigurationDialog() { }

	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		BaseConfigurationDialog* ccd = (BaseConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}

	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		BaseConfigurationDialog* ccd = (BaseConfigurationDialog*)(data);
		ccd->CancelCallback_real( w );
	}

private:

	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );

	// the cone
	base* theBase;

	// face label
	QuickLabel* teamLabel;

	// team buttons
	Fl_Group* teamButtons;
	Fl_Check_Button* redTeamButton;
	Fl_Check_Button* greenTeamButton;
	Fl_Check_Button* blueTeamButton;
	Fl_Check_Button* purpleTeamButton;

	// oncap label
	QuickLabel* oncapLabel;

	// oncap choice
	Fl_Choice* oncapChoice;
};

#endif /*BASECONFIGURATIONDIALOG_H_*/
