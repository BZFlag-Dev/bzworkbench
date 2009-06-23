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

#include "dialogs/MaterialEditor.h"

#include "dialogs/MaterialConfigurationDialog.h"
#include "objects/material.h"
#include "model/SceneBuilder.h"

#include "defines.h"

MaterialEditor::MaterialEditor( Model* model ) :
		Fl_Dialog( "Material Editor", 310, 400, Fl_Dialog::Fl_OK | Fl_Dialog::Fl_CANCEL )
{
	this->model = model;

	begin();

	// material label
	materialLabel = new QuickLabel( "Materials: ", 5, 5 );

	// material browser
	materialBrowser = new Fl_Multi_Browser( 5, 30, 225, 90 );
	refreshMaterialList();

	// material buttons
	materialAddButton = new Fl_Button( 235, 30, 70, DEFAULT_TEXTSIZE + 6, "Add" );
	materialAddButton->callback( MaterialAddCallback, this );
	materialRemoveButton = new Fl_Button( 235, 55, 70, DEFAULT_TEXTSIZE + 6, "Remove" );
	materialRemoveButton->callback( MaterialRemoveCallback, this );
	materialEditButton = new Fl_Button( 235, 80, 70, DEFAULT_TEXTSIZE + 6, "Edit" );
	materialEditButton->callback( MaterialEditCallback, this );

	// texture matrix label
	textureMatrixLabel = new QuickLabel( "Texture Matrices: ", 5, 125 );

	// texture matrix browser
	textureMatrixBrowser = new Fl_Multi_Browser( 5, 150, 225, 90 );
	refreshTexmatList();

	// texture matrix buttons
	textureMatrixAddButton = new Fl_Button( 235, 150, 70, DEFAULT_TEXTSIZE + 6, "Add" );
	textureMatrixRemoveButton = new Fl_Button( 235, 175, 70, DEFAULT_TEXTSIZE + 6, "Remove" );
	textureMatrixEditButton = new Fl_Button( 235, 200, 70, DEFAULT_TEXTSIZE + 6, "Edit" );

	// dynamic color label
	dyncolLabel = new QuickLabel( "Dynamic Colors: ", 5, 245 );

	// dynamic color browser
	dyncolBrowser = new Fl_Multi_Browser( 5, 270, 225, 90 );

	// dynamic color buttons
	dyncolAddButton = new Fl_Button( 235, 270, 70, DEFAULT_TEXTSIZE + 6, "Add" );
	dyncolRemoveButton = new Fl_Button( 235, 295, 70, DEFAULT_TEXTSIZE + 6, "Remove" );
	dyncolEditButton = new Fl_Button( 235, 320, 70, DEFAULT_TEXTSIZE + 6, "Edit" );

	end();

	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );
}

void MaterialEditor::refreshMaterialList() {
	materialBrowser->clear();

	std::map<string, material*> materials = model->_getMaterials();
	
	std::map<string, material*>::const_iterator i;
	for ( i = materials.begin(); i != materials.end(); i++ ) {
		materialBrowser->add( i->first.c_str() );
	}
}

void MaterialEditor::refreshTexmatList() {
	textureMatrixBrowser->clear();

	std::map<string, texturematrix*> texmats = model->_getTextureMatrices();

	std::map<string, texturematrix*>::const_iterator i;
	for ( i = texmats.begin(); i != texmats.end(); i++ ) {
		textureMatrixBrowser->add( i->first.c_str() );
	}
}

void MaterialEditor::refreshDyncolList() {
	dyncolBrowser->clear();

	std::map<string, dynamicColor*> dyncols = model->_getDynamicColors();

	std::map<string, dynamicColor*>::const_iterator i;
	for ( i = dyncols.begin(); i != dyncols.end(); i++ ) {
		dyncolBrowser->add( i->first.c_str() );
	}
}

// OK callback
void MaterialEditor::OKCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}

// Cancel callback
void MaterialEditor::CancelCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}

void MaterialEditor::MaterialAddCallback_real( Fl_Widget* w ) {
	// make a new material object
	material* newObj = dynamic_cast< material* >( model->_buildObject( "material" ) );

	if(!newObj)
		return;

	model->_getMaterials()[ newObj->getName() ] = newObj;

	// make sure the materail shows up
	refreshMaterialList();
}

void MaterialEditor::MaterialRemoveCallback_real( Fl_Widget* w ) {
	// FIXME: implement
}

void MaterialEditor::MaterialEditCallback_real( Fl_Widget* w ) {
	// find the first selected item
	const char* name = NULL;
	for (int i = 1; i <= materialBrowser->size(); i++) {
		if ( materialBrowser->selected( i ) ) {
			name = materialBrowser->text( i );
			break;
		}
	}

	// make sure something was actually selected
	if ( name == NULL )
		return;

	// get the material
	material* mat = dynamic_cast< material* >( model->_command( MODEL_GET, "material", string( name ) ) );

	if ( mat != NULL ) {
		// open a material config dialog
		MaterialConfigurationDialog* mcd = new MaterialConfigurationDialog( mat );

		mcd->show();

		while ( mcd->shown() ) { Fl::wait(); }

		// clean up
		delete mcd;
	}

	refreshMaterialList();
}

void MaterialEditor::TexmatAddCallback_real( Fl_Widget* w ) {

}

void MaterialEditor::TexmatRemoveCallback_real( Fl_Widget* w ) {

}

void MaterialEditor::TexmatEditCallback_real( Fl_Widget* w ) {

}

void MaterialEditor::DyncolAddCallback_real( Fl_Widget* w ) {

}

void MaterialEditor::DyncolRemoveCallback_real( Fl_Widget* w ) {

}

void MaterialEditor::DyncolEditCallback_real( Fl_Widget* w ) {

}