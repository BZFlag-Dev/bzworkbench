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

#include "dialogs/InfoConfigurationDialog.h"

// constructor
InfoConfigurationDialog::InfoConfigurationDialog( info* _theInfo ) :
	ConfigurationDialog( _theInfo, "Info Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	begin();

	theInfo = _theInfo;

	// set up the dialog
	infoTextLabel = new QuickLabel( "Info text: ", 5, 5);
	infoTextInput = new Fl_Multiline_Input( 5, 30, 290, 270 );
	vector<string> lines = theInfo->getLines();
	string value;
	for ( vector<string>::iterator i = lines.begin(); i != lines.end(); i++ )
		value += *i + "\n";

	infoTextInput->value( value.c_str() );

	end();

	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );

}

// OK callback
void InfoConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI
	theInfo->setLines( BZWParser::getLines( "", infoTextInput->value() ) );

	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void InfoConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}
