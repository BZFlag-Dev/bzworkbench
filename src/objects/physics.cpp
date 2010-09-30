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

#include "objects/physics.h"

#include "model/SceneBuilder.h"
#include "model/Model.h"

physics::physics() :
	DataEntry("physics", "<name><linear><angular><slide><death>"),
	osg::Referenced() {
	name = SceneBuilder::makeUniqueName( "physics" );
	deathMessage = string("");
	slide = 0.0f;
	linear = Point3D(0.0f, 0.0f, 0.0f);
	angular = Point3D(0.0f, 0.0f, 0.0f);
}

// osg-specific constructor
physics::physics( bool threadSafe ) :
	DataEntry("physics", "<name><linear><angular><slide><death>"),
	osg::Referenced( threadSafe ) {
	name = SceneBuilder::makeUniqueName( "physics" );
	deathMessage = string("");
	slide = 0.0f;
	linear = Point3D(0.0f, 0.0f, 0.0f);
	angular = Point3D(0.0f, 0.0f, 0.0f);
}

// osg-specific constructor
physics::physics( const osg::Referenced& ref ) :
	DataEntry("physics", "<name><linear><angular><slide><death>"),
	osg::Referenced( ref ) {
	name = SceneBuilder::makeUniqueName( "physics" );
	deathMessage = string("");
	slide = 0.0f;
	linear = Point3D(0.0f, 0.0f, 0.0f);
	angular = Point3D(0.0f, 0.0f, 0.0f);
}

// getter
string physics::get(void) { return toString(); }

// bzw methods
bool physics::parse( std::string& line ) {
	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );
	
	// check if we reached the end of the section
	if ( key == "end" )
		return false;

	if ( key == "name" ) {
		name = value;
	}
	else if ( key == "linear" ) {
		linear = Point3D( value.c_str() );
	}
	else if ( key == "angular" ) {
		angular = Point3D( value.c_str() );
	}
	else if ( key == "slide" ) {
		slide = atof( value.c_str() );
	}
	else if ( key == "death" ) {
		deathMessage = value;
	}
	else {
		throw BZWReadError( this, string( "Unknown key, " ) + key );
	}

	return true;
}

void physics::finalize() {
	// nothing to do
}

// toString
string physics::toString(void) {
	return string("physics\n") +
				  "  name " + name + "\n" +
				  "  linear " + linear.toString() +
				  "  angular " + angular.toString() +
				  "  slide " + string(ftoa(slide)) + "\n" +
				  (deathMessage.length() != 0 ? "  death " : "# death") + deathMessage + "\n" +
				  getUnusedText() +
				  "end\n";
}

// render
int physics::render(void) {
	return 0;
}

void physics::setName( const std::string& _name ) {
	if (_name != getName()){
		if ( Model::renamePhysicsDriver( getName(), _name ) ) {
			name = _name;
		}
	}
}
