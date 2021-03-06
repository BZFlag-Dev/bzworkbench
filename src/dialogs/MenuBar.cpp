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

#include "dialogs/MenuBar.h"
#include "dialogs/MaterialEditor.h"
#include "dialogs/PhysicsEditor.h"
#include "dialogs/DefineEditor.h"
#include "dialogs/RenameDialog.h"
#include "model/Model.h"
#include "commonControls.h"

#include "objects/base.h"
#include "objects/group.h"
#include "objects/teleporter.h"
#include "objects/define.h"

#include "dialogs/InfoConfigurationDialog.h"
#include "dialogs/GroupConfigurationDialog.h"

#include "OSFile.h"

void MenuBar::buildMenu(void) {

	add("File", 0, 0, 0, FL_SUBMENU);
		add("File/New...", FL_CTRL + 'n', new_world, this);
		add("File/Open...", FL_CTRL + 'o', open_world, this);
		add("File/Save", FL_CTRL + 's', save_world, this);
		add("File/Save As...", 0, save_world_as, this, FL_MENU_DIVIDER);
		//add("File/Save Selection...", 0, save_selection, this, FL_MENU_DIVIDER);
		add("File/Exit", 0, exit_bzwb, this);

	add("Edit", 0, 0, 0, FL_SUBMENU);
		//add("Edit/Undo", FL_CTRL + 'z', undo, this);
		//add("Edit/Redo", FL_CTRL + 'y', redo, this, FL_MENU_DIVIDER);
		add("Edit/Cut", FL_CTRL + 'x', cut, this);
		add("Edit/Copy", FL_CTRL + 'c', copy, this);
		add("Edit/Paste", FL_CTRL + 'v', paste, this);
		//add("Edit/Paste Saved Selection...", 0, paste_saved_selection, this);
		add("Edit/Duplicate", FL_CTRL + 'd', duplicate, this);
		add("Edit/Delete", FL_Delete, delete_callback, this, FL_MENU_DIVIDER);
		add("Edit/Select All", FL_CTRL + 'a', select_all, this);
		add("Edit/Unselect All", FL_CTRL + FL_ALT + 'a', unselect_all, this);

	add("Objects", 0, 0, 0, FL_SUBMENU);
		add("Objects/Add box", FL_CTRL+'b', addBoxCallback, this);
		add("Objects/Add pyramid", FL_CTRL+'p', addPyramidCallback, this);
		add("Objects/Add mesh Box", FL_CTRL+FL_ALT+'b', addMeshboxCallback, this);
		add("Objects/Add mesh pyramid", FL_CTRL+FL_ALT+'p', addMeshpyrCallback, this);
		add("Objects/Add teleporter", FL_CTRL+'t', addTeleporterCallback, this);
		add("Objects/Add sphere", 0, addSphereCallback, this);
		add("Objects/Add arc", 0, addArcCallback, this);
		add("Objects/Add mesh", FL_CTRL+'m', addMeshCallback, this);
		add("Objects/Add cone", 0, addConeCallback, this);
		add("Objects/Add zone", 0, addZoneCallback, this);
		add("Objects/Add group", 0, addGroupCallback, this);
		add("Objects/Add base", 0, 0, 0, FL_SUBMENU | FL_MENU_DIVIDER);
			add("Objects/Add base/Add Purple Base", 0, addPurpleBaseCallback, this);
			add("Objects/Add base/Add Red Base", 0, addRedBaseCallback, this);
			add("Objects/Add base/Add Green Base", 0, addGreenBaseCallback, this);
			add("Objects/Add base/Add Blue Base", 0, addBlueBaseCallback, this);

	add("Scene", 0, 0, 0, FL_SUBMENU );
		add("Scene/Import object...", 0, importObjectCallback, this, FL_MENU_DIVIDER);

		add("Scene/Make Define From Selection", FL_CTRL+'g', groupCallback, this);
		add("Scene/Define Editor...", 0, defineCallback, this, FL_MENU_DIVIDER);

		add("Scene/Configure World...", 0, configureWorldCallback, this);
		add("Scene/Configure Object...", FL_CTRL+'o', configureObjectCallback, this);
		add("Scene/Configure Info...", 0, configureInfoCallback, this, FL_MENU_DIVIDER);

		add("Scene/Material Editor...", 0, materialEditorCallback, this);
		add("Scene/Physics Editor...", 0, physicsEditorCallback, this, FL_MENU_DIVIDER);

		add("Scene/Define World Weapon...", FL_CTRL+'w', worldWeaponCallback, this);
		add("Scene/Link Teleporters", 0, linkCallback, this);
}

