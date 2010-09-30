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

#ifndef BOX_H_
#define BOX_H_

#include "DataEntry.h"
#include "render/Point3D.h"
#include "render/Point2D.h"
#include "ftoa.h"
#include "model/BZWParser.h"
#include "bz2object.h"

#include "model/SceneBuilder.h"
#include "model/Primitives.h"

#include <osg/Geometry>
#include <osg/PrimitiveSet>
#include <osg/Group>

/**
 * Box data
 */
class box : public bz2object {

public:
	enum {
		XP = 0,
		XN,
		YP,
		YN,
		ZP,
		ZN,
		FaceCount
	};

	// constructor
	box();

	static DataEntry* init() { return new box(); }

	// nothing to destroy...
	virtual ~box();

	// restore default values
	void setDefaults();

	// getter
	string get(void);

	// setters
	int update(UpdateMessage& msg);

	// bzw methods
	bool parse( std::string& line );
	void finalize();

	// toString
	string toString(void);

	void setSize( osg::Vec3 newSize );

	Point2D getTexsize( int face );
	Point2D getTexoffset( int face );
	bool getDrivethrough( int face );
	bool getShootthrough( int face );
	bool getRicochet( int face );

	void setTexsize( int face, Point2D value );
	void setTexoffset( int face, Point2D value );
	void setDrivethrough( int face, bool value );
	void setShootthrough( int face, bool value );
	void setRicochet( int face, bool value );

private:
	Point2D texSizes[FaceCount];
	Point2D texOffsets[FaceCount];
	bool driveThroughs[FaceCount];
	bool shootThroughs[FaceCount];
	bool ricochets[FaceCount];

	static const char* faceNames[FaceCount];

	void updateGeometry();
};

#endif /*BOX_H_*/
