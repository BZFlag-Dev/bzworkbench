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

// constructor with data
weapon::weapon(string& data) :
	bz2object("weapon", "<name><rotation><position><color><tilt><initdelay><delay><type><trigger><eventteam>", data.c_str()) {
	setDefaults();

	update(data);
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

// setter
int weapon::update(string& data) {
	const char* header = getHeader().c_str();
	
	// get the chunk of data
	vector<string> lines = BZWParser::getSectionsByHeader(header, data.c_str());
	
	// break if we didn't get only one
	if(lines[0] == BZW_NOT_FOUND)
		return 0;
		
	if(!hasOnlyOne(lines, header))
		return 0;
		
	// get the data
	const char* weaponData = lines[0].c_str();
		
	// get initdelay
	vector<string> initDelayVals = BZWParser::getValuesByKey("initdelay", header, weaponData);
	if(!hasOnlyOne(initDelayVals, "initdelay"))
		return 0;
	
	// get delay
	vector<string> delayVals = BZWParser::getValuesByKey("delay", header, weaponData);
	if(!hasOnlyOne(delayVals, "delay"))
		return 0;
	if(delayVals[0] == "delay") {
		printf("weapon::update(): Error: \"delay\" defined, but no values given!\n");
		return 0;	
	}
		
	// break up the delay values into individual chunks
	vector<string> delayElements = BZWParser::getLineElements(delayVals[0].c_str());
	if(delayElements.size() == 0) {
		printf("weapon::update(): Error! Defined \"delay\" but gave no values!\n");
		return 0;	
	}
		
	// get tilt
	vector<string> tiltVals = BZWParser::getValuesByKey("tilt", header, weaponData);
	if(tiltVals.size() > 1) {
		printf("weapon::update(): Error! Defined \"tilt\" %d times\n", (int)tiltVals.size());
		return 0;
	}
		
	// get type
	vector<string> typeVals = BZWParser::getValuesByKey("type", header, weaponData);
	if(!hasOnlyOne(typeVals, "type"))
		return 0;
	if( !Flag::isFlag(typeVals[0].c_str()) ) {
		printf("weapon::update(): Error: unknown flag value \"%s\"\n", typeVals[0].c_str());
		return 0;	
	}
		
	// get trigger
	vector<string> triggerVals = BZWParser::getValuesByKey("trigger", header, weaponData);
	if(triggerVals.size() > 1) {
		printf("weapon::update(): Error! Defined \"trigger\" %d times\n", (int)tiltVals.size());
		return 0;
	}
	
	if(triggerVals.size() > 0) {
		// only flagcap is supported...
		if( triggerVals[0] != "flagcap" ) {
			printf("weapon::update(): Error: unknown trigger value \"%s\"\n", triggerVals[0].c_str() );
			return 0;	
		}
	}
	
	// get event team
	vector<string> eventTeamVals = BZWParser::getValuesByKey("eventteam", header, weaponData);
	if(eventTeamVals.size() > 1) {
		printf("weapon::update(): Error: Defined \"eventteam\" %d times!\n", (int)eventTeamVals.size());
		return 0;
	}
	if(eventTeamVals.size() > 0) {
		if(! (eventTeamVals[0] == "R" || eventTeamVals[0] == "G" || eventTeamVals[0] == "B" || eventTeamVals[0] == "P") ) {
			printf("weapon::update(): Error: Unknown team %s\n", eventTeamVals[0].c_str());
			return 0;	
		}
	}
	
	// get the color
	vector<string> colorVals = BZWParser::getValuesByKey("color", header, weaponData);
	if(colorVals.size() > 1) {
		printf("weapon::update(): Error! Defined \"color\" %d times!\n", (int)colorVals.size());	
		return 0;
	}
	
	if(colorVals.size() > 0) {
		if(atoi( colorVals[0].c_str() ) < 0 || atoi( colorVals[0].c_str() ) > 4) {
			printf("weapon::update(): Error: unkown team %d\n", atoi( colorVals[0].c_str() ));
			return 0;	
		}
	}
		
	// do base-class update
	if(!bz2object::update(data))
		return 0;
		
	// load the data in
	
	delay.clear();
	for(vector<string>::iterator i = delayElements.begin(); i != delayElements.end(); i++) {
		float val = atof( i->c_str() );
		if(val < 0) {
			printf("weapon::update(): Error! Delay value %f is negative...\n", val);
			return 0;
		}
		delay.push_back(val);
	}
	
	type = typeVals[0];
	trigger = (triggerVals.size() != 0 ? triggerVals[0] : string(""));
	eventTeam = (eventTeamVals.size() > 0 ? atoi( eventTeamVals[0].c_str() ) : 0);
	initdelay = atof( initDelayVals[0].c_str() );
	tilt = (tiltVals.size() != 0 ? atof( tiltVals[0].c_str() ) : 0.0f);
	team = (colorVals.size() > 0 ? atoi( colorVals[0].c_str() ) : 0);
	
	return 1;
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