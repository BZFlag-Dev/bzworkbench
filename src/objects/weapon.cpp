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

#include "objects/weapon.h"

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Vec3>
#include <osg/PolygonMode>

// default constructor
weapon::weapon() : 
	bz2object("weapon", "<name><rotation><position><color><tilt><initdelay><delay><type><trigger><eventteam>") {
	setDefaults();
}

void weapon::setDefaults() {
	type = string("");
	trigger = string("");
	eventTeam = 0;
	tilt = 0.0f;
	initdelay = 0.0f;
	delay = vector<float>();
	team = 0;
	setSize( osg::Vec3( 5, 5, 5 ) );
	
	updateGeometry();
}

// getter
string weapon::get(void) { return toString(); }

// bzw methods
bool weapon::parse( std::string& line ) {
	// check if we reached the end of the section
	if ( line == "end" )
		return false;

	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );

	// parse keys
	if ( key == "initdelay" ) {
		initdelay = atof( value.c_str() );
	}
	else if ( key == "delay" ) {
		vector<string> delayElements = BZWParser::getLineElements(value.c_str());
		if(delayElements.size() == 0)
			throw BZWReadError( this, "Defined \"delay\" but gave no values!" );

		delay.clear();
		for(vector<string>::iterator i = delayElements.begin(); i != delayElements.end(); i++) {
			float val = atof( i->c_str() );
			if(val < 0)
				throw BZWReadError( this, string( "Delay value is negative..." ) + value);
			delay.push_back(val);
		}
	}
	else if ( key == "tilt" ) {
		tilt = atof( value.c_str() );
	}
	else if ( key == "type" ) {
		type = value;
	}
	else if ( key == "trigger" ) {
		trigger = value;
	}
	else if ( key == "eventteam" ) {
		eventTeam = atoi( value.c_str() );
	}
	else if ( key == "color" ) {
		team = atoi( value.c_str() );
	}
	else {
		return bz2object::parse( line );
	}

	return true;
}

void weapon::finalize() {
	bz2object::finalize();
}

// toString
string weapon::toString(void) {
	// need to string-ify the delay loop
	string delayString = string("");
	if(delay.size() > 0) {
		for(vector<float>::iterator i = delay.begin(); i != delay.end(); i++) {
			delayString += string(ftoa(*i)) + " ";
		}
		delayString += "\n";
	}
	
	return string("weapon\n") +
				  BZWLines( this ) +
				  (type.length() == 0 ? "# type\n" : "  type " + type + "\n") +
				  (trigger.length() == 0 ? "# trigger\n" : "  trigger " + trigger + "\n") +
				  (eventTeam != 0 ? "# eventteam\n" : "  eventteam " + string( itoa( eventTeam ) ) + "\n") +
				  "  initdelay " + string(ftoa(initdelay)) + "\n" +
				  "  tilt " + string(ftoa(tilt)) + "\n" +
				  "  color " + string(ftoa(team)) + "\n" +
				  (delayString.length() == 0 ? "# delay\n" : "  delay " + delayString) +
				  "end\n";
}

// render
int weapon::render(void) {
	return 0;
}

void weapon::updateGeometry() {
	osg::Geode* geode = new osg::Geode();
	osg::Geometry* geom = new osg::Geometry();
	osg::Vec3Array* vertices = new osg::Vec3Array();

	vertices->push_back( osg::Vec3( 0, 0, 1 ) ); // top
	vertices->push_back( osg::Vec3( 1, 0, 0 ) ); // +x
	vertices->push_back( osg::Vec3( 0, 1, 0 ) ); // +y
	vertices->push_back( osg::Vec3( -1, 0, 0 ) ); // -x
	vertices->push_back( osg::Vec3( 0, -1, 0 ) ); // -y
	vertices->push_back( osg::Vec3( 0, 0, -1 ) ); // bottom

	osg::DrawElementsUInt* indicesTop = new osg::DrawElementsUInt( osg::DrawElements::TRIANGLE_FAN, 0 );
	indicesTop->push_back( 0 );
	indicesTop->push_back( 1 );
	indicesTop->push_back( 2 );
	indicesTop->push_back( 3 );
	indicesTop->push_back( 4 );

	osg::DrawElementsUInt* indicesBottom = new osg::DrawElementsUInt( osg::DrawElements::TRIANGLE_FAN, 0 );
	indicesBottom->push_back( 5 );
	indicesBottom->push_back( 1 );
	indicesBottom->push_back( 2 );
	indicesBottom->push_back( 3 );
	indicesBottom->push_back( 4 );

	geode->addDrawable( geom );
	geom->setVertexArray( vertices );
	geom->addPrimitiveSet( indicesTop );
	geom->addPrimitiveSet( indicesBottom );

	// make wireframe material
	osg::StateSet* stateset = geode->getOrCreateStateSet();
	osg::PolygonMode* polyMode = new osg::PolygonMode();
	polyMode->setMode( osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE );
	stateset->setAttribute( polyMode, osg::StateAttribute::ON);

	geode->setStateSet( stateset );

	setThisNode( geode );
}