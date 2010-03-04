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

#include "objects/waterLevel.h"

#include "objects/material.h"

waterLevel::waterLevel() : DataEntry("waterLevel", "<name><height><matref>") {
	name = string("water");
	waterMaterial = NULL;
	height = -1.0f;
}

// get method
string waterLevel::get(void) {
	return toString();
}

// bzw methods
bool waterLevel::parse( std::string& line ) {
	// check if we reached the end of the section
	if ( line == "end" )
		return false;

	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );

	if ( key == "name" ) {
		name = value;
	}
	else if ( key == "matref" ) {
		material* mat = (material*)Model::command( MODEL_GET, "material", value );
		if( mat )
			waterMaterial = mat;
		else
			throw BZWReadError( this, string( "Couldn't find material, " ) + value );

	}
	else if ( key == "height" ) {
		height = atof( value.c_str() );
	}
	else {
		throw BZWReadError( this, string( "Unknown key, " ) + key );
	}

	return true;
}

void waterLevel::finalize() {

}

// toString method
string waterLevel::toString(void) {
	string waterLevelHeightStr = string("  height " + ftoa(height) + "\n" );
	string waterLevelMatStr = string("");
	if(waterMaterial != NULL){
		waterLevelMatStr = string("  matref " + waterMaterial->getName() + "\n" );
	}
		
	return string(string("waterLevel\n") +
						 "  name " + name + "\n" +
						 waterLevelHeightStr +
						 waterLevelMatStr +
						 getUnusedText() + "\n" +
						 "end\n");
}


waterLevel::~waterLevel() { }
