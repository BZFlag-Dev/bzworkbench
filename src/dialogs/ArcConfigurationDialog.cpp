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

#include "dialogs/ArcConfigurationDialog.h"

// constructor
ArcConfigurationDialog::ArcConfigurationDialog( arc* _theArc ) :
	ConfigurationDialog( _theArc, "Arc Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	begin();
	
	theArc = _theArc;
	
	// set up the dialog
	texsizeLabel = new QuickLabel( "Texture Size: ", 5, 5);
	texsizeXField = new Fl_Value_Input( 120, 5, 60, DEFAULT_TEXTSIZE + 6);
	texsizeYField = new Fl_Value_Input( 180, 5, 60, DEFAULT_TEXTSIZE + 6);
	texsizeZField = new Fl_Value_Input( 240, 5, 60, DEFAULT_TEXTSIZE + 6);
	texsizeWField = new Fl_Value_Input( 300, 5, 60, DEFAULT_TEXTSIZE + 6);
	texsizeXField->value( theArc->getTexsize().x() );
	texsizeYField->value( theArc->getTexsize().y() );
	texsizeZField->value( theArc->getTexsize().z() );
	texsizeWField->value( theArc->getTexsize().w() );
	
	
	subdivisionLabel = new QuickLabel("Subdivisions: ", 5, 30);
	subdivisionCounter = new Fl_Counter( 120, 30, 120, DEFAULT_TEXTSIZE + 6 );
	subdivisionCounter->type( FL_SIMPLE_COUNTER );
	subdivisionCounter->value( theArc->getDivisions() );
	subdivisionCounter->step( 1.0 );
	
	sweepAngleLabel = new QuickLabel("Sweep Angle: ", 5, 55 );
	sweepAngleCounter = new Fl_Counter( 120, 55, 120, DEFAULT_TEXTSIZE + 6 );
	sweepAngleCounter->type( FL_NORMAL_COUNTER );
	sweepAngleCounter->value( theArc->getSweepAngle() );

	ratioLabel = new QuickLabel("Ratio: ", 5, 80 );
	ratioCounter = new Fl_Counter( 120, 80, 120, DEFAULT_TEXTSIZE + 6 );
	ratioCounter->type( FL_NORMAL_COUNTER );
	ratioCounter->maximum( 1.0 );
	ratioCounter->minimum( 0.0 );
	ratioCounter->value( theArc->getRatio() );
	
	flatShadingButton = new Fl_Check_Button(5, 105, DEFAULT_WIDTH - 10, DEFAULT_TEXTSIZE + 6, "Flat Shading");
	flatShadingButton->value( theArc->getFlatshading() == true ? 1 : 0 );
	
	smoothBounceButton = new Fl_Check_Button(5, 130, DEFAULT_WIDTH - 10, DEFAULT_TEXTSIZE + 6, "Smooth Bounce");
	smoothBounceButton->value( theArc->getSmoothbounce() == true ? 1 : 0 );


	
	end();
	
	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );
	
}

// OK callback
void ArcConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI
	theArc->setSmoothbounce( smoothBounceButton->value() == 1 ? true : false );
	theArc->setSweepAngle( sweepAngleCounter->value() );
	theArc->setRatio( ratioCounter->value() );
	theArc->setDivisions( (int)subdivisionCounter->value() );
	theArc->setFlatshading( flatShadingButton->value() == 1 ? true : false );
	theArc->setTexsize( Point4D( texsizeXField->value(), texsizeYField->value(), texsizeZField->value(), texsizeWField->value() ) );
	
	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void ArcConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}
