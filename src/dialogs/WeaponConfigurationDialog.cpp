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

#include "dialogs/WeaponConfigurationDialog.h"

// constructor
WeaponConfigurationDialog::WeaponConfigurationDialog( weapon* _theWeapon ) :
	ConfigurationDialog( _theWeapon, "Weapon Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	begin();
	
	theWeapon = _theWeapon;

	colorLabel = new QuickLabel( "Color: ", 5, 5 );
	colorButtons = new Fl_Group( 60, 5, 375, DEFAULT_TEXTSIZE + 6 );
	rogueColorButton = new Fl_Round_Button( 60, 5, 60, DEFAULT_TEXTSIZE + 6, "Rogue");
	rogueColorButton->type( FL_RADIO_BUTTON );
	redColorButton = new Fl_Round_Button( 120, 5, 60, DEFAULT_TEXTSIZE + 6, "Red");
	redColorButton->type( FL_RADIO_BUTTON );
	greenColorButton = new Fl_Round_Button( 180, 5, 60, DEFAULT_TEXTSIZE + 6, "Green");
	greenColorButton->type( FL_RADIO_BUTTON );
	blueColorButton = new Fl_Round_Button( 240, 5, 60, DEFAULT_TEXTSIZE + 6, "Blue");
	blueColorButton->type( FL_RADIO_BUTTON );
	purpleColorButton = new Fl_Round_Button( 300, 5, 60, DEFAULT_TEXTSIZE + 6, "Purple");
	purpleColorButton->type( FL_RADIO_BUTTON );
	colorButtons->end();
	switch ( theWeapon->getColor() ) {
		case 0: rogueColorButton->setonly(); break;
		case 1: redColorButton->setonly(); break;
		case 2: greenColorButton->setonly(); break;
		case 3: blueColorButton->setonly(); break;
		case 4: purpleColorButton->setonly(); break;
		default: rogueColorButton->setonly(); break;
	}

	tiltLabel = new QuickLabel( "Tilt: ", 5, 30 );
	tiltInput = new Fl_Value_Input( 120, 30, 120, DEFAULT_TEXTSIZE + 6 );
	tiltInput->value( theWeapon->getTilt() );

	initdelayLabel = new QuickLabel( "Init Delay: ", 5, 55 );
	initdelayInput = new Fl_Value_Input( 120, 55, 120, DEFAULT_TEXTSIZE + 6 );
	initdelayInput->value( theWeapon->getInitdelay() );

	delayLabel = new QuickLabel( "Delays: ", 5, 80 );
	delayBrowser = new Fl_Multi_Browser( 5, 105, 305, 90 );
	delayInput = new Fl_Value_Input( 315, 105, 60, DEFAULT_TEXTSIZE + 6 );
	delayAddButton = new Fl_Button( 315, 130, 60, DEFAULT_TEXTSIZE + 6, "Add" );
	delayAddButton->callback( AddCallback, this );
	delayRemoveButton = new Fl_Button( 315, 155, 60, DEFAULT_TEXTSIZE + 6, "Remove" );
	delayRemoveButton->callback( RemoveCallback, this );
	vector<float> delay = theWeapon->getDelay();
	for ( vector<float>::iterator i = delay.begin(); i != delay.end(); i++ ) {
		delayBrowser->add( ftoa( *i ).c_str() );
	}

	type = new QuickLabel( "Type: ", 5, 200 );
	typeChoice = new Fl_Choice( 120, 200, 120, DEFAULT_TEXTSIZE + 6 );
	string type = theWeapon->getType();
	typeChoice->add( "NONE" );
	typeChoice->value( 0 );
	for (int i = 0; i < NUM_GOODFLAGS; i++) {
		typeChoice->add( _goodFlags[i].c_str() );

		if ( type == _goodFlags[i] ) {
			typeChoice->value( i + 1 );
		}
		
	}
	for (int i = 0; i < NUM_BADFLAGS; i++) {
		typeChoice->add( _badFlags[i].c_str() );

		if ( type == _badFlags[i] ) {
			typeChoice->value( NUM_GOODFLAGS + i + 1 );
		}
	}

	triggerLabel = new QuickLabel( "Trigger: ", 5, 225 );
	triggerChoice = new Fl_Choice( 120, 225, 120, DEFAULT_TEXTSIZE + 6 );
	triggerChoice->add( "oncap" );
	triggerChoice->add( "onspawn" );
	triggerChoice->add( "ondie" );
	string trigger = theWeapon->getTrigger();
	if ( trigger == "oncap" )
		triggerChoice->value( 0 );
	else if ( trigger == "onspawn" )
		triggerChoice->value( 1 );
	else if ( trigger == "ondie" )
		triggerChoice->value( 2 );
	else
		triggerChoice->value( 0 );

	eventteamLabel = new QuickLabel( "Event Team: ", 5, 250 );
	eventteamGroup = new Fl_Group( 120, 250, 240, DEFAULT_TEXTSIZE + 6);
	redEventteamButton = new Fl_Round_Button( 120, 250, 60, DEFAULT_TEXTSIZE + 6, "Red");
	redEventteamButton->type( FL_RADIO_BUTTON );
	greenEventteamButton = new Fl_Round_Button( 180, 250, 60, DEFAULT_TEXTSIZE + 6, "Green");
	greenEventteamButton->type( FL_RADIO_BUTTON );
	blueEventteamButton = new Fl_Round_Button( 240, 250, 60, DEFAULT_TEXTSIZE + 6, "Blue");
	blueEventteamButton->type( FL_RADIO_BUTTON );
	purpleEventteamButton = new Fl_Round_Button( 300, 250, 60, DEFAULT_TEXTSIZE + 6, "Purple");
	purpleEventteamButton->type( FL_RADIO_BUTTON );
	eventteamGroup->end();
	switch ( theWeapon->getEventTeam() ) {
		case 1: redEventteamButton->setonly(); break;
		case 2: greenEventteamButton->setonly(); break;
		case 3: blueEventteamButton->setonly(); break;
		case 4: purpleEventteamButton->setonly(); break;
		default: redEventteamButton->setonly(); break;
	}


	end();
	
	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );
	
}

