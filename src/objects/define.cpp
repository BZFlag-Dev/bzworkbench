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

#include "objects/define.h"

// constructor
define::define() : DataEntry("define", "") {
	objects = vector< osg::ref_ptr<bz2object> >();
	name = "";	
}

// destructor
define::~define() {
	// free previous objects
	objects.clear();
}

// getter
string define::get(void) { return toString(); }

// bzw methods
bool define::parse( std::string& line ) {
	// check if we are at the end of the define
	if ( line == "enddef" )
		return false;

	if ( currentObject ) {
		if ( !currentObject->parse( line ) ) {
			currentObject->finalize();
			objects.push_back( currentObject );
			currentObject = NULL;
		}
	}
	else {
		string header = BZWParser::key( line.c_str() );

		bz2object* obj = dynamic_cast< bz2object* >( Model::buildObject( header.c_str() ) );

		if ( obj != NULL ) {
			obj->parse( line );

			currentObject = obj;
		}
		else
			throw BZWReadError( this, string( "Encountered unknown object type in define, " ) + line );
	}

	
	return true;
}

void define::finalize() {
	// make sure all objects are complete
	if ( currentObject != NULL )
		throw BZWReadError( this, "Incomplete object in define." );
}

// toString
string define::toString(void) {
	string objString = "";
	if(objects.size() > 0) {
		for(vector< osg::ref_ptr< bz2object > >::iterator i = objects.begin(); i != objects.end(); i++) {
			objString += (*i)->toString() + "\n";
		}
	}
	
	return "define " + name + "\n" + objString + "enddef\n";
}
