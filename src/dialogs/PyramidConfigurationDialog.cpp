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

#include "dialogs/PyramidConfigurationDialog.h"

// constructor
PyramidConfigurationDialog::PyramidConfigurationDialog( pyramid* _thePyr ) :
	ConfigurationDialog( _thePyr, "Pyramid Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	begin();
	
	thePyr = _thePyr;

	// get values from box
	selectedFace = -1;
	for ( int i = 0; i < 5; i++ ) {
		texsizeX[i] = thePyr->getTexsize( i ).x();
		texsizeY[i] = thePyr->getTexsize( i ).y();
		texoffsetX[i] = thePyr->getTexoffset( i ).x();
		texoffsetY[i] = thePyr->getTexoffset( i ).y();
		drivethrough[i] = thePyr->getDrivethrough( i );
		shootthrough[i] = thePyr->getShootthrough( i );
		ricochet[i] = thePyr->getRicochet( i );
	}

	
	
	// set up the dialog
	faceLabel = new QuickLabel("Face: ", 5, 5);
	faceChoice = new Fl_Choice( 120, 5, 120, DEFAULT_TEXTSIZE + 6 );
	faceChoice->add( "X+", 0, PyramidConfigurationDialog::XPCallback, this );
	faceChoice->add( "X-", 0, PyramidConfigurationDialog::XNCallback, this );
	faceChoice->add( "Y+", 0, PyramidConfigurationDialog::YPCallback, this );
	faceChoice->add( "Y-", 0, PyramidConfigurationDialog::YNCallback, this );
	faceChoice->add( "Bottom", 0, PyramidConfigurationDialog::ZNCallback, this );

	flbox = new Fl_Box( 1, 30, 298, 145 );
	flbox->box( FL_DOWN_BOX );
	
	texsizeLabel = new QuickLabel("Texture size: ", 5, 35 );
	texsizeXInput = new Fl_Value_Input( 120, 35, 60, DEFAULT_TEXTSIZE + 6 );
	texsizeYInput = new Fl_Value_Input( 180, 35, 60, DEFAULT_TEXTSIZE + 6 );

	texoffsetLabel = new QuickLabel("Texture offset: ", 5, 60 );
	texoffsetXInput = new Fl_Value_Input( 120, 60, 60, DEFAULT_TEXTSIZE + 6 );
	texoffsetYInput = new Fl_Value_Input( 180, 60, 60, DEFAULT_TEXTSIZE + 6 );
	
	drivethroughButton = new Fl_Check_Button(5, 85, DEFAULT_WIDTH - 10, DEFAULT_TEXTSIZE + 6, "Drive Through");
	
	shootthroughButton = new Fl_Check_Button(5, 110, DEFAULT_WIDTH - 10, DEFAULT_TEXTSIZE + 6, "Shoot Through");
	
	ricochetButton = new Fl_Check_Button(5, 135, DEFAULT_WIDTH - 10, DEFAULT_TEXTSIZE + 6, "Ricochet");

	flipzButton = new Fl_Check_Button(5, 180, DEFAULT_WIDTH - 10, DEFAULT_TEXTSIZE + 6, "Flip Z");
	flipzButton->value( thePyr->getFlipz() );

	faceChoice->value( 0 );
	setFace( 0 );
	
	end();
	
	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );
	
}

// OK callback
void PyramidConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI
	saveFace( selectedFace );
	for ( int i = 0; i < 5; i++ ) {
		thePyr->setTexsize( i, Point2D( texsizeX[i], texsizeY[i] ) );
		thePyr->setTexoffset( i, Point2D( texoffsetX[i], texoffsetY[i] ) );
		thePyr->setDrivethrough( i, drivethrough[i] );
		thePyr->setShootthrough( i, shootthrough[i] );
		thePyr->setRicochet( i, ricochet[i] );
	}
	thePyr->setFlipz( flipzButton->value() );

	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void PyramidConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}

void PyramidConfigurationDialog::XPCallback_real( Fl_Widget* w ) {
	setFace( 0 );
}

void PyramidConfigurationDialog::XNCallback_real( Fl_Widget* w ) {
	setFace( 1 );
}

void PyramidConfigurationDialog::YPCallback_real( Fl_Widget* w ) {
	setFace( 2 );
}

void PyramidConfigurationDialog::YNCallback_real( Fl_Widget* w ) {
	setFace( 3 );
}

void PyramidConfigurationDialog::ZNCallback_real( Fl_Widget* w ) {
	setFace( 4 );
}

void PyramidConfigurationDialog::setFace( int face ) {
	// save values if selected face is valid
	if ( selectedFace >= 0 ) {
		saveFace( selectedFace );
	}

	// set values of the new selected face
	texsizeXInput->value( texsizeX[face] );
	texsizeYInput->value( texsizeY[face] );
	texoffsetXInput->value( texoffsetX[face] );
	texoffsetYInput->value( texoffsetY[face] );
	drivethroughButton->value( drivethrough[face] );
	shootthroughButton->value( shootthrough[face] );
	ricochetButton->value( ricochet[face] );
	selectedFace = face;
}

void PyramidConfigurationDialog::saveFace( int face ) 
{
	texsizeX[face] = texsizeXInput->value();
	texsizeY[face] = texsizeYInput->value();
	texoffsetX[face] = texoffsetXInput->value();
	texoffsetY[face] = texoffsetYInput->value();
	drivethrough[face] = drivethroughButton->value();
	shootthrough[face] = shootthroughButton->value();
	ricochet[face] = ricochetButton->value();
}