// constructor
MenuBar::MenuBar( MainWindow* mw ) : Fl_Menu_Bar(0, 0, mw->w(), 30) {
	this->parent = mw;
	printf("MenuBar: parent mw addr: %p\n", parent);
	printf("MenuBar: parent mw model addr: %p\n", parent->getModel());
	this->buildMenu();
}

void MenuBar::new_world_real( Fl_Widget* w ) {
	Model* model = this->parent->getModel();
	model->_newWorld();

	// configure the new world
	WorldOptionsDialog* wod = new WorldOptionsDialog( model->_getWorldData(),
													  model->_getOptionsData(),
													  model->_getWaterLevelData());
	wod->show();

	// wait for configuration to end
	while( wod->shown() ) { Fl::wait(); }

	// reset the world
	ObserverMessage obs(ObserverMessage::UPDATE_WORLD, model->_getWorldData() );
	model->notifyObservers( &obs );
}

void MenuBar::open_world_real( Fl_Widget* w ) {

	string filename ;

	if (!callOpenFileDialog(filename,"*.bzw",FindShareFile(""),"*.bzw","Open..."))
		return;

	/*Fl_File_Chooser* fc = new Fl_File_Chooser("share/", "*.bzw", Fl_File_Chooser::SINGLE, "Open..." );
	fc->value("*.bzw");
	fc->show();

	// wait for a value
	while( fc->shown() ) { Fl::wait(); }

	// get a value (the selected filename )
	if(fc->value() == NULL)
		return;

	string filename = fc->value(); */

	printf("file: %s\n", filename.c_str());

	// invoke BZWParser and load it
	bool success = BZWParser::loadFile( filename.c_str() );

	if (!success) {
		parent->error( parent->getModel()->getErrors().c_str() );
	}
}

void MenuBar::save_world_real( Fl_Widget* w ) {
	// do a "save as..." if the world is yet unnamed
	if( parent->getWorldName().size() <= 0 ) {
		save_world_as_real( w );
		return;
	}

	// save the world
	string path = parent->getWorldName();

	// do a world save
	do_world_save( path.c_str() );
}

void MenuBar::save_world_as_real( Fl_Widget* w ) {

	string filename;
	//this is a save as 
	string newName = "";
	if(parent->getWorldName().compare("") == 0){
		newName = "Untitled.bzw";
	}else{
		newName = parent->getWorldName();
	}

	if (!callSaveFileDialog(filename,newName.c_str(),FindShareFile(""),"*.bzw","Save As..."))
		return;

	// set the world name
	parent->setWorldName( filename.c_str() );

	// save the world
	string path = parent->getWorldName();

	// do the world save
	do_world_save( path.c_str() );
}

// save the world
void MenuBar::do_world_save( const char* filename ) {

	ofstream fileOutput( filename );

	// if we can't open a new file (access permissions, etc), then bail
	if(!fileOutput.is_open()) {
		parent->error( TextUtils::format("Could not open %s for writing\n", filename).c_str() );
		return;
	}

	string text = parent->getModel()->toString();

	fileOutput.write( text.c_str(), text.size() );
	fileOutput.close();

}

void MenuBar::save_selection_real( Fl_Widget* w ) {

}

void MenuBar::exit_bzwb_real( Fl_Widget* w ) {
	while (Fl::first_window())
		Fl::first_window()->hide();
}

void MenuBar::undo_real( Fl_Widget* w ) {

}

void MenuBar::redo_real( Fl_Widget* w ) {

}

void MenuBar::cut_real( Fl_Widget* w ) {
	parent->getModel()->_cutSelection();
}

void MenuBar::copy_real( Fl_Widget* w ) {
	parent->getModel()->_copySelection();
}

void MenuBar::paste_real( Fl_Widget* w ) {
	parent->getModel()->_pasteSelection();
}

void MenuBar::paste_saved_selection_real( Fl_Widget* w ) {

}

void MenuBar::select_all_real( Fl_Widget* w ) {
	parent->getModel()->selectAll();
}

void MenuBar::unselect_all_real( Fl_Widget* w ) {
	parent->getModel()->unselectAll();
}

// add a box
void MenuBar::addBoxCallback_real(Fl_Widget* w) {
	makeObject( "box" );
	value(0);
}

