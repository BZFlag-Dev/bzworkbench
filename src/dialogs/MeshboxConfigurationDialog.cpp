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

#include "dialogs/MeshboxConfigurationDialog.h"

// constructor
MeshboxConfigurationDialog::MeshboxConfigurationDialog( meshbox* _theMeshbox ) :
	ConfigurationDialog( _theMeshbox, "Meshbox Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	begin();
	
	theMeshbox = _theMeshbox;
	
	// set up the dialog
	texsizeLabel = new QuickLabel( "Texture Size: ", 5, 5);
	texsizeXField = new Fl_Value_Input( 120, 5, 60, DEFAULT_TEXTSIZE + 6);
	texsizeYField = new Fl_Value_Input( 180, 5, 60, DEFAULT_TEXTSIZE + 6);
	texsizeZField = new Fl_Value_Input( 240, 5, 60, DEFAULT_TEXTSIZE + 6);
	texsizeWField = new Fl_Value_Input( 300, 5, 60, DEFAULT_TEXTSIZE + 6);
	texsizeXField->value( theMeshbox->getTexsize().x() );
	texsizeYField->value( theMeshbox->getTexsize().y() );
	texsizeZField->value( theMeshbox->getTexsize().z() );
	texsizeWField->value( theMeshbox->getTexsize().w() );
	
	end();
	
	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );
	
}

// OK callback
void MeshboxConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI
	theMeshbox->setTexsize( Point4D( texsizeXField->value(), texsizeYField->value(), texsizeZField->value(), texsizeWField->value() ) );
	
	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void MeshboxConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}
