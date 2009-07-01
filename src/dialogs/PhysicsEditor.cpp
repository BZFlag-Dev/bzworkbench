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

#include "dialogs/PhysicsEditor.h"

#include "dialogs/PhysicsConfigurationDialog.h"
#include "objects/physics.h"
#include "model/SceneBuilder.h"

#include "defines.h"

PhysicsEditor::PhysicsEditor( Model* model ) :
		Fl_Dialog( "Physics Editor", 310, 200, Fl_Dialog::Fl_OK )
{
	this->model = model;

	begin();

	// physics label
	physicsLabel = new QuickLabel( "Physics: ", 5, 5 );

	// physics browser
	physicsBrowser = new Fl_Multi_Browser( 5, 30, 225, 90 );
	refreshPhysicsList();

	// physics buttons
	physicsAddButton = new Fl_Button( 235, 30, 70, DEFAULT_TEXTSIZE + 6, "Add" );
	physicsAddButton->callback( PhysicsAddCallback, this );
	physicsRemoveButton = new Fl_Button( 235, 55, 70, DEFAULT_TEXTSIZE + 6, "Remove" );
	physicsRemoveButton->callback( PhysicsRemoveCallback, this );
	physicsEditButton = new Fl_Button( 235, 80, 70, DEFAULT_TEXTSIZE + 6, "Edit" );
	physicsEditButton->callback( PhysicsEditCallback, this );


	end();

	// add the callbacks
	setOKEventHandler( OKCallback, this );
}

void PhysicsEditor::refreshPhysicsList() {
	physicsBrowser->clear();

	std::map<string, physics*> phydrvs = model->_getPhysicsDrivers();

	std::map<string, physics*>::const_iterator i;
	for ( i = phydrvs.begin(); i != phydrvs.end(); i++ ) {
		physicsBrowser->add( i->first.c_str() );
	}
}

// OK callback
void PhysicsEditor::OKCallback_real( Fl_Widget* w ) {
	// don't delete this dialog box just yet...just hide it
	hide();
}

void PhysicsEditor::PhysicsAddCallback_real( Fl_Widget* w ) {
	// make a new material object
	physics* newObj = dynamic_cast< physics* >( model->_buildObject( "physics" ) );

	if(!newObj)
		return;

	model->_getPhysicsDrivers()[ newObj->getName() ] = newObj;

	// make sure the material shows up
	refreshPhysicsList();
}

void PhysicsEditor::PhysicsRemoveCallback_real( Fl_Widget* w ) {
	const char* name = NULL;
	for (int i = 1; i <= physicsBrowser->size(); i++) {
		if ( physicsBrowser->selected( i ) ) {
			name = physicsBrowser->text( i );
			break;
		}
	}

	if ( name == NULL )
		return;

	map< string, physics* > phydrvs = model->_getPhysicsDrivers();

	if ( phydrvs.count( string( name ) ) ) {
		physics* mat = phydrvs[ string( name ) ];

		model->_removePhysicsDriver( mat );
	}

	refreshPhysicsList();
}

void PhysicsEditor::PhysicsEditCallback_real( Fl_Widget* w ) {
	// find the first selected item
	const char* name = NULL;
	for (int i = 1; i <= physicsBrowser->size(); i++) {
		if ( physicsBrowser->selected( i ) ) {
			name = physicsBrowser->text( i );
			break;
		}
	}

	// make sure something was actually selected
	if ( name == NULL )
		return;

	// get the material
	physics* mat = dynamic_cast< physics* >( model->_command( MODEL_GET, "phydrv", string( name ) ) );

	if ( mat != NULL ) {
		// open a material config dialog
		PhysicsConfigurationDialog* pcd = new PhysicsConfigurationDialog( mat );

		pcd->show();

		while ( pcd->shown() ) { Fl::wait(); }

		// clean up
		delete pcd;
	}

	refreshPhysicsList();
}
