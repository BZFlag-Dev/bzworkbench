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

#include "dialogs/MaterialConfigurationDialog.h"

// constructor
MaterialConfigurationDialog::MaterialConfigurationDialog( material* _theMat ) :
	ConfigurationDialog( _theMat, "Base Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	begin();

	theMat = _theMat;

	nameLabel = new QuickLabel( "Name:", 5, 5 );
	nameInput = new Fl_Input( 60, 5, 230, DEFAULT_TEXTSIZE + 6);

	matrefLabel = new QuickLabel( "Material References:", 5, 30 );
	matrefBrowser = new Fl_Multi_Browser( 5, 55, 185, 90 );
	matrefChoice = new Fl_Choice( 195, 55, 100, DEFAULT_TEXTSIZE + 6 );
	matrefAddButton = new Fl_Button( 195, 80, 100, DEFAULT_TEXTSIZE + 6, "Add" );
	matrefRemoveButton = new Fl_Button( 195, 105, 100, DEFAULT_TEXTSIZE + 6, "Remove" );

	dyncolLabel = new QuickLabel( "Dynamic Color:", 5, 150 );
	dyncolChoice = new Fl_Choice( 150, 150, 120, DEFAULT_TEXTSIZE + 6 );

	noTexturesButton = new Fl_Check_Button( 5, 175, 200, DEFAULT_TEXTSIZE + 6, "No Textures" );
	noShadowButton = new Fl_Check_Button( 5, 200, 200, DEFAULT_TEXTSIZE + 6, "No Shadow" );
	noCullingButton = new Fl_Check_Button( 5, 225, 200, DEFAULT_TEXTSIZE + 6, "No Culling" );
	noSortingButton = new Fl_Check_Button( 5, 250, 200, DEFAULT_TEXTSIZE + 6, "No Sorting" );
	noRadarButton = new Fl_Check_Button( 5, 275, 200, DEFAULT_TEXTSIZE + 6, "No Radar" );
	noLightingButton = new Fl_Check_Button( 5, 300, 200, DEFAULT_TEXTSIZE + 6, "No Lighting" );
	groupAlphaButton = new Fl_Check_Button( 5, 325, 200, DEFAULT_TEXTSIZE + 6, "Group Alpha" );
	occluderButton = new Fl_Check_Button( 5, 350, 200, DEFAULT_TEXTSIZE + 6, "Occluder" );

	alphaThresholdLabel = new QuickLabel( "Alpha Threshold:", 5, 375 );
	alphaThresholdInput = new Fl_Value_Input( 150, 375, 120, DEFAULT_TEXTSIZE + 6 );

	textureLabel = new QuickLabel( "Texture:", 5, 400 );
	textureInput = new Fl_Input( 150, 400, 120, DEFAULT_TEXTSIZE + 6 );

	textureMatrixLabel = new QuickLabel( "Texture Matrix:", 5, 425 );
	textureMatrixChoice = new Fl_Choice( 150, 425, 120, DEFAULT_TEXTSIZE + 6 );

	noAlphaButton = new Fl_Check_Button( 5, 450, 200, DEFAULT_TEXTSIZE + 6, "No Alpha" );
	noColorButton = new Fl_Check_Button( 5, 475, 200, DEFAULT_TEXTSIZE + 6, "No Color" );
	sphereMapButton = new Fl_Check_Button( 5, 500, 200, DEFAULT_TEXTSIZE + 6, "Sphere Map" );

	end();

	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );

}

// OK callback
void MaterialConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI


	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void MaterialConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}
