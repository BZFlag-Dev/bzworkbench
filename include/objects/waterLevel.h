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

#ifndef WATERLEVEL_H_
#define WATERLEVEL_H_

#include "DataEntry.h"
#include <string>

class material;

class waterLevel : public DataEntry {
public:

	// default constructor
	waterLevel();
	
	static DataEntry* init() { return new waterLevel(); }
	
	// get method
	std::string get(void);

	// bzw methods
	bool parse( std::string& line );
	void finalize();
	
	// toString method
	string toString(void);
	
	// getters/setters
	string getName() { return name; }
	material* getMaterial() { return waterMaterial; }
	float getHeight() { return height; }
	
	void setName( std::string _name ) { name = _name; }
	void setMaterial( material* _material ) { waterMaterial = _material; }
	void setHeight( float _height ) { height = _height; }
	
	virtual ~waterLevel();
	
private:
	std::string name;
	material* waterMaterial;
	float height;
};

#endif /*WATERLEVEL_H_*/
