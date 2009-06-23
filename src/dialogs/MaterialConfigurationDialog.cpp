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
	nameInput->value( theMat->getName().c_str() );

	// FIXME: add matref functionality
	matrefLabel = new QuickLabel( "Material References:", 5, 30 );
	matrefBrowser = new Fl_Multi_Browser( 5, 55, 185, 90 );
	matrefChoice = new Fl_Choice( 195, 55, 100, DEFAULT_TEXTSIZE + 6 );
	matrefAddButton = new Fl_Button( 195, 80, 100, DEFAULT_TEXTSIZE + 6, "Add" );
	matrefRemoveButton = new Fl_Button( 195, 105, 100, DEFAULT_TEXTSIZE + 6, "Remove" );

	// FIXME: add dynamic color functionality
	dyncolLabel = new QuickLabel( "Dynamic Color:", 5, 150 );
	dyncolChoice = new Fl_Choice( 150, 150, 120, DEFAULT_TEXTSIZE + 6 );

	noTexturesButton = new Fl_Check_Button( 5, 175, 200, DEFAULT_TEXTSIZE + 6, "No Textures" );
	noTexturesButton->value( theMat->getNoTextures() ? true : false );
	noShadowButton = new Fl_Check_Button( 5, 200, 200, DEFAULT_TEXTSIZE + 6, "No Shadow" );
	noShadowButton->value( theMat->getNoShadows() ? true : false );
	noCullingButton = new Fl_Check_Button( 5, 225, 200, DEFAULT_TEXTSIZE + 6, "No Culling" );
	noCullingButton->value( theMat->getNoCulling() ? true : false );
	noSortingButton = new Fl_Check_Button( 5, 250, 200, DEFAULT_TEXTSIZE + 6, "No Sorting" );
	noSortingButton->value( theMat->getNoSorting() ? true : false );
	noRadarButton = new Fl_Check_Button( 5, 275, 200, DEFAULT_TEXTSIZE + 6, "No Radar" );
	noRadarButton->value( theMat->getNoRadar() ? true : false );
	noLightingButton = new Fl_Check_Button( 5, 300, 200, DEFAULT_TEXTSIZE + 6, "No Lighting" );
	noLightingButton->value( theMat->getNoLighting() ? true : false );
	groupAlphaButton = new Fl_Check_Button( 5, 325, 200, DEFAULT_TEXTSIZE + 6, "Group Alpha" );
	groupAlphaButton->value( theMat->getGroupAlpha() ? true : false );
	occluderButton = new Fl_Check_Button( 5, 350, 200, DEFAULT_TEXTSIZE + 6, "Occluder" );
	occluderButton->value( theMat->getOccluder() ? true : false );

	alphaThresholdLabel = new QuickLabel( "Alpha Threshold:", 5, 375 );
	alphaThresholdInput = new Fl_Value_Input( 150, 375, 120, DEFAULT_TEXTSIZE + 6 );
	alphaThresholdInput->value( theMat->getAlphaThreshold() );


	textureLabel = new QuickLabel( "Texture:", 5, 400 );
	textureInput = new Fl_Input( 150, 400, 120, DEFAULT_TEXTSIZE + 6 );
	textureMatrixLabel = new QuickLabel( "Texture Matrix:", 5, 425 );
	textureMatrixChoice = new Fl_Choice( 150, 425, 120, DEFAULT_TEXTSIZE + 6 );
	noAlphaButton = new Fl_Check_Button( 5, 450, 200, DEFAULT_TEXTSIZE + 6, "No Alpha" );
	noColorButton = new Fl_Check_Button( 5, 475, 200, DEFAULT_TEXTSIZE + 6, "No Color" );
	sphereMapButton = new Fl_Check_Button( 5, 500, 200, DEFAULT_TEXTSIZE + 6, "Sphere Map" );

	// only assign values if there is a texture
	if ( theMat->getTextureCount() > 0 ) {
		textureInput->value( theMat->getTexture( 0 ).c_str() );
		noAlphaButton->value( theMat->getNoTexAlpha( 0 ) ? true : false );
		noColorButton->value( theMat->getNoTexColor( 0 ) ? true : false );
		sphereMapButton->value( theMat->getUseSphereMap( 0 ) ? true : false );
	}

	end();

	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );

}

// OK callback
void MaterialConfigurationDialog::OKCallback_real( Fl_Widget* w ) {
	// call cone-specific setters from the UI
	theMat->setName( string( nameInput->value() ) );
	theMat->setNoTextures( noTexturesButton->value() == 1 ? true : false );
	theMat->setNoShadows( noShadowButton->value() == 1 ? true : false );
	theMat->setNoCulling( noCullingButton->value() == 1 ? true : false );
	theMat->setNoSorting( noSortingButton->value() == 1 ? true : false );
	theMat->setNoRadar( noRadarButton->value() == 1 ? true : false );
	theMat->setNoLighting( noLightingButton->value() == 1 ? true : false );
	theMat->setGroupAlpha( groupAlphaButton->value() == 1 ? true : false );
	theMat->setOccluder( occluderButton->value() == 1 ? true : false );
	theMat->setAlphaThreshold( alphaThresholdInput->value() );
	theMat->setTexture( string( textureInput->value() ) );
	theMat->setNoTexAlpha( noAlphaButton->value() == 1 ? true : false );
	theMat->setNoTexColor( noColorButton->value() == 1 ? true : false );
	theMat->setSphereMap( sphereMapButton->value() == 1 ? true : false );

	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void MaterialConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}
