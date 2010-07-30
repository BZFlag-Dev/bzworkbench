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

#include "dialogs/WorldOptionsDialog.h"

#include "model/Model.h"
#include "objects/material.h"

// helper method--concat strings from a vector of strings
string concat(vector<string> strings) {
	string ret = string("");
	for(vector<string>::iterator i = strings.begin(); i != strings.end(); i++) {
		ret += *i + " ";	
	}	
	return ret;
}

// constructor
// The buttons and fields are initialized and placed here.
WorldOptionsDialog::WorldOptionsDialog( world* theWorld, options* theOptions, waterLevel* theWaterLevel ) :
	Fl_Dialog("World Options", WIDTH, HEIGHT, Fl_Dialog::Fl_OK | Fl_Dialog::Fl_CANCEL) {
	
	// initialize the variables
	worldData 		= theWorld;
	optionsData 		= theOptions;
	waterLevelData 	= theWaterLevel;
	
	// parse the values
	string nameStr = worldData->getName();
	float size = worldData->getSize();
	float flagHeight = worldData->getFlagHeight();
	bool noWalls = !worldData->hasWalls();
	
	string options = optionsData->getOptionsString();
	
	float waterHeight = waterLevelData->getHeight();
	material* waterMaterial = waterLevelData->getMaterial();		
		
	// initialize widgets
	worldNameLabel = new QuickLabel("Name:", 5, 5);
	worldNameField = new Fl_Input(100, 5, 200, DEFAULT_TEXTSIZE + 6);
	worldNameField->value(nameStr.c_str());
	
	worldSizeLabel = new QuickLabel("Size:", 5, 35);
	worldSizeField = new Fl_Input(120, 35, 120, DEFAULT_TEXTSIZE + 6);
	worldSizeField->type(FL_FLOAT_INPUT);
	worldSizeField->callback(worldSizeField_cb);
	worldSizeField->when(FL_WHEN_CHANGED);
	worldSizeField->tooltip("float value > 0.0; default is 400.0");
	worldSizeField->value(ftoa(size).c_str());
	
	flagHeightLabel = new QuickLabel("Flag height:", 5, 65);
	flagHeightField = new Fl_Input(120, 65, 120, DEFAULT_TEXTSIZE + 6);
	flagHeightField->type(FL_FLOAT_INPUT);
	flagHeightField->callback(flagHeightField_cb);
	flagHeightField->when(FL_WHEN_CHANGED);
	flagHeightField->tooltip("float value > 0.0; default is 10.0");
	flagHeightField->value(ftoa(flagHeight).c_str());
	
	waterCheckButton = new Fl_Check_Button(5, 95, DEFAULT_TEXTSIZE + 6, DEFAULT_TEXTSIZE + 6, "Water level");
	waterCheckButton->value( waterHeight >= 0.0f ? true : false );
	
	waterLevelField = new Fl_Input(120, 95, 120, DEFAULT_TEXTSIZE + 6);
	waterLevelField->type(FL_FLOAT_INPUT);
	waterLevelField->callback(waterLevelField_cb, waterCheckButton);
	waterLevelField->when(FL_WHEN_CHANGED);
	waterLevelField->tooltip("float value > 0.0; -1 to disable");
	waterLevelField->value(ftoa(waterHeight).c_str());
		
	waterCheckButton->callback(waterCheckButton_cb, waterLevelField);
	
	worldOptionsLabel = new QuickLabel("Options:", 5, 125);
	worldOptionsField = new Fl_Input(100, 125, 200, DEFAULT_TEXTSIZE + 6);
	worldOptionsField->value(options.c_str());
	
	waterTextureLabel = new QuickLabel("Water Material:", 5, 155);
	waterTextureField = new Fl_Input(120, 155, 180, DEFAULT_TEXTSIZE + 6);
	if ( waterMaterial != NULL )
		waterTextureField->value( waterMaterial->getName().c_str() );
	
	noWallsCheckButton = new Fl_Check_Button(5, 185, DEFAULT_TEXTSIZE + 6, DEFAULT_TEXTSIZE + 6, "No Walls");
	noWallsCheckButton->value(noWalls);
	
	// add widgets
	add(worldNameLabel);
	add(worldNameField);
	
	add(worldSizeLabel);
	add(worldSizeField);
	
	add(flagHeightLabel);
	add(flagHeightField);
	
	add(waterCheckButton);
	add(waterLevelField);
	
	add(worldOptionsLabel);
	add(worldOptionsField);
	
	add(waterTextureLabel);
	add(waterTextureField);
	
	add(noWallsCheckButton);
	
	// set event handlers
	setOKEventHandler(OKButtonCallback, this);
	setCancelEventHandler(CancelButtonCallback, this);
}

void WorldOptionsDialog::worldSizeField_cb(Fl_Widget *o, void* data) {
	float f = atof( ((Fl_Input*)o)->value() );
	if(f <= 0.0f)
		((Fl_Input*)o)->value("400.0");
}

void WorldOptionsDialog::flagHeightField_cb(Fl_Widget *o, void* data) {
	float f = atof( ((Fl_Input*)o)->value() );
	if(f <= 0.0f)
		((Fl_Input*)o)->value("10.0");
}

void WorldOptionsDialog::waterLevelField_cb(Fl_Widget *o, void* data) {
	float f = atof( ((Fl_Input*)o)->value() );
	if(f > 0.0f)
		((Fl_Check_Button*)data)->value(true);
	if(f <= 0.0f){
		((Fl_Check_Button*)data)->value(false);
		((Fl_Input*)o)->value("-1.0");
	}
}

void WorldOptionsDialog::waterCheckButton_cb(Fl_Widget *o, void* data) {
	bool b = ((Fl_Check_Button*)o)->value();
	if(b){
		((Fl_Input*)data)->value("1.0");
	}else{
		((Fl_Input*)data)->value("-1.0");
	}
}

WorldOptionsDialog::~WorldOptionsDialog() { 
	
}

// set all variables from the widget values when the OK button is pressed
void WorldOptionsDialog::OKButtonCallback_real(Fl_Widget* w) {
	
	unsigned char hasWater = waterCheckButton->value();
	
	float waterLevel;
	if(hasWater == 0)
		waterLevel = -1.0f;
	else
		waterLevel = atof(waterLevelField->value());
		
	float flagHeight = atof(flagHeightField->value());
	
	string worldName = string(worldNameField->value());
	
	string optionsString = string(worldOptionsField->value());
	
	material* waterMaterialString = dynamic_cast< material* >( Model::command( MODEL_GET, "material", string( waterTextureField->value() ) ) );
	
	float size = atof(worldSizeField->value());
	
	bool noWalls = (noWallsCheckButton->value() != 0);
	
	worldData->setName( worldName.c_str() );
	worldData->setSize( size );
	worldData->setWalls( !noWalls );
	worldData->setFlagHeight(flagHeight);
	
	optionsData->setOptionsString(optionsString);
	
	waterLevelData->setHeight(waterLevel);
	waterLevelData->setMaterial(waterMaterialString);
	
	Fl::delete_widget(this);
}	

// close the window when the Cancel button is pressed
void WorldOptionsDialog::CancelButtonCallback_real(Fl_Widget* w) {

	Fl::delete_widget(this);
}
