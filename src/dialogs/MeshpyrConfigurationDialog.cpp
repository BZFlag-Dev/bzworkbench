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

#include "dialogs/MeshpyrConfigurationDialog.h"

// constructor
MeshpyrConfigurationDialog::MeshpyrConfigurationDialog( meshpyr* _theMeshpyr ) :
	ConfigurationDialog( _theMeshpyr, "Meshpyr Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	begin();
	
	theMeshpyr = _theMeshpyr;
	
	// set up the dialog
	texsizeLabel = new QuickLabel( "Texture Size: ", 5, 5);
	texsizeXField = new Fl_Value_Input( 120, 5, 60, DEFAULT_TEXTSIZE + 6);
	texsizeYField = new Fl_Value_Input( 180, 5, 60, DEFAULT_TEXTSIZE + 6);
	texsizeXField->value( theMeshpyr->getTexsize().x() );
	texsizeYField->value( theMeshpyr->getTexsize().y() );

	flipzButton = new Fl_Check_Button(5, 30, DEFAULT_WIDTH - 10, DEFAULT_TEXTSIZE + 6, "Flip Z");
	flipzButton->value( theMeshpyr->getFlipz() == true ? 1 : 0 );
	
	end();
	
	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );
	
}

// OK callback
void MeshpyrConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI
	theMeshpyr->setTexsize( Point2D( texsizeXField->value(), texsizeYField->value() ) );
	theMeshpyr->setFlipz( flipzButton->value() == 1 ? true : false );
	
	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void MeshpyrConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}
