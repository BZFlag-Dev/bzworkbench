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

#include "dialogs/ConeConfigurationDialog.h"

// constructor
ConeConfigurationDialog::ConeConfigurationDialog( cone* _theCone ) :
	ConfigurationDialog( _theCone, "Cone Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	begin();
	
	theCone = _theCone;
	
	// set up the dialog
	texsizeLabel = new QuickLabel( "Texture Size: ", 5, 5);
	texsizeXField = new Fl_Value_Input( 120, 5, 60, DEFAULT_TEXTSIZE + 6);
	texsizeYField = new Fl_Value_Input( 180, 5, 60, DEFAULT_TEXTSIZE + 6);
	texsizeXField->value( theCone->getTexsize().x() );
	texsizeYField->value( theCone->getTexsize().y() );
	
	
	subdivisionLabel = new QuickLabel("Subdivisions: ", 5, 30);
	subdivisionCounter = new Fl_Counter( 120, 30, 120, DEFAULT_TEXTSIZE + 6 );
	subdivisionCounter->type( FL_SIMPLE_COUNTER );
	subdivisionCounter->value( theCone->getDivisions() );
	subdivisionCounter->step( 1.0 );
	
	sweepAngleLabel = new QuickLabel("Sweep Angle: ", 5, 55 );
	sweepAngle = new Fl_Input( 120, 55, 120, DEFAULT_TEXTSIZE + 6 );
	sweepAngle->type( FL_FLOAT_INPUT );
	sweepAngle->callback(sweepAngle_cb);
	sweepAngle->when(FL_WHEN_CHANGED);
	sweepAngle->tooltip("float value between -360.0 to 360.0");
	sweepAngle->value( ftoa(theCone->getSweepAngle()).c_str() );
	
	flatShadingButton = new Fl_Check_Button(5, 80, DEFAULT_WIDTH - 10, DEFAULT_TEXTSIZE + 6, "Flat Shading");
	flatShadingButton->value( theCone->getFlatshading() == true ? 1 : 0 );
	
	smoothBounceButton = new Fl_Check_Button(5, 105, DEFAULT_WIDTH - 10, DEFAULT_TEXTSIZE + 6, "Smooth Bounce");
	smoothBounceButton->value( theCone->getSmoothbounce() == true ? 1 : 0 );

	flipzButton = new Fl_Check_Button(5, 130, DEFAULT_WIDTH - 10, DEFAULT_TEXTSIZE + 6, "Flip Z");
	flipzButton->value( theCone->getFlipz() == true ? 1 : 0 );
	
	end();
	
	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );
	
}

void ConeConfigurationDialog::sweepAngle_cb(Fl_Widget *o, void* data) {
	float f = atof( ((Fl_Input*)o)->value() );
	if(f > 360.0f)
		((Fl_Input*)o)->value("360.0");
	if(f < -360.0f)
		((Fl_Input*)o)->value("-360.0");
}

// OK callback
void ConeConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI
	theCone->setSmoothbounce( smoothBounceButton->value() == 1 ? true : false );
	theCone->setSweepAngle( atof(sweepAngle->value()) );
	theCone->setDivisions( (int)subdivisionCounter->value() );
	theCone->setFlatshading( flatShadingButton->value() == 1 ? true : false );
	theCone->setTexsize( Point2D( texsizeXField->value(), texsizeYField->value() ) );
	theCone->setFlipz( flipzButton->value() == 1 ? true : false );
	
	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void ConeConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}
