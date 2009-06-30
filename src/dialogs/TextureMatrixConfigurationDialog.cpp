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

#include "dialogs/TextureMatrixConfigurationDialog.h"

// constructor
TextureMatrixConfigurationDialog::TextureMatrixConfigurationDialog( texturematrix* _theTexmat ) :
	ConfigurationDialog( _theTexmat, "Dynamic Color Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	begin();

	theTexmat = _theTexmat;

	nameLabel = new QuickLabel( "Name:", 5, 5 );
	nameInput = new Fl_Input( 60, 5, 235, DEFAULT_TEXTSIZE + 6 );
	nameInput->value( theTexmat->getName().c_str() );

	scaleLabel = new QuickLabel( "Scale:", 5, 30 );
	scaleInput = new TexcoordWidget( 140, 30 );
	scaleInput->setTexcoord( theTexmat->getScale() );

	scaleFreqLabel = new QuickLabel( "Scale Frequency:", 5, 55 );
	scaleFreqInput = new TexcoordWidget( 140, 55 );
	scaleFreqInput->setTexcoord( theTexmat->getScaleFreq() );

	spinLabel = new QuickLabel( "Spin Frequency:", 5, 80 );
	spinInput = new Fl_Value_Input( 140, 80, 120, DEFAULT_TEXTSIZE + 6 );
	spinInput->value( theTexmat->getSpin() );

	shiftLabel = new QuickLabel( "Shift Frequency:", 5, 105 );
	shiftInput = new TexcoordWidget( 140, 105 );
	shiftInput->setTexcoord( theTexmat->getShift() );

	centerLabel = new QuickLabel( "Dynamic Center:", 5, 130 );
	centerInput = new TexcoordWidget( 140, 130 );
	centerInput->setTexcoord( theTexmat->getCenter() );

	fixedScaleLabel = new QuickLabel( "Fixed Scale:", 5, 155 );
	fixedScaleInput = new TexcoordWidget( 140, 155 );
	fixedScaleInput->setTexcoord( theTexmat->getFixedScale() );

	fixedSpinLabel = new QuickLabel( "Fixed Spin:", 5, 180 );
	fixedSpinInput = new Fl_Value_Input( 140, 180, 120, DEFAULT_TEXTSIZE + 6 );
	fixedSpinInput->value( theTexmat->getFixedSpin() );

	fixedShiftLabel = new QuickLabel( "Fixed Shift:", 5, 205 );
	fixedShiftInput = new TexcoordWidget( 140, 205 );
	fixedShiftInput->setTexcoord( theTexmat->getFixedShift() );

	fixedCenterLabel = new QuickLabel( "Fixed Center:", 5, 230 );
	fixedCenterInput = new TexcoordWidget( 140, 230 );
	fixedCenterInput->setTexcoord( theTexmat->getFixedCenter() );

	end();


	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );

}

// OK callback
void TextureMatrixConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI
	theTexmat->setName( string( nameInput->value() ) );
	theTexmat->setScale( scaleInput->getTexcoord() );
	theTexmat->setScaleFreq( scaleFreqInput->getTexcoord() );
	theTexmat->setShift( shiftInput->getTexcoord() );
	theTexmat->setCenter( centerInput->getTexcoord() );
	theTexmat->setFixedScale( fixedScaleInput->getTexcoord() );
	theTexmat->setFixedShift( fixedShiftInput->getTexcoord() );
	theTexmat->setFixedCenter( fixedCenterInput->getTexcoord() );
	theTexmat->setSpin( spinInput->value() );
	theTexmat->setFixedSpin( fixedSpinInput->value() );

	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void TextureMatrixConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}
