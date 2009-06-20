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

	// material buttons
	materialAddButton = new Fl_Button( 235, 30, 70, DEFAULT_TEXTSIZE + 6, "Add" );
	materialRemoveButton = new Fl_Button( 235, 55, 70, DEFAULT_TEXTSIZE + 6, "Remove" );
	materialEditButton = new Fl_Button( 235, 80, 70, DEFAULT_TEXTSIZE + 6, "Edit" );
	materialEditButton->callback( MaterialEditCallback, this );

	// texture matrix label
	textureMatrixLabel = new QuickLabel( "Texture Matrices: ", 5, 125 );

	// texture matrix browser
	textureMatrixBrowser = new Fl_Multi_Browser( 5, 150, 225, 90 );

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

void MaterialEditor::MaterialEditCallback_real( Fl_Widget* w ) {
	// just temporary code allowing the dialog to be accessed
	MaterialConfigurationDialog* mcd = new MaterialConfigurationDialog( 0 );

	mcd->show();

	while ( mcd->shown() ) { Fl::wait(); }

	delete mcd;
}

