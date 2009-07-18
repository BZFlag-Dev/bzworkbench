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

#include "objects/link.h"

// constructor
Tlink::Tlink() : bz2object("link", "<name><from><to>") {
	Object::setName( SceneBuilder::makeUniqueName( "link" ) );
	from = NULL;
	to = NULL;
}

// getter
string Tlink::get(void) { return toString(); }

// bzw methods
bool Tlink::parse( std::string& line ) {
	// check if we reached the end of the section
	if ( line == "end" )
		return false;

	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );

	// parse keys
	if ( key == "from" ) {
		from = dynamic_cast< teleporter* > (Model::command( MODEL_GET, "teleporter", value ));
	}
	else if ( key == "to" ) {
		to = dynamic_cast< teleporter* > (Model::command( MODEL_GET, "teleporter", value ));
	}
	else {
		return bz2object::parse( line );
	}

	return true;
}

void Tlink::finalize() {
	buildGeometry();
	bz2object::finalize();
}

// toString
string Tlink::toString(void) {
	string fromName = (from == NULL ? "# from:(unknown)\n" : "  from " + from->getName() + "\n");
	string toName = (to == NULL ? "# to:(unknown)\n" : "  to " + to->getName() + "\n" );
	
	return string("link\n") +
				  (getName().length() != 0 ? "  name " + getName() : "# name") + "\n" +
				  fromName + 
				  toName + 
				  "end\n";
}

// build the link geometry
void Tlink::buildGeometry() {
	// don't draw links if there aren't defined "from" or "to" values
	if( from == NULL || to == NULL )
		return;
		
	// basically, make a yellow line between the teleporters
	osg::ref_ptr< osg::Cylinder > line;
	
	// get the positions of the teleporters
	osg::Vec3 fromPos = from->getPos();
	osg::Vec3 toPos = to->getPos();
	
	// raise the positions to the top of the teleporters
	fromPos.set( fromPos.x(), fromPos.y(), fromPos.z() + from->getSize().z() );
	toPos.set( toPos.x(), toPos.y(), toPos.z() + to->getSize().z() );
	
	
}
