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

#include "dialogs/TeleporterConfigurationDialog.h"

// constructor
TeleporterConfigurationDialog::TeleporterConfigurationDialog( teleporter* _theTele ) :
	ConfigurationDialog( _theTele, "Teleporter Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	begin();
	
	theTele = _theTele;
	
	// set up the dialog
	texsizeLabel = new QuickLabel( "Texture Size: ", 5, 5);
	texsizeField = new Fl_Value_Input( 120, 5, 120, DEFAULT_TEXTSIZE + 6);
	texsizeField->value( theTele->getTexsize() );
	
	borderLabel = new QuickLabel("Border: ", 5, 30);
	borderField = new Fl_Value_Input( 120, 30, 120, DEFAULT_TEXTSIZE + 6);
	borderField->value( theTele->getBorder() );
	
	
	end();
	
	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );
	
}

// OK callback
void TeleporterConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI
	theTele->setTexsize( texsizeField->value() );
	theTele->setBorder( borderField->value() );
	
	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void TeleporterConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}
