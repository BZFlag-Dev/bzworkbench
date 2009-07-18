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

#include "objects/world.h"

#include "model/Model.h"

world::world() : DataEntry("world", "<name><size><flagHeight><noWalls>") {
	worldName = string("");
	size = 400.0;
	noWalls = false;
	flagHeight = 10.0f;
}

// send the data
string world::get(void) {
	return toString();
}

// bzw methods
bool world::parse( std::string& line ) {
	// check if we reached the end of the section
	if ( line == "end" )
		return false;

	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );

	if ( key == "name" ) {
		worldName = value;
	}
	else if ( key == "size" ) {
		size = atof( value.c_str() );
	}
	else if ( key == "flagheight" ) {
		flagHeight = atof( value.c_str() );
	}
	else if ( key == "nowalls" ) {
		noWalls = true;
	}
	else {
		throw BZWReadError( this, string( "Unknown key, " ) + key );
	}

	return true;
}

void world::finalize() {
	// nothing to do
}

// toString method
string world::toString(void) {
	string sizeString = string(ftoa(size));
	string flagHeightString = string(ftoa(flagHeight));
	return string(string("world\n") +
						 (worldName.length() != 0 ? "  name " + worldName : "# name") + "\n" +
						 "  size " + sizeString + "\n" +
						 "  flagHeight " + flagHeightString + "\n" +
							(noWalls == true ? "  noWalls\n" : "# noWalls\n") +
							getUnusedText() +
						 "end\n");
				  
}

// render method
int world::render(void) { return 0; } 
