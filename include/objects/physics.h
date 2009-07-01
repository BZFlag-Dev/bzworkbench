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

#ifndef PHYSICS_H_
#define PHYSICS_H_

#include "render/Point3D.h"
#include "DataEntry.h"

#include <osg/Referenced>

#include <string>

class physics : public DataEntry, public osg::Referenced {

public:

	// default constructor
	physics();

	// constructors for osg::Referenced
	physics( bool threadSafe );
	physics( const osg::Referenced& ref );

	// constructor with data
	physics(std::string& data);

	static DataEntry* init() { return new physics(); }
	static DataEntry* init(std::string& data) { return new physics(data); }

	// getter
	std::string get(void);

	// setter
	int update(std::string& data);

	// toString
	std::string toString(void);

	// render
	int render(void);

	// getters
	std::string& getName() { return name; }

	// setters
	void setName( const std::string& _name );

	Point3D getLinear() { return linear; }
	Point3D getAngular() { return angular; }
	float getSlide() { return slide; }
	std::string getDeathMessage() { return deathMessage; }

	void setLinear( Point3D value ) { linear = value; }
	void setAngular( Point3D value ) { angular = value; }
	void setSlide( float value ) { slide = value; }
	void setDeathMessage( std::string value ) { deathMessage = value; }

private:

	Point3D linear, angular;
	float slide;
	std::string name, deathMessage;
};

#endif /*PHYSICS_H_*/
