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

#include "dialogs/GroupConfigurationDialog.h"

// constructor
GroupConfigurationDialog::GroupConfigurationDialog( group* _theGroup ) :
	ConfigurationDialog( _theGroup, "Group Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	begin();
	
	theGroup = _theGroup;

	defineLabel = new QuickLabel( "Define:", 5, 5 );
	defineChoice = new Fl_Choice( 120, 5, 170, DEFAULT_TEXTSIZE + 6 );

	// populate define choices
	map< string, define* > defines = Model::getGroups();
	for (map< string, define* >::iterator i = defines.begin(); i != defines.end(); i++ ) {
		defineChoice->add( i->first.c_str() );
	}
	defineChoice->value( 0 );
	
	end();
	
	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );
	
}

// OK callback
void GroupConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	define* def = dynamic_cast< define* >( Model::command( MODEL_GET, "define", string( defineChoice->text() ) ) );

	if ( def != NULL ) {
		theGroup->setDefine( def );
	}

	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void GroupConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}
