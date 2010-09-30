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

#ifndef LODCOMMAND_H_
#define LODCOMMAND_H_

#include "DataEntry.h"
#include "model/Model.h"

class LODCommand : public DataEntry {
	
public:
	
	// constructor
	LODCommand() : DataEntry("", "<dlist><sphere><points><lines><lineloop><linestrip><tris><tristrip><trifan><quads><quadstrip><polygon>") {
		name = string("");
		args = vector<int>();
		rad = 0;
		x = y = z = 0;
	}
	
	// constructor with data
	LODCommand(string& data) : DataEntry("", "<dlist><sphere><points><lines><lineloop><linestrip><tris><tristrip><trifan><quads><quadstrip><polygon>", data.c_str()) {
		name = string("");
		args = vector<int>();
		rad = 0;
		x = y = z = 0;
		
		this->update(data);	
	}
	
	// getter
	string get(void) { return this->toString(); }
	
	// setter
	int update(string& data) {
		
		// assume its just one line
		// is the command valid?
		if(!isValidCommand(data))
			return 0;
		
		// get the name
		string newName = BZWParser::key( data.c_str() );
		
		// get the data
		string value = BZWParser::value( newName.c_str(), data.c_str() );
		
		// break up the data into line elements
		vector<string> values = vector<string>();
		if(value != newName)
			values = BZWParser::getLineElements( value.c_str() );
		
		// set the data in the superclass
		if(!DataEntry::update(data))
			return 0;
		
		// load the data in (first clear the arg list)
		args.clear();
		this->name = newName;
		
		// exception: sphere's last arg has to be a float
		if(newName == "sphere") {
			x = atof( values[0].c_str() );
			y = atof( values[1].c_str() );
			z = atof( values[2].c_str() );
			rad = atof( values[3].c_str() );
		}
		else if(values.size() > 0) {
			for(vector<string>::iterator i = values.begin(); i != values.end(); i++) {
				args.push_back( atoi( i->c_str() ) );	
			}
		}
		
		return 1;
		
	}
	
	/**
	 * Helper method:  determine whether or not the command is valid
	 */
	 
	static bool isValidCommand(string& command) {
		
		// non-existant commands are valid
		if(command.length() == 0)
			return true;
			
		// get the name and values
		string commandName = BZWParser::key( command.c_str() );
		
		// make sure its a valid command
		// (can't call isKey here--isKey isn't and can't be static)
		if(! (commandName == "dlist" ||
			  commandName == "sphere" ||
			  commandName == "points" ||
			  commandName == "lines" ||
			  commandName == "lineloop" ||
			  commandName == "linestrip" ||
			  commandName == "tris" ||
			  commandName == "tristrip" ||
			  commandName == "trifan" ||
			  commandName == "quads" ||
			  commandName == "quadstrip" ||
			  commandName == "polygon") )
			  		return false;
		
		// get the command values
		string val = BZWParser::value( commandName.c_str(), command.c_str() );
			  		
		vector<string> values = BZWParser::getLineElements( val.c_str() );
		
		// sphere gets four args
		if(commandName == "sphere" && values.size() != 4)
			return false;
			
		// points gets at least one arg
		if(commandName == "point" && values[0].size() > 0)
			return false;
			
		// lines gets at least 2 args
		if(commandName == "lines" && values.size() < 2)
			return false;
			
		// lineloop gets at least 2 args
		if(commandName == "lineloop" && values.size() < 2)
			return false;
			
		// linestrip gets at least 2 args
		if(commandName == "linestrip" && values.size() < 2)
			return false;
			
		// tris gets at least 3 args
		if(commandName == "tris" && values.size() < 3)
			return false;
			
		// tristrip gets at least 3 args
		if(commandName == "tristrip" && values.size() < 3)
			return false;
			
		// trifan gets at least 3 args
		if(commandName == "trifan" && values.size() < 3)
			return false;
			
		// quads gets at least 4 args
		if(commandName == "quads" && values.size() < 4)
			return false;
		
		// quadstrip gets at least 4 args
		if(commandName == "quadStrip" && values.size() < 4)
			return false;
			
		// polygon gets at least 2
		if(commandName == "polygon" && values.size() < 2)
			return false;
			
		// test passed
		return true;
	}
	
	// toString
	string toString(void) {
		if(name == "dlist")
			return name;
		else if(name == "sphere")
			return name + " " + string(ftoa(x)) + " " + string(ftoa(y)) + " " + string(ftoa(z)) + " " + string(ftoa(rad));
		else
			return name + " " + stringify(args);
	}
	
	bool parse( string& line ) { 
		string key = BZWParser::key( line.c_str() );
		string value = BZWParser::value( key.c_str(), line.c_str() );
		
		if ( key == "dlist"){
			// dlist # display list for this material set
			if(update(line))
			return true; 
		}
		else if ( key == "sphere"){ 
			// sphere <x> <y> <z> <radiusSquared>
			if(update(line))
			return true; 
		}
		else if ( key == "points"){ 
			// points    0            (repeatable)
			//vertices = BZWParser::getIntList( value.c_str() );
			if(update(line))
			return true; 
		}
		else if ( key == "lines"){ 
			// lines     0 1          (repeatable)
			if(update(line))
			return true; 
		}
		else if ( key == "lineloop"){ 
			// lineloop  0 1          (repeatable)
			if(update(line))
			return true; 
		}
		else if ( key == "linestrip"){ 
			// linestrip 0 1          (repeatable)
			if(update(line))
			return true; 
		}
		else if ( key == "tris"){ 
			// tris      0 1 2        (repeatable)
			if(update(line))
			return true; 
		}
		else if ( key == "tristrip"){ 
			// tristrip  0 1 2        (repeatable)
			if(update(line))
			return true; 
		}
		else if ( key == "trifan"){ 
			// trifan    0 1 2        (repeatable)
			if(update(line))
			return true; 
		}
		else if ( key == "quads"){ 
			// quads     0 1 2 3      (repeatable)
			if(update(line))
			return true; 
		}
		else if ( key == "quadstrip"){ 
			// quadstrip 0 1 2 3      (repeatable)
			if(update(line))
			return true; 
		}
		else if ( key == "polygon"){ 
			// polygon   0 1 2        (repeatable)
			if(update(line))
			return true; 
		}
		else if( key == "end" ){
			return false;
		}
		throw BZWReadError( this, string( "Unknown MaterialSet(LODCommand) Command: " ) + line );
		return true;
	} 
	
	// render
	int render(void) {
		return 0;	
	}
	
	string getName() {return name;}
	vector<int> getArgs() {return args;}
	
private:

	// turn an array of ints into a string
	string stringify(vector<int>& values) {
		string ret = string("");
		
		if(values.size() > 0) {
			for(vector<int>::iterator i = values.begin(); i != values.end(); i++) {
				ret += string(itoa(*i)) + " ";	
			}	
		}
		
		return ret;
	}

	string name;
	vector<int> args;
	float x, y, z, rad;		// only used in sphere
};

#endif /*LODCOMMAND_H_*/
