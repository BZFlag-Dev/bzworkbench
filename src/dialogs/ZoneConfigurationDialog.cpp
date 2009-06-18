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

#include "dialogs/ZoneConfigurationDialog.h"

// constructor
ZoneConfigurationDialog::ZoneConfigurationDialog( zone* _theZone ) :
	ConfigurationDialog( _theZone, "Zone Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	begin();

	theZone = _theZone;

	// set up the dialog
	teamLabel = new QuickLabel( "Team: ", 5, 5);
	rogueTeamButton = new Fl_Check_Button( 60, 5, 60, DEFAULT_TEXTSIZE + 6, "Rogue");
	redTeamButton = new Fl_Check_Button( 120, 5, 60, DEFAULT_TEXTSIZE + 6, "Red");
	greenTeamButton = new Fl_Check_Button( 180, 5, 60, DEFAULT_TEXTSIZE + 6, "Green");
	blueTeamButton = new Fl_Check_Button( 240, 5, 60, DEFAULT_TEXTSIZE + 6, "Blue");
	purpleTeamButton = new Fl_Check_Button( 300, 5, 60, DEFAULT_TEXTSIZE + 6, "Purple");
	vector<int> teams = theZone->getTeams();
	for (vector<int>::iterator itr = teams.begin(); itr != teams.end(); itr++) {
		switch ( *itr ) {
			case 0: rogueTeamButton->value( 1 ); break;
			case 1: redTeamButton->value( 1 ); break;
			case 2: greenTeamButton->value( 1 ); break;
			case 3: blueTeamButton->value( 1 ); break;
			case 4: purpleTeamButton->value( 1 ); break;
		}
	}

	safetyLabel = new QuickLabel( "Safety: ", 5, 30);
	redSafetyButton = new Fl_Check_Button( 120, 30, 60, DEFAULT_TEXTSIZE + 6, "Red");
	greenSafetyButton = new Fl_Check_Button( 180, 30, 60, DEFAULT_TEXTSIZE + 6, "Green");
	blueSafetyButton = new Fl_Check_Button( 240, 30, 60, DEFAULT_TEXTSIZE + 6, "Blue");
	purpleSafetyButton = new Fl_Check_Button( 300, 30, 60, DEFAULT_TEXTSIZE + 6, "Purple");
	vector<int> safety = theZone->getSafety();
	for (vector<int>::iterator itr = safety.begin(); itr != safety.end(); itr++) {
		switch ( *itr ) {
			case 1: redSafetyButton->value( 1 ); break;
			case 2: greenSafetyButton->value( 1 ); break;
			case 3: blueSafetyButton->value( 1 ); break;
			case 4: purpleSafetyButton->value( 1 ); break;
		}
	}

	flagLabel = new QuickLabel( "Flags: ", 5, 60 );
	flagBrowser = new Fl_Check_Browser( 5, 80, 370, 90 );
	vector<string> flags = theZone->getFlags();
	for (int i = 0; i < NUM_GOODFLAGS; i++) {
		flagBrowser->add( _goodFlags[i].c_str() );

		for (vector<string>::iterator itr = flags.begin(); itr != flags.end(); itr++) {
			if ( (*itr) == _goodFlags[i] ) {
				flagBrowser->checked( i + 1, 1 );
			}
		}
	}
	for (int i = 0; i < NUM_BADFLAGS; i++) {
		flagBrowser->add( _badFlags[i].c_str() );

		for (vector<string>::iterator itr = flags.begin(); itr != flags.end(); itr++) {
			if ( (*itr) == _badFlags[i] ) {
				flagBrowser->checked( NUM_GOODFLAGS + i + 1, 1 );
			}
		}
	}
	flagBrowser->add( "good" );
	flagBrowser->add( "bad" );
	for (vector<string>::iterator itr = flags.begin(); itr != flags.end(); itr++) {
		if ( *itr == "good" ) {
			flagBrowser->checked( NUM_GOODFLAGS + NUM_BADFLAGS + 1, 1 );
		}
		if ( *itr == "bad" ) {
			flagBrowser->checked( NUM_GOODFLAGS + NUM_BADFLAGS + 2, 1 );
		}
	}

	zoneflagLabel = new QuickLabel( "Zone Flags: ", 5, 175 );
	zoneflagBrowser = new Fl_Multi_Browser( 5, 195, 370, 90 );
	vector<zone::FlagElement> zoneflags = theZone->getZoneflags();
	for ( vector<zone::FlagElement>::iterator itr = zoneflags.begin(); itr != zoneflags.end(); itr++ )
		zoneflagBrowser->add( (string("Flag: ") + (*itr).getFlag() + " \tQuantity: " + string( itoa( (*itr).getQty() ) )).c_str() );

	zoneflagFlagLabel = new QuickLabel( "Flag: ", 5, 285 );
	zoneflagFlagInput = new Fl_Choice( 60, 285, 120, DEFAULT_TEXTSIZE + 6 );
	for (int i = 0; i < NUM_GOODFLAGS; i++)
		zoneflagFlagInput->add( _goodFlags[i].c_str() );
	for (int i = 0; i < NUM_BADFLAGS; i++)
		zoneflagFlagInput->add( _badFlags[i].c_str() );
	zoneflagFlagInput->value( 0 );
	zoneflagQtyLabel = new QuickLabel( "Qty: ", 180, 285 );
	zoneflagQtyInput = new Fl_Value_Input( 240, 285, 120, DEFAULT_TEXTSIZE + 6 );
	zoneflagAddButton = new Fl_Button( 5, 310, 60, DEFAULT_TEXTSIZE + 6, "Add" );
	zoneflagAddButton->callback( AddCallback, this );
	zoneflagRemoveButton = new Fl_Button( 70, 310, 60, DEFAULT_TEXTSIZE + 6, "Remove" );
	zoneflagRemoveButton->callback( RemoveCallback, this );


	end();

	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );

}

