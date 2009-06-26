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

#include "dialogs/DynamicColorConfigurationDialog.h"

// constructor
DynamicColorConfigurationDialog::DynamicColorConfigurationDialog( dynamicColor* _theDyncol ) :
	ConfigurationDialog( _theDyncol, "Dynamic Color Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	begin();

	theDyncol = _theDyncol;

	nameLabel = new QuickLabel( "Name:", 5, 5 );
	nameInput = new Fl_Input( 60, 5, 250, DEFAULT_TEXTSIZE + 6 );
	nameInput->value( theDyncol->getName().c_str() );

	colorLabel = new QuickLabel( "Color", 10, 40);
	commandLabel = new QuickLabel( "Type",70, 40);;
	paramsLabel = new QuickLabel( "Params", 140, 40);;

	scrollArea = new Fl_Scroll( 5, 65, 520, 200 );
	scrollArea->end();

	addButton = new Fl_Button( 5, 350, 70, DEFAULT_TEXTSIZE + 6, "Add" );
	addButton->callback( AddCallback, this );
	removeButton = new Fl_Button( 80, 350, 70, DEFAULT_TEXTSIZE + 6, "Remove" );
	removeButton->callback( RemoveCallback, this );

	end();

	vector<ColorCommand> redCommands = theDyncol->getRedCommands();
	for ( vector<ColorCommand>::iterator i = redCommands.begin(); i != redCommands.end(); i++ ) {
		ColorCommandWidget* ccw = new ColorCommandWidget( scrollArea->x() + 5,
			scrollArea->y() + (DEFAULT_TEXTSIZE*2 + 6)*commandWidgets.size(), "red", *i );

		commandWidgets.push_back( ccw );
		scrollArea->add( ccw );
	}

	vector<ColorCommand> greenCommands = theDyncol->getGreenCommands();
	for ( vector<ColorCommand>::iterator i = greenCommands.begin(); i != greenCommands.end(); i++ ) {
		ColorCommandWidget* ccw = new ColorCommandWidget( scrollArea->x() + 5,
			scrollArea->y() + (DEFAULT_TEXTSIZE*2 + 6)*commandWidgets.size(), "green", *i );

		commandWidgets.push_back( ccw );
		scrollArea->add( ccw );
	}

	vector<ColorCommand> blueCommands = theDyncol->getBlueCommands();
	for ( vector<ColorCommand>::iterator i = blueCommands.begin(); i != blueCommands.end(); i++ ) {
		ColorCommandWidget* ccw = new ColorCommandWidget( scrollArea->x() + 5,
			scrollArea->y() + (DEFAULT_TEXTSIZE*2 + 6)*commandWidgets.size(), "blue", *i );

		commandWidgets.push_back( ccw );
		scrollArea->add( ccw );
	}

	vector<ColorCommand> alphaCommands = theDyncol->getAlphaCommands();
	for ( vector<ColorCommand>::iterator i = alphaCommands.begin(); i != alphaCommands.end(); i++ ) {
		ColorCommandWidget* ccw = new ColorCommandWidget( scrollArea->x() + 5,
			scrollArea->y() + (DEFAULT_TEXTSIZE*2 + 6)*commandWidgets.size(), "alpha", *i );

		commandWidgets.push_back( ccw );
		scrollArea->add( ccw );
	}

	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );

}

// OK callback
void DynamicColorConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI
	vector<ColorCommand> redCommands;
	vector<ColorCommand> greenCommands;
	vector<ColorCommand> blueCommands;
	vector<ColorCommand> alphaCommands;
	for ( list<ColorCommandWidget*>::iterator i = commandWidgets.begin(); i != commandWidgets.end(); i++ ) {
		string color = (*i)->getColor();

		if ( color == "red" ) {
			redCommands.push_back( (*i)->getEditedCommand() );
		}
		else if ( color == "green" ) {
			greenCommands.push_back( (*i)->getEditedCommand() );
		}
		else if ( color == "blue" ) {
			blueCommands.push_back( (*i)->getEditedCommand() );
		}
		else if ( color == "alpha" ) {
			alphaCommands.push_back( (*i)->getEditedCommand() );
		}
	}
	theDyncol->setRedCommands( redCommands );
	theDyncol->setGreenCommands( greenCommands );
	theDyncol->setBlueCommands( blueCommands );
	theDyncol->setAlphaCommands( alphaCommands );

	theDyncol->setName( string( nameInput->value() ) );

	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void DynamicColorConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}

void DynamicColorConfigurationDialog::AddCallback_real( Fl_Widget* w ) {
	ColorCommandWidget* ccw = new ColorCommandWidget( scrollArea->x() + 5,
			scrollArea->y() + (DEFAULT_TEXTSIZE*2 + 6)*commandWidgets.size() );

	commandWidgets.push_back( ccw );
	scrollArea->add( ccw );
	scrollArea->redraw();
}

void DynamicColorConfigurationDialog::RemoveCallback_real( Fl_Widget* w ) {
	list<ColorCommandWidget*>::reverse_iterator last = commandWidgets.rbegin();

	scrollArea->remove( *last );
	commandWidgets.remove( *last );
	scrollArea->redraw();
}
