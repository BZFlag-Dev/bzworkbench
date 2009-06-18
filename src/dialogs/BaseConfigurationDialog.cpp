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

#include "dialogs/BaseConfigurationDialog.h"

#include "flags.h"

// constructor
BaseConfigurationDialog::BaseConfigurationDialog( base* _theBase ) :
	ConfigurationDialog( _theBase, "Base Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	begin();
	
	theBase = _theBase;
	
	// set up the dialog
	teamLabel = new QuickLabel( "Team: ", 5, 5);
	redTeamButton = new Fl_Check_Button( 60, 5, 60, DEFAULT_TEXTSIZE + 6, "Red");
	redTeamButton->type( FL_RADIO_BUTTON );
	greenTeamButton = new Fl_Check_Button( 120, 5, 60, DEFAULT_TEXTSIZE + 6, "Green");
	greenTeamButton->type( FL_RADIO_BUTTON );
	blueTeamButton = new Fl_Check_Button( 180, 5, 60, DEFAULT_TEXTSIZE + 6, "Blue");
	blueTeamButton->type( FL_RADIO_BUTTON );
	purpleTeamButton = new Fl_Check_Button( 240, 5, 60, DEFAULT_TEXTSIZE + 6, "Purple");
	purpleTeamButton->type( FL_RADIO_BUTTON );
	int team = theBase->getTeam();
	switch ( team ) {
		case 1: redTeamButton->value( 1 ); break;
		case 2: greenTeamButton->value( 1 ); break;
		case 3: blueTeamButton->value( 1 ); break;
		case 4: purpleTeamButton->value( 1 ); break;
	}
	

	oncapLabel = new QuickLabel( "On capture: ", 5, 30 );
	oncapChoice = new Fl_Choice( 120, 30, 60, DEFAULT_TEXTSIZE + 6 );
	string flag = theBase->getWeapon();
	oncapChoice->add( "NONE" );
	oncapChoice->value( 0 );
	for (int i = 0; i < NUM_GOODFLAGS; i++) {
		oncapChoice->add( _goodFlags[i].c_str() );

		if ( _goodFlags[i] == flag ) {
			oncapChoice->value( i + 1 );
		}
	}
	for (int i = 0; i < NUM_BADFLAGS; i++) {
		oncapChoice->add( _badFlags[i].c_str() );

		if ( _badFlags[i] == flag ) {
			oncapChoice->value( NUM_GOODFLAGS + i + 1 );
		}
	}

	
	
	end();
	
	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );
	
}

// OK callback
void BaseConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI
	int team = 1;
	if ( redTeamButton->value() == 1 ) team = 1;
	else if ( greenTeamButton->value() == 1 ) team = 2;
	else if ( blueTeamButton->value() == 1 ) team = 3;
	else if ( purpleTeamButton->value() == 1 ) team = 4;
	theBase->setTeam( team );

	const char* text = oncapChoice->text();
	if ( text != NULL ) {
		if ( string(text) == "NONE" )
			theBase->setWeapon( "" );
		else 
			theBase->setWeapon( text );
	}
	
	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void BaseConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}