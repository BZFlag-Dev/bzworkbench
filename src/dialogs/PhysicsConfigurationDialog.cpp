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

#include "dialogs/PhysicsConfigurationDialog.h"

// constructor
PhysicsConfigurationDialog::PhysicsConfigurationDialog( physics* _thePhysics ) :
	ConfigurationDialog( _thePhysics, "Physics Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	begin();

	thePhysics = _thePhysics;

	nameLabel = new QuickLabel( "Name:", 5, 5 );
	nameInput = new Fl_Input( 60, 5, 235, DEFAULT_TEXTSIZE + 6 );
	nameInput->value( thePhysics->getName().c_str() );

	linearLabel = new QuickLabel( "Linear:", 5, 30 );
	linearInput = new Point3DWidget( 60, 30 );
	linearInput->setPoint3D( thePhysics->getLinear() );

	angularLabel = new QuickLabel( "Angular:", 5, 55 );
	angularInput = new Point3DWidget( 60, 55 );
	angularInput->setPoint3D( thePhysics->getAngular() );

	slideLabel = new QuickLabel( "Slide:", 5, 80 );
	slideInput = new Fl_Value_Input( 60, 80, 235, DEFAULT_TEXTSIZE + 6 );
	slideInput->value( thePhysics->getSlide() );

	deathMessageLabel = new QuickLabel( "Death:", 5, 105 );
	deathMessageInput = new Fl_Input( 60, 105, 235, DEFAULT_TEXTSIZE + 6 );
	deathMessageInput->value( thePhysics->getDeathMessage().c_str() );

	end();

	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );

}

// OK callback
void PhysicsConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI
	thePhysics->setName( string( nameInput->value() ) );
	thePhysics->setLinear( linearInput->getPoint3D() );
	thePhysics->setAngular( angularInput->getPoint3D() );
	thePhysics->setSlide( slideInput->value() );
	thePhysics->setDeathMessage( string( deathMessageInput->value() ) );

	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void PhysicsConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}

