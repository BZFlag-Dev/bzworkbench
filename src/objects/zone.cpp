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

#include "objects/zone.h"

#include <osg/PolygonMode>

// constructor
zone::zone() : bz2object("zone", "<name><position><size><rotation><team><flag><safety><zoneflag>") {
	setDefaults();
}

void zone::setDefaults() {
	updateGeometry();

	teams = vector<int>();
	safety = vector<int>();
	setSize( osg::Vec3( 10, 10, 10 ) );
}

// getter
string zone::get(void) { return toString(); }

int zone::update( UpdateMessage& message ) {
	switch( message.type ) {
		case UpdateMessage::SET_POSITION: 	// handle a new position
			setPos( *(message.getAsPosition()) );
			break;

		case UpdateMessage::SET_POSITION_FACTOR:	// handle a translation
			setPos( getPos() + *(message.getAsPositionFactor()) );
			break;

		case UpdateMessage::SET_ROTATION:		// handle a new rotation
			setRotationZ( message.getAsRotation()->z() );
			break;

		case UpdateMessage::SET_ROTATION_FACTOR:	// handle an angular translation
			setRotationZ( getRotation().z() + message.getAsRotationFactor()->z() );
			break;

		case UpdateMessage::SET_SCALE:		// handle a new scale
			setSize( *(message.getAsScale()) );
			break;

		case UpdateMessage::SET_SCALE_FACTOR:	// handle a scaling factor
			setSize( getSize() + *(message.getAsScaleFactor()) );
			break;

		default:	// unknown event; don't handle
			return 0;
	}

	return 1;
}

// bzw methods
bool zone::parse( std::string& line ) {
	// check if we reached the end of the section
	if ( line == "end" )
		return false;

	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );

	// parse keys
	if ( key == "team" ) {
		vector<string> teamElements = BZWParser::getLineElements( value.c_str() );
		
		// candidate teams
		vector<int> teamCandidates = vector<int>();

		// determine whether or not the given teams are valid (throw a error if invalid)
		for(vector<string>::iterator i = teamElements.begin(); i != teamElements.end(); i++) {
			int teamNumber = atoi( i->c_str() );
			if(teamNumber < 0 || teamNumber > 4) {
				throw BZWReadError( this, string( "Undefined team, " ) + value);
			}
			teamCandidates.push_back(teamNumber);
		}

		teams = teamCandidates;
	}
	else if ( key == "safety" ) {
		vector<string> safetyElements = BZWParser::getLineElements( value.c_str() );

		// candidate safeties
		vector<int> safetyCandidates = vector<int>();

		// determine whether or not the given safety values are valid (throw a warning if not)
		if(safetyElements.size() > 0) {
			for(vector<string>::iterator i = safetyElements.begin(); i != safetyElements.end(); i++) {
				int teamNumber = atoi( i->c_str() );
				if(teamNumber < 0 || teamNumber > 4) {
					throw BZWReadError( this, string( "Undefined safety, " ) + value );
				}
				safetyCandidates.push_back(teamNumber);
			}
		}

		safety = safetyCandidates;
	}
	else if ( key == "flag" ) {
		vector<string> flagElements = BZWParser::getLineElements(value.c_str());

		// determine whether or not the given flags are valid (throw a error if its invalid)
		for(vector<string>::iterator i = flagElements.begin(); i != flagElements.end(); i++) {
			if(	! (Flag::isFlag( i->c_str() )  || (*i) == "good" || (*i) == "bad") ) {
				throw BZWReadError( this, string( "Unrecognized flag type, " ) + *i);
			}
		}

		flags = flagElements;
	}
	else if ( key == "zoneflag" ) {
		vector<string> elems = BZWParser::getLineElements( value.c_str() );

		// there should be only two elements, a flag and a quantity
		if ( elems.size() != 2 )
			throw BZWReadError( this, string( "Invalid zoneflag, " ) + value );

		// read the flag
		string flag = TextUtils::toupper( elems[0] );
		// see if its valid
		if( !Flag::isFlag( flag.c_str() ) )
			throw BZWReadError( this, string( "Unrecognized flag type, " ) + flag );

		// try to read the quantity
		int num = atoi( elems[1].c_str() );

		// num will be zero if atoi() failed...
		if(num == 0)
			throw BZWReadError( this, string( "Unrecognized flag quantity, " ) + elems[1] );

		zoneflags.push_back( FlagElement( flag, num ) );
	}
	else {
		return bz2object::parse( line );
	}

	return true;
}

void zone::finalize() {

}

// toString
string zone::toString(void) {
	// string-ify the teams, safeties, zoneflags, and flags
	string teamString = string("");
	string safetyString = string("");
	string zoneflagString = string("");
	string flagString = string("");

	// make the "safety" string
	if(safety.size() > 0) {
		for(vector<int>::iterator index = safety.begin(); index != safety.end(); index++) {
			safetyString += string(itoa(*index)) + " ";
		}
		safetyString += "\n";
	}

	if(teams.size() > 0) {
		// make the "team" string
		for(vector<int>::iterator index = teams.begin(); index != teams.end(); index++) {
			teamString += string(itoa(*index)) + " ";
		}
		teamString += "\n";
	}

	if(flags.size() > 0) {
		// make the "flag" string
		for(vector<string>::iterator index = flags.begin(); index != flags.end(); index++) {
			flagString += (*index) + " ";
		}
		flagString += "\n";
	}

	if(zoneflags.size() > 0) {
		// make the "zoneflag" strings
		for(vector<FlagElement>::iterator index = zoneflags.begin(); index != zoneflags.end(); index++) {
			zoneflagString += "  zoneflag " + index->toString() + "\n";
		}
	}

	return string("zone\n") +
				  BZWLines( this ) +
				  (safetyString.length() == 0 ? "# safety\n" : "  safety " + safetyString) +
				  (teamString.length() == 0 ? "# team\n" : "  team " + teamString) +
				  (flagString.length() == 0 ? "# flag\n" : "  flag " + flagString) +
				  (zoneflagString.length() == 0 ? "# zoneflag\n" : zoneflagString) +
				  "end\n";
}

// render
int zone::render(void) {
	return 0;
}

void zone::updateGeometry() {
	// create unit box
	osg::ref_ptr< osg::Geode > boxGeode = new osg::Geode();
	osg::ref_ptr< osg::Box > box = new osg::Box( osg::Vec3( 0, 0, 0.5f ), 2, 2, 1 );
	osg::ref_ptr< osg::ShapeDrawable > boxDrawable = new osg::ShapeDrawable( box.get() );
	boxGeode->addDrawable( boxDrawable.get() );

	// assign material
	SceneBuilder::assignMaterial( osg::Vec4f( 0.0, 0.0, 1.0, 1.0 ),
								 osg::Vec4f( 0.0, 0.0, 0.0, 1.0 ),
								 osg::Vec4f( 0.0, 0.0, 0.0, 0.0 ),
								 osg::Vec4f( 0.0, 0.0, 0.0, 0.0 ),
								 0.0f,
								 1.0f,
								 boxGeode.get() );
	
	osg::ref_ptr< osg::StateSet > stateset = boxDrawable->getOrCreateStateSet();
	osg::ref_ptr< osg::PolygonMode > polyMode = new osg::PolygonMode();
	polyMode->setMode( osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE );
	stateset->setAttribute( polyMode.get(), osg::StateAttribute::ON);

	setThisNode( boxGeode.get() );
}