// OK callback
void WeaponConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI
	int color = 0;
	if ( rogueColorButton->value() == 1 ) color = 0;
	else if ( redColorButton->value() == 1 ) color = 1;
	else if ( greenColorButton->value() == 1 ) color = 2;
	else if ( blueColorButton->value() == 1 ) color = 3;
	else if ( purpleColorButton->value() == 1 ) color = 4;
	theWeapon->setColor( color );

	theWeapon->setTilt( tiltInput->value() );
	theWeapon->setInitdelay( initdelayInput->value() );

	vector<float> delay;
	for ( int i = 1; i <= delayBrowser->size(); i++ )
		delay.push_back( atof( delayBrowser->text( i ) ) );
	theWeapon->setDelay( delay );

	theWeapon->setTrigger( string( triggerChoice->text() ) );

	theWeapon->setType( string( typeChoice->text() ) );

	int eventTeam = 1;
	if ( redEventteamButton->value() == 1 ) eventTeam = 1;
	else if ( greenEventteamButton->value() == 1 ) eventTeam = 2;
	else if ( blueEventteamButton->value() == 1 ) eventTeam = 3;
	else if ( purpleEventteamButton->value() == 1 ) eventTeam = 4;
	theWeapon->setEventTeam( eventTeam );
	
	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void WeaponConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}

void WeaponConfigurationDialog::AddCallback_real( Fl_Widget *w ) {
	delayBrowser->add( ftoa( delayInput->value() ).c_str() );
}

void WeaponConfigurationDialog::RemoveCallback_real(Fl_Widget *w) {
	for ( int i = 1; i <= delayBrowser->size(); i++ ) {
		if ( delayBrowser->selected( i ) )
			delayBrowser->remove( i );
	}
}