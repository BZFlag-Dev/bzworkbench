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

#include "dialogs/AdvancedOptionsDialog.h"

#include "objects/bz2object.h"

// main constructor
AdvancedOptionsDialog::AdvancedOptionsDialog( bz2object* _obj ) :
	ConfigurationDialog( _obj, "Advanced Options", DEFAULT_WIDTH, DEFAULT_HEIGHT ) {
	
	obj = _obj;
	
	begin();
	
	tabs = new Fl_Tabs( 5, 5, DEFAULT_WIDTH - 10, 310 );
	

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
	MaterialWidget* mw = new MaterialWidget( materialList->x() + 5, 0, materialList->w() - 10, 2 * DEFAULT_TEXTSIZE, materialRefs, NULL );
	addMaterial( mw );
}

void AdvancedOptionsDialog::AdvancedOptionsPage::removeMaterialCallback_real( Fl_Widget* w ) {
	if(listPack->children() > 0){
		bool r = false;
		for(int i=listPack->children()-1;i>-1;i--){
			MaterialWidget* mw = (MaterialWidget*)listPack->child(i);
			if(mw->isActive()){
				listPack->remove(listPack->child(i));
				r = true;
			}
		}
		if(r)
			materialList->scroll_to(0, 0);
		parent()->redraw();
	}
}

void AdvancedOptionsDialog::AdvancedOptionsPage::addMaterial( MaterialWidget* mw ) {
	if( mw != NULL ) {
		int h = listPack->h() + listPack->spacing() + mw->h();
		listPack->add( mw );
		materialWidgets.push_back( mw );
		listPack->resize(materialList->x()+1,materialList->y()+1,materialList->w()-2,h-2);
		int pos = h - materialList->h();
		if(pos > 0)
			materialList->scroll_to(0, pos + materialList->yposition());
		else
			materialList->scroll_to(0, 0);
		parent()->redraw();
	}
}


AdvancedOptionsDialog::AdvancedOptionsPage::AdvancedOptionsPage(int x, int y, std::string name, std::vector<material*> mats, physics *phys, bool usephydrv ) : Fl_Group( x, y, 380, 300 ) {
	this->name = name;
	this->usephydrv = usephydrv;
	label ( this->name.c_str() );

	begin();

	materialLabel = new QuickLabel("Materials (order matters)", 5 + x, DEFAULT_TEXTSIZE + y);
	materialAdd = new Fl_Button(DEFAULT_WIDTH - 180 + x, 5 + y, 55, DEFAULT_TEXTSIZE + 6, "Add" );
	materialAdd->callback( addMaterialCallback, this );
	materialRemove = new Fl_Button(DEFAULT_WIDTH - 120 + x, 5 + y, 65, DEFAULT_TEXTSIZE + 6, "Remove" );
	materialRemove->callback( removeMaterialCallback, this );
	materialList = new Fl_Scroll( 5 + x, 30 + y, DEFAULT_WIDTH - 30, 200 );
	listPack = new Fl_Pack( 6 + x, 31 + y, DEFAULT_WIDTH - 32, 198);
	listPack->spacing(5);
	materialList->end();
	materialList->box(FL_THIN_DOWN_BOX);
	materialList->type(Fl_Scroll::VERTICAL_ALWAYS);
	
	// add materials
	// first, query the model for them
	materialRefs = vector<string>();
	materialRefs.push_back( MaterialWidget_NONE );
	materialRefs.push_back( "object: ambient" );
	materialRefs.push_back( "object: diffuse" );
	materialRefs.push_back( "object: specular" );
	materialRefs.push_back( "object: emission" );
	materialRefs.push_back( "object: texture" );
	
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
			MaterialWidget* mw = new MaterialWidget( materialList->x() + 5, 0, materialList->w() - 10, 2 * DEFAULT_TEXTSIZE, materialRefs, (*i) );
			if((*i)->getMatType() == material::MAT_AMBIENT){
				mw->setSelectedMaterial( "object: ambient" );
			} else if ((*i)->getMatType() == material::MAT_DIFFUSE){
				mw->setSelectedMaterial( "object: diffuse" );
			} else if ((*i)->getMatType() == material::MAT_SPECULAR){
				mw->setSelectedMaterial( "object: specular" );
			} else if ((*i)->getMatType() == material::MAT_EMISSION){
				mw->setSelectedMaterial( "object: emission" );
			} else if ((*i)->getMatType() == material::MAT_TEXTURE){
				mw->setSelectedMaterial( "object: texture" );
			} else {
				mw->setSelectedMaterial( (*i)->getName() );
			}
			addMaterial( mw );
		}
	}
	
	if (usephydrv) {
		phydrvLabel = new QuickLabel("Physics Driver", 5 + x, 235 + y );
		phydrvMenu = new Fl_Choice( 5 + x, 250 + y, DEFAULT_WIDTH - 40, DEFAULT_TEXTSIZE + 6 );
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
	materialList->scroll_to(0, 0);
	parent()->redraw();
}

void AdvancedOptionsDialog::AdvancedOptionsPage::commitChanges( bz2object* obj ) {
	std::string slot;
	if ( name == "All" )
		slot = "";
	else
		slot = name;

	obj->getMaterials( slot ).clear();

	for ( vector< MaterialWidget* >::iterator i = materialWidgets.begin(); i != materialWidgets.end(); i++ ) {
		string selected = (*i)->getSelectedMaterial();
		material* mat = NULL;
		if(selected.compare("object: ambient") == 0){
			mat = new material();
			mat->setMatType(material::MAT_AMBIENT);
			mat->setAmbient((*i)->getRGBA());
		} else if (selected.compare("object: diffuse") == 0){
			mat = new material();
			mat->setMatType(material::MAT_DIFFUSE);
			mat->setDiffuse((*i)->getRGBA());
		} else if (selected.compare("object: specular") == 0){
			mat = new material();
			mat->setMatType(material::MAT_SPECULAR);
			mat->setSpecular((*i)->getRGBA());
		} else if (selected.compare("object: emission") == 0){
			mat = new material();
			mat->setMatType(material::MAT_EMISSION);
			mat->setEmission((*i)->getRGBA());
		} else if (selected.compare("object: texture") == 0){
			mat = new material();
			mat->setMatType(material::MAT_TEXTURE);
			string tex = (*i)->getText();
			if(tex.length() > 0)
				mat->setTexture(tex);
		} else if(selected.compare(MaterialWidget_NONE) != 0){
			mat = dynamic_cast< material* >( Model::command( MODEL_GET, "material", selected.c_str() ) );
		}
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
