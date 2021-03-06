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

#ifndef CONE_H_
#define CONE_H_

#include "objects/bz2object.h"
#include "model/SceneBuilder.h"
#include "render/Point2D.h"

#include <math.h>

class cone : public bz2object {
public:
	enum {
		Edge,
		Bottom,
		StartFace,
		EndFace,
		MaterialCount
	};

	// default constructor
	cone();
	
	static DataEntry* init() { return new cone(); }
	
	virtual void setDefaults();

	// getter
	string get(void);
	
	// setter
	int update(UpdateMessage& msg);

	// bzw methods
	bool parse( std::string& line );
	void finalize();
	
	// toString
	string toString(void);
	
	// getters/setters
	float getSweepAngle() { return sweepAngle; }
	float getSweepRotation() { return angle; }
	int getDivisions() { return divisions; }
	
	
	void setSweepAngle(float value);
	void setSweepRotation(float value);
	
	void setDivisions(int value);

	bool getFlipz() { return flipz; }
	void setFlipz(bool value) { flipz = value; buildGeometry();}
	Point2D getTexsize() { return texsize; }
	void setTexsize( Point2D value ) { texsize = value; }
	
protected:
	static const char* sideNames[MaterialCount];

	bool flipz;
	int divisions;
	bool pyramidStyle;
	
	// sweep angle
	float sweepAngle;
	// rotation angle
	float angle;
	
	// helper method to build the geometry
	void buildGeometry();
};

#endif /*CONE_H_*/
