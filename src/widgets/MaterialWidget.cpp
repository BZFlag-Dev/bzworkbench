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

#include "widgets/MaterialWidget.h"

// copy constructor (does nothing )
MaterialWidget::MaterialWidget( const MaterialWidget& mat ) : Fl_Group( 0, 0, 0, 0 ) { end(); }

// main constructor
MaterialWidget::MaterialWidget( int x, int y, int width, int height, vector< string > materialChoices, material* _mat) :
	Fl_Group( x, y, width, height ) {
		
	end();
	
	mat = _mat;
	// do not resize children when resized
	resizable(NULL);
	activeButton = new Fl_Check_Button(x, y, DEFAULT_TEXTSIZE + 6, DEFAULT_TEXTSIZE + 6);
	materialMenu = new Fl_Menu_Button(x + 30, y, width - 50, DEFAULT_TEXTSIZE + 12);
	rgbaInput = new RGBAWidget(x + 30, y + 30);
	texInput = new Fl_Input(x + 30, y + 30, width - 50, DEFAULT_TEXTSIZE + 6);
	setMaterials( materialChoices );
	setSelectedMaterial( "(none)" );
	add( activeButton );
	add( materialMenu );
	add( rgbaInput );
	add( texInput );
}

// set the current material
void MaterialWidget::setSelectedMaterial( const string& material ) {
	rgbaInput->clear_visible();
	texInput->clear_visible();
	size(w(), 2 * DEFAULT_TEXTSIZE);
	//are we a object material setting, matref or none?
	if (material.compare("object: ambient") == 0){
		if(mat != NULL)
			rgbaInput->setRGBA(mat->getAmbient());
		else
			rgbaInput->setRGBA(RGBA("1 1 1 1"));
		rgbaInput->set_visible();
		materialMenu->label("object: ambient");
		size(w(), 4 * DEFAULT_TEXTSIZE);
		redraw();
		return;
	}
	if (material.compare("object: diffuse") == 0){
		if(mat != NULL)
			rgbaInput->setRGBA(mat->getDiffuse());
		else
			rgbaInput->setRGBA(RGBA("1 1 1 1"));
		rgbaInput->set_visible();
		materialMenu->label("object: diffuse");
		size(w(), 4 * DEFAULT_TEXTSIZE);
		redraw();
		return;
	}
	if (material.compare("object: specular") == 0){
		if(mat != NULL)
			rgbaInput->setRGBA(mat->getSpecular());
		else
			rgbaInput->setRGBA(RGBA("1 1 1 1"));
		rgbaInput->set_visible();
		materialMenu->label("object: specular");
		size(w(), 4 * DEFAULT_TEXTSIZE);
		redraw();
		return;
	}
	if (material.compare("object: emission") == 0){
		if(mat != NULL)
			rgbaInput->setRGBA(mat->getEmission());
		else
			rgbaInput->setRGBA(RGBA("1 1 1 1"));
		rgbaInput->set_visible();
		materialMenu->label("object: emission");
		size(w(), 4 * DEFAULT_TEXTSIZE);
		redraw();
		return;
	}
	if (material.compare("object: texture") == 0){
		if(mat != NULL)
			texInput->value(mat->getTexture(0).c_str());
		else
			texInput->value("caution");
		texInput->set_visible();
		materialMenu->label("object: texture");
		size(w(), 4 * DEFAULT_TEXTSIZE);
		redraw();
		return;
	}
	if( materials.size() > 0 ) {
		unsigned int i = 0;
		for( vector<string>::iterator itr = materials.begin(); itr != materials.end(); i++, itr++ ) {
			if( *itr == material ) {
				materialMenu->label( (*itr).c_str() );
				materialMenu->redraw();
				return;
			}
		}
	}
	else {
		materialMenu->copy_label( MaterialWidget_NONE );
	}
}

// set materials
void MaterialWidget::setMaterials( vector< string >& materialChoices ) {
	// set the new material array, making sure to add "(none)" as an option
	// materials = materialChoices;
	
	materials.push_back( MaterialWidget_NONE );
	materials.push_back( "object: ambient" );
	materials.push_back( "object: diffuse" );
	materials.push_back( "object: specular" );
	materials.push_back( "object: emission" );
	materials.push_back( "object: texture" );
	for( vector< string >::iterator i = materialChoices.begin(); i != materialChoices.end(); i++ ) {
		materials.push_back( i->c_str() );
	}
	
	// erase the current menu
	materialMenu->menu(NULL);
	
	// rebuild the menu button options
	for( vector< string >::iterator i = materials.begin(); i != materials.end(); i++ ) {
		materialMenu->add( i->c_str(), 0, changeMaterialCallback, this );
	}
}

// handle a change in the menu (i.e. just relabel it)
void MaterialWidget::changeMaterialCallback_real( MaterialWidget* mw, Fl_Menu_Button* mb ) {
	const Fl_Menu_Item* menu = mb->menu();
	mw->setSelectedMaterial( menu[mb->value()].label() );
	if(mw->parent() != NULL)
		if(mw->parent()->parent() != NULL)
			mw->parent()->parent()->redraw();
}