// add a pyramid
void MenuBar::addPyramidCallback_real(Fl_Widget* w) {
	makeObject( "pyramid" );
	value(0);
}

// add a meshbox
void MenuBar::addMeshboxCallback_real(Fl_Widget* w) {
	makeObject( "meshbox" );
	value(0);
}

// add a mesh pyramid
void MenuBar::addMeshpyrCallback_real(Fl_Widget* w) {
	makeObject( "meshpyr" );
	value(0);
}

// add a teleporter
void MenuBar::addTeleporterCallback_real(Fl_Widget* w) {
	makeObject( "teleporter" );
	value(0);
}

// add a sphere
void MenuBar::addSphereCallback_real(Fl_Widget* w) {
	makeObject( "sphere" );
	value(0);
}

// add an arc
void MenuBar::addArcCallback_real(Fl_Widget* w) {
	makeObject( "arc" );
	value(0);
}

// add a mesh
void MenuBar::addMeshCallback_real(Fl_Widget* w) {
	makeObject( "mesh" );
	value(0);
}

// add a cone
void MenuBar::addConeCallback_real(Fl_Widget* w) {
	makeObject( "cone" );
	value(0);
}

// add a zone
void MenuBar::addZoneCallback_real(Fl_Widget* w) {
	makeObject( "zone" );
	value(0);
}

// add a group
void MenuBar::addGroupCallback_real(Fl_Widget* w) {
	// first make sure there is at least one define
	if ( parent->getModel()->getGroups().size() <= 0 ) {
		parent->error( "Must have at least one define to make a group!" );
		return;
	}

	group* grp = dynamic_cast< group* >( makeObject( "group" ) );

	if ( grp != NULL ) {
		// should configure the group since it will be selected after creation
		GroupConfigurationDialog* gcd = new GroupConfigurationDialog( grp );

		gcd->show();

		while ( gcd->shown() ) { Fl::wait(); }

		delete gcd;
	}

	value(0);
}

// import an object
void MenuBar::importObjectCallback_real(Fl_Widget* w) {
	printf("imported an object\n");
	value(0);
}

// add base 1
void MenuBar::addPurpleBaseCallback_real(Fl_Widget* w) {
	base* newObj = dynamic_cast< base* >( makeObject( "base" ) );

	// set the base
	if (newObj)
		newObj->setTeam( BASE_PURPLE );

	value(0);
}

// add base 2
void MenuBar::addRedBaseCallback_real(Fl_Widget* w) {
	base* newObj = dynamic_cast< base* >( makeObject( "base" ) );

	// set the base
	if (newObj)
		newObj->setTeam( BASE_RED );

	value(0);
}

// add base 3
void MenuBar::addGreenBaseCallback_real(Fl_Widget* w) {
	base* newObj = dynamic_cast< base* >( makeObject( "base" ) );

	// set the base
	if (newObj)
		newObj->setTeam( BASE_GREEN );

	value(0);
}

// add base 4
void MenuBar::addBlueBaseCallback_real(Fl_Widget* w) {
	base* newObj = dynamic_cast< base* >( makeObject( "base" ) );

	// set the base
	if (newObj)
		newObj->setTeam( BASE_BLUE );

	value(0);
}

// handle selection clone
void MenuBar::duplicate_real(Fl_Widget* w) {
	parent->getModel()->_copySelection();
	parent->getModel()->_pasteSelection();

	value(0);
}

// handle deletion
void MenuBar::delete_real(Fl_Widget* w) {
	parent->getView()->getSelectHandler()->clearLastSelected();
	parent->getModel()->_deleteSelection();
	value(0);
}

// handle (un)grouping
void MenuBar::groupCallback_real(Fl_Widget* w) {
	// get the selection
	Model::objRefList objects = this->parent->getModel()->_getSelection();

	if( objects.size() < 0 )
		return;

	// make a define and add it to the model
	define* def = new define();
	parent->getModel()->_getGroups()[ def->getName() ] = def;

	// clone and add selected objects to define
	vector< osg::ref_ptr< bz2object > > newObjs;
	for ( Model::objRefList::iterator i = objects.begin(); i != objects.end(); i++ ) {
		newObjs.push_back( SceneBuilder::cloneBZObject( (*i).get() ) );
	}
	def->setObjects( newObjs );

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

	value(0);
}

void MenuBar::defineCallback_real(Fl_Widget* w) {
	// configure the new world
	DefineEditor* wod = new DefineEditor( parent->getModel() );
	wod->show();

	// wait for configuration to end
	while( wod->shown() ) { Fl::wait(); }

	value(0);
}

