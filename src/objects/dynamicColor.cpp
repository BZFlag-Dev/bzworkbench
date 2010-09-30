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

#include "objects/dynamicColor.h"

#include "model/Model.h"
#include "model/SceneBuilder.h"

// default constructor
dynamicColor::dynamicColor() :
	DataEntry("dynamicColor", "<red><green><blue><alpha><name>") {

	name = SceneBuilder::makeUniqueName( "dynamicColor" );
	redCommands = vector<ColorCommand>();
	greenCommands = vector<ColorCommand>();
	blueCommands = vector<ColorCommand>();
	alphaCommands = vector<ColorCommand>();
}

// getter
string dynamicColor::get(void) { return toString(); }

/**
 * Helper method:  check and see if the commands given are valid ColorCommands
 */

bool areValidCommands(vector<string>& commandList) {
	// an empty list is valid
	if(commandList.size() == 0)
		return true;

	// iterate through and use ColorCommand::isValid() to check each command
	for(vector<string>::iterator i = commandList.begin(); i != commandList.end(); i++) {
		if(! ColorCommand::isValid( *i ) ) {
			printf("dynamicColor::update(): Error! Invalid color command \"%s\"\n", i->c_str());
			return false;
		}
	}

	return true;
}

/**
 * 	Helper method: convert a vector of ColorCommand objects into one huge BZW-formatted string
 */
string stringifyCommands(vector<ColorCommand>& commands, const char* color) {
	string ret = string("");

	// break if there are no commands
	if(commands.size() == 0)
		return ret;

	// iterate through the commands and concat their toString() output to ret
	for(vector<ColorCommand>::iterator i = commands.begin(); i != commands.end(); i++) {
		ret += string("  ") + color + " " + i->toString();
	}

	return ret;
}

// bzw methods
bool dynamicColor::parse( std::string& line ) {
	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );
	
	// check if we reached the end of the section
	if ( key == "end" )
		return false;

	if ( key == "name" ) {
		name = value;
	}
	else if ( key == "red" ) {
		redCommands.push_back( ColorCommand( value ) );
	}
	else if ( key == "green" ) {
		greenCommands.push_back( ColorCommand( value ) ); 
	}
	else if ( key == "blue" ) {
		blueCommands.push_back( ColorCommand( value ) );
	}
	else if ( key == "alpha" ) {
		alphaCommands.push_back( ColorCommand( value ) );
	}
	else {
		throw BZWReadError( this, string( "Unknown key, " ) + key );
	}

	return true;	
}

void dynamicColor::finalize() {
	// nothing to do
}

// toString
string dynamicColor::toString(void) {
	// string-ify the commands by color
	string redString = stringifyCommands( redCommands, "red" );
	string blueString = stringifyCommands( blueCommands, "blue" );
	string greenString = stringifyCommands( greenCommands, "green" );
	string alphaString = stringifyCommands( alphaCommands, "alpha" );

	return string("dynamicColor\n") +
				  (name.length() != 0 ? "  name " + name : string("# name")) + "\n" +
				  redString +
				  greenString +
				  blueString +
				  alphaString +
				  "end\n";
}

// render
int dynamicColor::render(void) {
	return 0;
}

void dynamicColor::setName( const string& _name ) {
	if (_name != getName()){
		if ( Model::renameDynamicColor( getName(), _name ) ) {
			this->name = _name;
		}
	}
}
