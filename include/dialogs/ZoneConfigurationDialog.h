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

#ifndef ZONECONFIGURATIONDIALOG_H_
#define ZONECONFIGURATIONDIALOG_H_

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

#include "objects/zone.h"

class ZoneConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 380;
	static const int DEFAULT_HEIGHT = 350;

	// constructor
	ZoneConfigurationDialog( zone* theZone );

	// static constructor
	static ConfigurationDialog* init( DataEntry* d ) {
		zone* c = dynamic_cast< zone* > (d);
		if( c )
			return new ZoneConfigurationDialog( dynamic_cast< zone* >( d ) );
		else
			return NULL;
	}

	// destructor
	virtual ~ZoneConfigurationDialog() { }

	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		ZoneConfigurationDialog* ccd = (ZoneConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}

	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		ZoneConfigurationDialog* ccd = (ZoneConfigurationDialog*)(data);
		ccd->CancelCallback_real( w );
	}

	// add/remove callbacks
	static void AddCallback( Fl_Widget* w, void* data ) {
		ZoneConfigurationDialog* ccd = (ZoneConfigurationDialog*)(data);
		ccd->AddCallback_real( w );
	}

	static void RemoveCallback( Fl_Widget* w, void* data ) {
		ZoneConfigurationDialog* ccd = (ZoneConfigurationDialog*)(data);
		ccd->RemoveCallback_real( w );
	}

private:

	// the cone
	zone* theZone;

	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );
	void AddCallback_real( Fl_Widget* w );
	void RemoveCallback_real( Fl_Widget* w );

	// face label
	QuickLabel* teamLabel;

	// team buttons
	Fl_Check_Button* rogueTeamButton;
	Fl_Check_Button* redTeamButton;
	Fl_Check_Button* greenTeamButton;
	Fl_Check_Button* blueTeamButton;
	Fl_Check_Button* purpleTeamButton;

	// safety label
	QuickLabel* safetyLabel;

	// safety buttons
	Fl_Check_Button* redSafetyButton;
	Fl_Check_Button* greenSafetyButton;
	Fl_Check_Button* blueSafetyButton;
	Fl_Check_Button* purpleSafetyButton;

	// flag box
	Fl_Box* flagBox;

	// flag labels
	QuickLabel* flagLabel;

	// texoffset value inputs
	Fl_Check_Browser* flagBrowser;

	// zoneflag label
	QuickLabel* zoneflagLabel;

	// zoneflag browser
	Fl_Multi_Browser* zoneflagBrowser;

	QuickLabel* zoneflagFlagLabel;
	QuickLabel* zoneflagQtyLabel;
	Fl_Choice* zoneflagFlagInput;
	Fl_Value_Input* zoneflagQtyInput;

	// zoneflag add/remove buttons
	Fl_Button* zoneflagAddButton;
	Fl_Button* zoneflagRemoveButton;
};

#endif /*ZONECONFIGURATIONDIALOG_H_*/