// handle (un)hiding
void MenuBar::hideCallback_real(Fl_Widget* w) {
	printf("(un)hid\n");
	value(0);
}

// handle save selection
void MenuBar::saveSelectionCallback_real(Fl_Widget* w) {
	printf("saved selection\n");
	value(0);
}

// handle configure world
void MenuBar::configureWorldCallback_real(Fl_Widget* w) {
	// configure the new world
	WorldOptionsDialog* wod = new WorldOptionsDialog( parent->getModel()->_getWorldData(),
													  parent->getModel()->_getOptionsData(),
													  parent->getModel()->_getWaterLevelData());
	wod->show();

	// wait for configuration to end
	while( wod->shown() ) { Fl::wait(); }

	// reset the world
	ObserverMessage obs(ObserverMessage::UPDATE_WORLD, parent->getModel()->_getWorldData() );
	parent->getModel()->notifyObservers( &obs );
	value(0);
}

void MenuBar::materialEditorCallback_real(Fl_Widget* w) {
	MaterialEditor* ed = new MaterialEditor( parent );

	ed->show();

	while( ed->shown() ) { Fl::wait(); }

	delete ed;

	value(0);
}

void MenuBar::physicsEditorCallback_real(Fl_Widget* w) {
	PhysicsEditor* ed = new PhysicsEditor( parent->getModel() );

	ed->show();

	while( ed->shown() ) { Fl::wait(); }

	delete ed;

	value(0);
}

// handle world weapon creation
void MenuBar::worldWeaponCallback_real(Fl_Widget* w) {
	makeObject( "weapon" );
	value(0);
}

// handle object configuration
void MenuBar::configureObjectCallback_real(Fl_Widget* w) {
	// launch a MasterConfigurationDialog
	Model::objRefList selection = parent->getModel()->_getSelection();
	if( selection.size() > 1 || selection.size() == 0 )
		return;

	bz2object* obj = selection[0].get();
	MasterConfigurationDialog* mcd = new MasterConfigurationDialog( obj );
	mcd->show();

	if( parent->getModel()->_hasInitializer( obj ) )
		mcd->setAdditionalConfiguration( parent->getModel()->_configureObject( obj ) );

	value(0);
}

void MenuBar::configureInfoCallback_real(Fl_Widget* w) {
	InfoConfigurationDialog* icd = new InfoConfigurationDialog( parent->getModel()->_getInfoData() );
	icd->show();

	// wait for configuration to end
	while( icd->shown() ) { Fl::wait(); }

	value(0);
}

// handle teleporter linking
void MenuBar::linkCallback_real(Fl_Widget* w) {
	// get all selected objects
	Model::objRefList selection = this->parent->getModel()->_getSelection();
	if( selection.size() <= 0 )
		return;

	// map of teleporter links to create
	map< teleporter*, teleporter* > teleporterMap;
	for( Model::objRefList::iterator i = selection.begin(); i != selection.end(); i++ ) {
		if( (*i)->getHeader() == "teleporter" ) {
			teleporter* t1 = dynamic_cast< teleporter* > (i->get());
			if( !t1 )
				continue;

			// get other teleporters
			if( selection.size() >= 2 ) {
				for( Model::objRefList::iterator j = i+1; j != selection.end(); j++ ) {

					if( (*j)->getHeader() == "teleporter" ) {
						teleporter* t2 = dynamic_cast< teleporter* >( j->get() );
						if( !t2 )
							continue;

						teleporterMap[ t1 ] = t2;
					}
				}
			}
		}
	}

	// make the links
	if( teleporterMap.size() > 0 ) {
		for( map< teleporter*, teleporter* >::iterator i = teleporterMap.begin(); i != teleporterMap.end(); i++ ) {
			parent->getModel()->_linkTeleporters( i->first, i->second );
		}
	}

	value(0);
}

bz2object* MenuBar::makeObject( const char* objectName ) {
	// make a new box using the Model's object registry
	DataEntry* newBox = this->parent->getModel()->_buildObject( objectName );

	// make it into a bz2object
	bz2object* newObj = dynamic_cast< bz2object* >( newBox );

	if(!newObj)
		return NULL;

	// add the object to the model
	parent->getModel()->_unselectAll();
	parent->getModel()->_addObject( newObj );
	parent->getModel()->_setSelected( newObj );

	return newObj;
}
