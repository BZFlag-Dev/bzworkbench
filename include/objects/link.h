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

#ifndef LINK_H_
#define LINK_H_

#include "bz2object.h"

#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Shape>

class teleporter;

/**
 * Link object between teleporters.
 * Needs to be called "Tlink" because link() is a system call
 * which voids the effect of the constructor
 */
class Tlink : public bz2object {
	
public:
	
	// constructor
	Tlink();
	
	static DataEntry* init() { return new Tlink(); }
	
	// getter
	string get(void);
	
	// bzw methods
	bool parse( std::string& line );
	void finalize();
	
	// toString
	string toString(void);
	
	// getters
	teleporter* getFrom() { return from; }
	teleporter* getTo() { return to; }
	
	// setters
	void setFrom( teleporter* _from ) { from = _from; }
	void setTo( teleporter* _to ) { to = _to; }
	
private:
	teleporter *from;
	teleporter *to;
	
	// build the linkage geometry
	void buildGeometry();
};


#include "teleporter.h"

#endif /*LINK_H_*/
