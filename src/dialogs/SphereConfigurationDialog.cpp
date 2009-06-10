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

#include "dialogs/SphereConfigurationDialog.h"

// constructor
SphereConfigurationDialog::SphereConfigurationDialog( sphere* _theSphere ) :
	ConfigurationDialog( _theSphere, "Cone Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	begin();
	
	theSphere = _theSphere;
	
	// set up the dialog
	texsizeLabel = new QuickLabel( "Texture Size: ", 5, 5);
	texsizeXField = new Fl_Value_Input( 120, 5, 60, DEFAULT_TEXTSIZE + 6);
	texsizeYField = new Fl_Value_Input( 180, 5, 60, DEFAULT_TEXTSIZE + 6);
	texsizeXField->value( theSphere->getTexsize().x() );
	texsizeYField->value( theSphere->getTexsize().y() );
	
	
	subdivisionLabel = new QuickLabel("Subdivisions: ", 5, 30);
	subdivisionCounter = new Fl_Counter( 120, 30, 120, DEFAULT_TEXTSIZE + 6 );
	subdivisionCounter->type( FL_SIMPLE_COUNTER );
	subdivisionCounter->value( theSphere->getDivisions() );
	subdivisionCounter->step( 1.0 );
	
	flatShadingButton = new Fl_Check_Button(5, 55, DEFAULT_WIDTH - 10, DEFAULT_TEXTSIZE + 6, "Flat Shading");
	flatShadingButton->value( theSphere->getFlatshading() == true ? 1 : 0 );
	
	smoothBounceButton = new Fl_Check_Button(5, 80, DEFAULT_WIDTH - 10, DEFAULT_TEXTSIZE + 6, "Smooth Bounce");
	smoothBounceButton->value( theSphere->getSmoothbounce() == true ? 1 : 0 );

	hemisphereButton = new Fl_Check_Button(5, 105, DEFAULT_WIDTH - 10, DEFAULT_TEXTSIZE + 6, "Hemisphere");
	hemisphereButton->value( theSphere->getHemisphere() == true ? 1 : 0 );
	
	end();
	
	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );
	
}

// OK callback
void SphereConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI
	theSphere->setSmoothbounce( smoothBounceButton->value() == 1 ? true : false );
	theSphere->setDivisions( (int)subdivisionCounter->value() );
	theSphere->setFlatshading( flatShadingButton->value() == 1 ? true : false );
	theSphere->setTexsize( Point2D( texsizeXField->value(), texsizeYField->value() ) );
	theSphere->setHemisphere( hemisphereButton->value() == 1 ? true : false );
	
	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void SphereConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}
