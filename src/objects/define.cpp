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

#include "objects/define.h"

// constructor
define::define() : DataEntry("define", "") {
	objects = vector< osg::ref_ptr<bz2object> >();
	name = SceneBuilder::makeUniqueName("define");
	currentObject = NULL;
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
	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );
	
	// check if we reached the end of the section
	if ( key == "enddef" )
		return false;

	if ( currentObject ) {
		if ( !currentObject->parse( line ) ) {
			currentObject->finalize();
			objects.push_back( currentObject );
			currentObject = NULL;
		}
	}
	else if ( key == "define" ) {
		name = value;
	}
	else {
		bz2object* obj = dynamic_cast< bz2object* >( Model::buildObject( key.c_str() ) );

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

void define::setName( const string& _name ) {
	if (_name != getName()){
		if ( Model::renameGroup( name, _name ) ) {
			this->name = _name; 
		}
	}
}