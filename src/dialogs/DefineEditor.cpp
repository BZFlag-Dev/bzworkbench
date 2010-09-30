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

#include "dialogs/DefineEditor.h"

#include "dialogs/RenameDialog.h"
#include "objects/define.h"
#include "model/SceneBuilder.h"

#include "defines.h"

DefineEditor::DefineEditor( Model* model ) :
		Fl_Dialog( "Define Editor", 310, 200, Fl_Dialog::Fl_OK )
{
	this->model = model;

	begin();

	// physics label
	defineLabel = new QuickLabel( "Defines: ", 5, 5 );

	// physics browser
	defineBrowser = new Fl_Multi_Browser( 5, 30, 225, 90 );
	refreshDefineList();

	// physics buttons
	removeButton = new Fl_Button( 235, 55, 70, DEFAULT_TEXTSIZE + 6, "Remove" );
	removeButton->callback( RemoveCallback, this );
	editButton = new Fl_Button( 235, 80, 70, DEFAULT_TEXTSIZE + 6, "Edit" );
	editButton->callback( EditCallback, this );


	end();

	// add the callbacks
	setOKEventHandler( OKCallback, this );
}

void DefineEditor::refreshDefineList() {
	defineBrowser->clear();

	std::map<string, define* > defs = model->_getGroups();

	std::map<string, define* >::const_iterator i;
	for ( i = defs.begin(); i != defs.end(); i++ ) {
		defineBrowser->add( i->first.c_str() );
	}
}

// OK callback
void DefineEditor::OKCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}

void DefineEditor::RemoveCallback_real( Fl_Widget* w ) {
	const char* name = NULL;
	for (int i = 1; i <= defineBrowser->size(); i++) {
		if ( defineBrowser->selected( i ) ) {
			name = defineBrowser->text( i );
			break;
		}
	}

	if ( name == NULL )
		return;

	map< string, define* > defs = model->_getGroups();

	if ( defs.count( string( name ) ) ) {
		define* def = defs[ string( name ) ];

		model->_removeGroup( def );
	}

	refreshDefineList();
}

void DefineEditor::EditCallback_real( Fl_Widget* w ) {
	// find the first selected item
	const char* name = NULL;
	for (int i = 1; i <= defineBrowser->size(); i++) {
		if ( defineBrowser->selected( i ) ) {
			name = defineBrowser->text( i );
			break;
		}
	}

	// make sure something was actually selected
	if ( name == NULL )
		return;

	// get the material
	define* def = dynamic_cast< define* >( model->_command( MODEL_GET, "define", string( name ) ) );

	if ( def != NULL ) {
		// make a rename dialog and show it
		RenameDialog* dialog = new RenameDialog();
		dialog->setName( def->getName() );
		dialog->show();

		// wait until it is closed
		while( dialog->shown() ) { Fl::wait(); }

		if ( !dialog->getCancelled() ) {
			def->setName( dialog->getName() );
		}

		delete dialog;
	}

	refreshDefineList();
}
