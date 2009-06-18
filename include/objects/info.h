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

#ifndef INFO_H_
#define INFO_H_

#include "objects/bz2object.h"

class info : public bz2object {
public:

	// default constructor
	info();

	// constructor with data
	info(string& data);

	static DataEntry* init() { return new info(); }
	static DataEntry* init(string& data) { return new info(data); }

	// getter
	string get(void);

	// setter
	int update(string& data);

	// toString
	string toString(void);

	// getters/setters
	std::vector<std::string> getLines() { return infoLines; }

	void setLines( std::vector<std::string> value ) { infoLines = value; }

protected:
	std::vector<std::string> infoLines;

};

#endif /* INFO_H_ */
