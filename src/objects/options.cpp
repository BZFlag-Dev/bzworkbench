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

#include "objects/options.h"

// default constructor
options::options() : DataEntry("options", "") {
	optionsString = string("");
}

// get method
string options::get(void) {
	return toString();
}

// bzw methods
bool options::parse( std::string& line ) {
	// check if we reached the end of the section
	if ( line == "end" )
		return false;

	// otherwise add to the optionsString
	optionsString += line + " ";

	return true;
}

void options::finalize() {
	// nothing to do
}

// toString method
string options::toString(void) {
	return string(string("options\n") +
						"  " + optionsString + "\n" + 
						getUnusedText() + 
						"end\n");
}