// OK callback
void ZoneConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI
	vector<int> teams;
	if ( rogueTeamButton->value() == 1 ) teams.push_back( 0 );
	if ( redTeamButton->value() == 1 ) teams.push_back( 1 );
	if ( greenTeamButton->value() == 1 ) teams.push_back( 2 );
	if ( blueTeamButton->value() == 1 ) teams.push_back( 3 );
	if ( purpleTeamButton->value() == 1 ) teams.push_back( 4 );
	theZone->setTeams( teams );

	vector<int> safety;
	if ( redSafetyButton->value() == 1 ) safety.push_back( 1 );
	if ( greenSafetyButton->value() == 1 ) safety.push_back( 2 );
	if ( blueSafetyButton->value() == 1 ) safety.push_back( 3 );
	if ( purpleSafetyButton->value() == 1 ) safety.push_back( 4 );
	theZone->setSafety( safety );

	vector<string> flags;
	for ( int i = 1; i <= flagBrowser->nitems(); i++ ) {
		if ( flagBrowser->checked( i ) == 1 )
			flags.push_back( flagBrowser->text( i ) );
	}
	theZone->setFlags( flags );

	vector<zone::FlagElement> zoneflags;
	for ( int i = 1; i <= zoneflagBrowser->size(); i++ ) {
		vector<string> elems = BZWParser::getLineElements( zoneflagBrowser->text( i ) );
		zoneflags.push_back(zone::FlagElement( elems[1], atoi( elems[3].c_str() ) ) );
	}
	theZone->setZoneflags( zoneflags );

	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void ZoneConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}

void ZoneConfigurationDialog::AddCallback_real( Fl_Widget* w ) {
	zoneflagBrowser->add( (string("Flag: ") + string(zoneflagFlagInput->text()) +
		" \tQuantity: " + string( itoa( zoneflagQtyInput->value() ) )).c_str() );
}

void ZoneConfigurationDialog::RemoveCallback_real( Fl_Widget* w ) {
	for ( int i = 1; i <= zoneflagBrowser->size(); i++ ) {
		if ( zoneflagBrowser->selected( i ) ) {
			zoneflagBrowser->remove( i );
		}
	}
}
