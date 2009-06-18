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

#ifndef WEAPONCONFIGURATIONDIALOG_H_
#define WEAPONCONFIGURATIONDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Round_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Multi_Browser.H>

#include "ConfigurationDialog.h"
#include "widgets/QuickLabel.h"
#include "defines.h"

#include "objects/weapon.h"

class WeaponConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 380;
	static const int DEFAULT_HEIGHT = 300;

	// constructor
	WeaponConfigurationDialog( weapon* theWeapon );

	// static constructor
	static ConfigurationDialog* init( DataEntry* d ) {
		weapon* c = dynamic_cast< weapon* > (d);
		if( c )
			return new WeaponConfigurationDialog( dynamic_cast< weapon* >( d ) );
		else
			return NULL;
	}

	// destructor
	virtual ~WeaponConfigurationDialog() { }

	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		WeaponConfigurationDialog* ccd = (WeaponConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}

	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		WeaponConfigurationDialog* ccd = (WeaponConfigurationDialog*)(data);
		ccd->CancelCallback_real( w );
	}

	// add callback
	static void AddCallback( Fl_Widget* w, void* data ) {
		WeaponConfigurationDialog* ccd = (WeaponConfigurationDialog*)(data);
		ccd->AddCallback_real( w );
	}

	// remove callback
	static void RemoveCallback( Fl_Widget* w, void* data ) {
		WeaponConfigurationDialog* ccd = (WeaponConfigurationDialog*)(data);
		ccd->RemoveCallback_real( w );
	}

private:

	// the cone
	weapon* theWeapon;

	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );
	void AddCallback_real( Fl_Widget* w );
	void RemoveCallback_real( Fl_Widget* w );

	// face label
	QuickLabel* colorLabel;

	// color buttons
	Fl_Group* colorButtons;
	Fl_Round_Button* rogueColorButton;
	Fl_Round_Button* redColorButton;
	Fl_Round_Button* greenColorButton;
	Fl_Round_Button* blueColorButton;
	Fl_Round_Button* purpleColorButton;

	// tilt label
	QuickLabel* tiltLabel;

	// tilt input 
	Fl_Value_Input* tiltInput;

	// initdelay label
	QuickLabel* initdelayLabel;

	// initdelay label
	Fl_Value_Input* initdelayInput;

	// delay label
	QuickLabel* delayLabel;

	// delay browser
	Fl_Multi_Browser* delayBrowser;

	// delay input
	Fl_Value_Input* delayInput;

	// zoneflag add/remove buttons
	Fl_Button* delayAddButton;
	Fl_Button* delayRemoveButton;

	// type label
	QuickLabel* type;

	// type choice
	Fl_Choice* typeChoice;

	// trigger label
	QuickLabel* triggerLabel;

	// trigger choice
	Fl_Choice* triggerChoice;

	// eventteam label
	QuickLabel* eventteamLabel;

	// eventteam buttons
	Fl_Group* eventteamGroup;
	Fl_Round_Button* redEventteamButton;
	Fl_Round_Button* greenEventteamButton;
	Fl_Round_Button* blueEventteamButton;
	Fl_Round_Button* purpleEventteamButton;
};

#endif /*WEAPONCONFIGURATIONDIALOG_H_*/
