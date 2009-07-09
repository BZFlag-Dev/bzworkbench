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

#ifndef PYRAMID_H_
#define PYRAMID_H_

#include "bz2object.h"
#include <string>
#include <vector>

#include "render/GeometryExtractorVisitor.h"
#include "render/Point2D.h"

using namespace std;

/**
 * The 1.x pyramid object.
 */

class pyramid : public bz2object {
public:
	enum {
		XP = 0,
		XN,
		YP,
		YN,
		ZN,
		FaceCount
	};

	// default constructor
	pyramid();

	// constructor with data
	pyramid(string& data);

	static DataEntry* init() { return new pyramid(); }
	static DataEntry* init(string& data) { return new pyramid(data); }

	void setDefaults();

	// getter
	string get();

	// setter
	int update(string& data);

	// setter with messaging
	int update(UpdateMessage& message);

	// toString
	string toString(void);

	// render
	int render(void);

	void setSize( osg::Vec3 newSize );

	Point2D getTexsize( int face );
	Point2D getTexoffset( int face );
	bool getDrivethrough( int face );
	bool getShootthrough( int face );
	bool getRicochet( int face );
	bool getFlipz();

	void setTexsize( int face, Point2D value );
	void setTexoffset( int face, Point2D value );
	void setDrivethrough( int face, bool value );
	void setShootthrough( int face, bool value );
	void setRicochet( int face, bool value );
	void setFlipz( bool value );

private:
	bool flipz;

	Point2D texSizes[FaceCount];
	Point2D texOffsets[FaceCount];
	bool driveThroughs[FaceCount];
	bool shootThroughs[FaceCount];
	bool ricochets[FaceCount];

	static const char* faceNames[FaceCount];

	void updateGeometry();
};

#endif /*PYRAMID_H_*/
