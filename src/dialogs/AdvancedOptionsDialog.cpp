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

#include "dialogs/AdvancedOptionsDialog.h"

#include "objects/bz2object.h"

// main constructor
AdvancedOptionsDialog::AdvancedOptionsDialog( bz2object* _obj ) :
	ConfigurationDialog( _obj, "Advanced Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	
	obj = _obj;
	
	begin();
	
	tabs = new Fl_Tabs( 5, 5, DEFAULT_WIDTH - 10, 300 );
	

	vector<string> materialSlots = _obj->materialSlotNames();
	vector<string> physicsSlots = _obj->physicsSlotNames();

	for ( vector<string>::iterator i = materialSlots.begin(); i != materialSlots.end(); i++ ) {
		vector<material*> materials = _obj->getMaterials( *i );
		physics* phys = NULL;
		bool usephys = false;
		for ( vector<string>::iterator j = physicsSlots.begin(); j != physicsSlots.end(); j++ ) {
			if ( *j == *i ) {
				phys = _obj->getPhyDrv( *j ).get();
				usephys = true;
			}
		}

		AdvancedOptionsPage* page;
		if ( *i == "" )
			page = new AdvancedOptionsPage( 10, 35, "All", materials, phys, usephys );
		else
			page = new AdvancedOptionsPage( 10, 35, *i, materials, phys, usephys );

		tabs->add( page );
		tabPages.push_back( page );
	}
	
	end();

	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );
}

// OK callback
void AdvancedOptionsDialog::OKCallback_real( Fl_Widget* w ) {
	for ( vector< AdvancedOptionsPage* >::iterator i = tabPages.begin(); i != tabPages.end(); i++ ) {
		(*i)->commitChanges( obj );
	}

	Fl::delete_widget( this );
}

// Cancel Callback
void AdvancedOptionsDialog::CancelCallback_real( Fl_Widget* w ) {
	Fl::delete_widget( this );
}

// add material callback
void AdvancedOptionsDialog::AdvancedOptionsPage::addMaterialCallback_real( Fl_Widget* w ) {
	// add material
	MaterialWidget* mw = new MaterialWidget( materialList->x() + 5, 0, materialList->w() - 10, 2 * DEFAULT_TEXTSIZE, materialRefs );
	addMaterial( mw );
}

void AdvancedOptionsDialog::AdvancedOptionsPage::removeMaterialCallback_real( Fl_Widget* w ) {
	materialList->remove( materialList->child( materialList->children() ) );
}

void AdvancedOptionsDialog::AdvancedOptionsPage::addMaterial( MaterialWidget* mw ) {
	if( mw != NULL ) {
		int x = materialList->x() + 5;
		int y = materialList->y() + 5 + 2 * DEFAULT_TEXTSIZE * ( materialList->children() - 1 );
		mw->position( x, y );
		materialList->add( mw );
		materialWidgets.push_back( mw );

		redraw();
	}
}


AdvancedOptionsDialog::AdvancedOptionsPage::AdvancedOptionsPage(int x, int y, std::string name, std::vector<material*> mats, physics *phys, bool usephydrv ) : Fl_Group( x, y, 380, 300 ) {
	this->name = name;
	this->usephydrv = usephydrv;
	label ( this->name.c_str() );

	begin();

	materialLabel = new QuickLabel("Materials (order matters)", 5 + x, 5 + y);
	materialAdd = new Fl_Button(DEFAULT_WIDTH - 180 + x, 5 + y, 55, DEFAULT_TEXTSIZE + 6, "Add" );
	materialAdd->callback( addMaterialCallback, this );
	materialRemove = new Fl_Button(DEFAULT_WIDTH - 120 + x, 5 + y, 65, DEFAULT_TEXTSIZE + 6, "Remove" );
	materialList = new Fl_Scroll( 5 + x, 30 + y, DEFAULT_WIDTH - 40, 120 );
	materialList->end();
	materialList->box(FL_UP_BOX);
	materialList->type(Fl_Scroll::VERTICAL_ALWAYS);
	
	// add materials
	// first, query the model for them
	materialRefs = vector<string>();
	materialRefs.push_back( MaterialWidget_NONE );
	
	map< string, osg::ref_ptr< material > > materialMap = Model::getMaterials();
	if( materialMap.size() > 0 ) {
		for( map< string, osg::ref_ptr< material > >::iterator i = materialMap.begin(); i != materialMap.end(); i++ ) {
			materialRefs.push_back( i->first );
		}
	}
	
	// second, add the widgets
	materialWidgets = vector< MaterialWidget* >();
	if( mats.size() > 0 ) {
		for( vector< material* >::iterator i = mats.begin(); i != mats.end(); i++ ) {
			MaterialWidget* mw = new MaterialWidget( materialList->x() + 5, 0, materialList->w() - 10, 2 * DEFAULT_TEXTSIZE, materialRefs );
			mw->setSelectedMaterial( (*i)->getName() );
			addMaterial( mw );
		}
	}
	
	if (usephydrv) {
		phydrvLabel = new QuickLabel("Physics Driver", 5 + x, 190 + y );
		phydrvMenu = new Fl_Choice( 5 + x, 215 + y, DEFAULT_WIDTH - 40, DEFAULT_TEXTSIZE + 6 );
		phydrvMenu->add( "(none)" );
		phydrvMenu->value( 0 );
		map< string, osg::ref_ptr< physics > > phydrvs = Model::getPhysicsDrivers();
		for (map< string, osg::ref_ptr< physics > >::iterator i = phydrvs.begin(); i != phydrvs.end(); i++ ) {
			int idx = phydrvMenu->add( i->first.c_str() );
			if ( phys == i->second ) {
				phydrvMenu->value( idx );
			}
		}
	}

	end();
}

void AdvancedOptionsDialog::AdvancedOptionsPage::commitChanges( bz2object* obj ) {
	std::string slot;
	if ( name == "All" )
		slot = "";
	else
		slot = name;

	obj->getMaterials( slot ).clear();

	for ( vector< MaterialWidget* >::iterator i = materialWidgets.begin(); i != materialWidgets.end(); i++ ) {
		material* mat = dynamic_cast< material* >( Model::command( MODEL_GET, "material", (*i)->getSelectedMaterial().c_str() ) );

		if ( mat != NULL ) {
			obj->addMaterial( mat, slot );
		}
	}

	if ( usephydrv ) {
		string physname( phydrvMenu->text() );
		if ( physname == "(none)" )
			obj->setPhyDrv( NULL, slot );
		else {
			physics* phys = dynamic_cast< physics* >( Model::command( MODEL_GET, "phydrv", physname.c_str() ) );

			if ( phys != NULL )
				obj->setPhyDrv( phys, slot );
		}
	}
}
