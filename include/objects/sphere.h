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

#ifndef SPHERE_H_
#define SPHERE_H_

#include "bz2object.h"
 
//#include "render/Point2D.h" - not needed here - is included in bz2object.h

class sphere : public bz2object {
public:
	enum {
		Edge,
		Bottom,
		MaterialCount
    };
	// default constructor
	sphere();
	
	static DataEntry* init() { return new sphere(); }

	// restore default values
	void setDefaults();
	
	// getter
	string get(void);
	
	// setter
	int update(UpdateMessage& message);

	// bzw methods
	bool parse( std::string& line );
	void finalize();
	
	// toString
	string toString(void);
	
	// render
	int render(void);

	void setSize( osg::Vec3 newSize );
	osg::Vec3 getSize();

	void setFlatshading( bool value );
	void setSmoothbounce( bool value );
	void setHemisphere( bool value );
	void setDivisions( int value );
	void setTexsize( Point2D value );

	bool getFlatshading();
	bool getSmoothbounce();
	bool getHemisphere();
	int getDivisions();
	Point2D getTexsize();
	
private:
	bool flatShading, smoothbounce, hemisphere;
	int divisions;
	osg::Vec3 realSize;

	void updateGeometry();

	static const char* sideNames[MaterialCount];
};

#endif /*SPHERE_H_*/
