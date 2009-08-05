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

	matrefLabel = new QuickLabel( "Material References:", 5, 30 );
	matrefBrowser = new Fl_Multi_Browser( 5, 55, 185, 90 );
	vector<string> matrefs = theMat->getMaterials();
	for ( vector<string>::const_iterator i = matrefs.begin(); i != matrefs.end(); i++ ) {
		matrefBrowser->add( (*i).c_str() );
	}
	matrefChoice = new Fl_Choice( 195, 55, 100, DEFAULT_TEXTSIZE + 6 );
	map<string, osg::ref_ptr< material > > materials = Model::getMaterials();
	for ( map<string, osg::ref_ptr< material > >::const_iterator i = materials.begin(); i != materials.end(); i++ ) {
		if ( i->second != theMat ) {
			matrefChoice->add( i->first.c_str() );
		}
	}
	matrefAddButton = new Fl_Button( 195, 80, 100, DEFAULT_TEXTSIZE + 6, "Add" );
	matrefAddButton->callback( MatrefAddCallback, this );
	matrefRemoveButton = new Fl_Button( 195, 105, 100, DEFAULT_TEXTSIZE + 6, "Remove" );
	matrefRemoveButton->callback( MatrefRemoveCallback, this );

	dyncolLabel = new QuickLabel( "Dynamic Color:", 5, 150 );
	dyncolChoice = new Fl_Choice( 150, 150, 120, DEFAULT_TEXTSIZE + 6 );
	map<string, dynamicColor*> dyncols = Model::getDynamicColors();
	dyncolChoice->add( "NONE" );
	for ( map<string, dynamicColor*>::const_iterator i = dyncols.begin(); i != dyncols.end(); i++ ) {
		int index = dyncolChoice->add( i->first.c_str() );

		if ( i->second == theMat->getDynamicColor() )
			dyncolChoice->value( index );
	}

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

	ambientLabel = new QuickLabel( "Ambient:", 5, 375 );
	ambientInput = new RGBAWidget( 80, 375 );
	ambientInput->setRGBA( theMat->getAmbient() );

	diffuseLabel = new QuickLabel( "Diffuse:", 5, 400 );
	diffuseInput = new RGBAWidget( 80, 400 );
	diffuseInput->setRGBA( theMat->getDiffuse() );

	specularLabel = new QuickLabel( "Specular:", 5, 425 );
	specularInput = new RGBAWidget( 80, 425 );
	specularInput->setRGBA( theMat->getSpecular() );

	emissiveLabel = new QuickLabel( "Emissive:", 5, 450 );
	emissiveInput = new RGBAWidget( 80, 450 );
	emissiveInput->setRGBA( theMat->getEmissive() );

	shininessLabel = new QuickLabel( "Shininess:", 5, 475 );
	shininessInput = new Fl_Value_Input( 150, 475, 120, DEFAULT_TEXTSIZE + 6 );
	shininessInput->value( theMat->getShininess() );

	alphaThresholdLabel = new QuickLabel( "Alpha Threshold:", 5, 500 );
	alphaThresholdInput = new Fl_Value_Input( 150, 500, 120, DEFAULT_TEXTSIZE + 6 );
	alphaThresholdInput->value( theMat->getAlphaThreshold() );


	textureLabel = new QuickLabel( "Texture:", 5, 525 );
	textureInput = new Fl_Input( 150, 525, 120, DEFAULT_TEXTSIZE + 6 );
	textureMatrixLabel = new QuickLabel( "Texture Matrix:", 5, 550 );
	textureMatrixChoice = new Fl_Choice( 150, 550, 120, DEFAULT_TEXTSIZE + 6 );
	map<string, texturematrix*> texmats = Model::getTextureMatrices();
	textureMatrixChoice->add( "NONE" );
	for (map<string, texturematrix*>::const_iterator i = texmats.begin(); i != texmats.end(); i++ ) {
		int index = textureMatrixChoice->add( i->first.c_str() );

		if ( theMat->getTextureCount() > 0 && i->second == theMat->getTextureMatrix( 0 ) )
			textureMatrixChoice->value( index );
	}
	noAlphaButton = new Fl_Check_Button( 5, 575, 200, DEFAULT_TEXTSIZE + 6, "No Alpha" );
	noColorButton = new Fl_Check_Button( 5, 600, 200, DEFAULT_TEXTSIZE + 6, "No Color" );
	sphereMapButton = new Fl_Check_Button( 5, 625, 200, DEFAULT_TEXTSIZE + 6, "Sphere Map" );

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
	theMat->setAmbient( ambientInput->getRGBA() );
	theMat->setDiffuse( diffuseInput->getRGBA() );
	theMat->setSpecular( specularInput->getRGBA() );
	theMat->setEmissive( emissiveInput->getRGBA() );
	theMat->setShininess( shininessInput->value() );
	theMat->setAlphaThreshold( alphaThresholdInput->value() );
	theMat->setTexture( string( textureInput->value() ) );
	theMat->setNoTexAlpha( noAlphaButton->value() == 1 ? true : false );
	theMat->setNoTexColor( noColorButton->value() == 1 ? true : false );
	theMat->setSphereMap( sphereMapButton->value() == 1 ? true : false );

	if ( dyncolChoice->text() != NULL && string( dyncolChoice->text() ) != "NONE" )
		theMat->setDynamicColor( Model::getDynamicColors()[ string( dyncolChoice->text() ) ] );
	else
		theMat->setDynamicColor( NULL );

	if (textureMatrixChoice->text() != NULL && string( textureMatrixChoice->text() ) != "NONE" )
		theMat->setTextureMatrix( Model::getTextureMatrices()[ string( textureMatrixChoice->text() ) ] );
	else
		theMat->setTextureMatrix( NULL );

	vector<string> matrefs;
	for ( int i = 1; i <= matrefBrowser->size(); i++ ) {
		matrefs.push_back( matrefBrowser->text( i ) );
	}
	theMat->setMaterials( matrefs );

	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void MaterialConfigurationDialog::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}

void MaterialConfigurationDialog::MatrefAddCallback_real( Fl_Widget* w ) {
	if ( matrefChoice->text() != NULL ) {
		matrefBrowser->add( matrefChoice->text() );
	}
}

void MaterialConfigurationDialog::MatrefRemoveCallback_real( Fl_Widget* w ) {
	for ( int i = 1; i <= matrefBrowser->size(); i++ ) {
		if ( matrefBrowser->selected( i ) ) {
			matrefBrowser->remove( i );
		}
	}
}
