/* BZWorkbench
 * Copyright (c) 1993 - 2010 Tim Riker
 *
 * This package is free software;  you can redistribute it and/or
 * modify it under the terms of the license found in the file
 * named COPYING that should have accompanied this file.
 *
 * THIS PACKAGE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include "dialogs/BoxConfigurationDialog.h"

// constructor
BoxConfigurationDialog::BoxConfigurationDialog( ::box* _theBox ) :
	ConfigurationDialog( _theBox, "Box Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	begin();
	
	theBox = _theBox;

	// get values from box
	selectedFace = -1;
	for ( int i = 0; i < 6; i++ ) {
		texsizeX[i] = theBox->getTexsize( i ).x();
		texsizeY[i] = theBox->getTexsize( i ).y();
		texoffsetX[i] = theBox->getTexoffset( i ).x();
		texoffsetY[i] = theBox->getTexoffset( i ).y();
		drivethrough[i] = theBox->getDrivethrough( i );
		shootthrough[i] = theBox->getShootthrough( i );
		ricochet[i] = theBox->getRicochet( i );
	}

	
	
	// set up the dialog
	faceLabel = new QuickLabel("Face: ", 5, 5);
	faceChoice = new Fl_Choice( 120, 5, 120, DEFAULT_TEXTSIZE + 6 );
	faceChoice->add( "X+", 0, BoxConfigurationDialog::XPCallback, this );
	faceChoice->add( "X-", 0, BoxConfigurationDialog::XNCallback, this );
	faceChoice->add( "Y+", 0, BoxConfigurationDialog::YPCallback, this );
	faceChoice->add( "Y-", 0, BoxConfigurationDialog::YNCallback, this );
	faceChoice->add( "Z+", 0, BoxConfigurationDialog::ZPCallback, this );
	faceChoice->add( "Z-", 0, BoxConfigurationDialog::ZNCallback, this );

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

	faceChoice->value( 0 );
	setFace( 0 );
	
	end();
	
	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );
	
}

// OK callback
void BoxConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI
	saveFace( selectedFace );
	for ( int i = 0; i < 6; i++ ) {
		theBox->setTexsize( i, Point2D( texsizeX[i], texsizeY[i] ) );
		theBox->setTexoffset( i, Point2D( texoffsetX[i], texoffsetY[i] ) );
		theBox->setDrivethrough( i, drivethrough[i] );
		theBox->setShootthrough( i, shootthrough[i] );
		theBox->setRicochet( i, ricochet[i] );
	}
	
	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void BoxConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}

void BoxConfigurationDialog::XPCallback_real( Fl_Widget* w ) {
	setFace( 0 );
}

void BoxConfigurationDialog::XNCallback_real( Fl_Widget* w ) {
	setFace( 1 );
}

void BoxConfigurationDialog::YPCallback_real( Fl_Widget* w ) {
	setFace( 2 );
}

void BoxConfigurationDialog::YNCallback_real( Fl_Widget* w ) {
	setFace( 3 );
}

void BoxConfigurationDialog::ZPCallback_real( Fl_Widget* w ) {
	setFace( 4 );
}

void BoxConfigurationDialog::ZNCallback_real( Fl_Widget* w ) {
	setFace( 5 );
}

void BoxConfigurationDialog::setFace( int face ) {
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

void BoxConfigurationDialog::saveFace( int face ) 
{
	texsizeX[face] = texsizeXInput->value();
	texsizeY[face] = texsizeYInput->value();
	texoffsetX[face] = texoffsetXInput->value();
	texoffsetY[face] = texoffsetYInput->value();
	drivethrough[face] = drivethroughButton->value() == 1 ? true : false;
	shootthrough[face] = shootthroughButton->value() == 1 ? true : false;
	ricochet[face] = ricochetButton->value() == 1 ? true : false;
}