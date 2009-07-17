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

#ifndef OPTIONS_H_
#define OPTIONS_H_

#include "DataEntry.h"
#include <string>

using namespace std;

class options : public DataEntry {
public:
	
	// default constructor
	options();
	
	static DataEntry* init() { return new options(); }

	// get method
	string get(void);

	// bzw methods
	bool parse( std::string& line );
	void finalize();
	
	// toString method
	string toString(void);
	
	// get/set the options string
	string getOptionsString() { return optionsString; }
	void setOptionsString( string newString ) { optionsString = newString; }
	
private:
	string optionsString;
};

#endif /*OPTIONS_H_*/
