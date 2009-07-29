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

#ifndef MESHFACE_H_
#define MESHFACE_H_

#include "DataEntry.h"
#include "model/BZWParser.h"
#include "objects/physics.h"
#include "objects/material.h"
#include <vector>
#include <string>

/**
 * This is a class for reading, parsing, writing, and storing information about
 * a "mesh" object's faces.  It assists "mesh" similar to how "ColorCommand" assists
 * "dynamicColor" and "Transform" assists "bz2object"
 */

class MeshFace : public DataEntry {

public:

	// default constructor
	MeshFace();

	// constructor with data
	MeshFace(material* mat, physics* phydrv, bool noclusters, bool smoothbounce, bool drivethrough, bool shootthrough);

	~MeshFace();

	// getter
	string get(void);

	bool parse( std::string& line );

	// toString
	string toString(void);

	// render
	int render(void);

	vector<int> getVertices() { return vertices; }
	vector<int> getNormals() { return normals; }
	vector<int> getTexcoords() { return texcoords; }
	material* getMaterial() { return mat; }

	    struct LinkGeometry {
      LinkGeometry()
      : centerIndex(-1) // index to a vertex
      , sDirIndex(-1)   // index to a normal
      , tDirIndex(-1)   // index to a normal
      , pDirIndex(-1)   // index to a normal
      , sScale(1.0f)
      , tScale(1.0f)
      , pScale(0.0f) // note the 0.0f
      , angle(0.0f)
	  , LinkAutoSscale(true)
	  , LinkAutoTscale(true)
	  , LinkAutoPscale(true)
      {}
      int centerIndex;
      int sDirIndex;
      int tDirIndex;
      int pDirIndex;
	  osg::Vec3 center;
      osg::Vec3 sDir; // usually points right,   looking towards the plane
      osg::Vec3 tDir; // usually points upwards, looking towards the plane
      osg::Vec3 pDir; // usually the -plane.xyz() for srcs, +plane.xyz() for dsts
      float sScale;
      float tScale;
      float pScale;
      float angle;   // calculated
	  bool LinkAutoSscale;
      bool LinkAutoTscale;
      bool LinkAutoPscale ;
    };

    struct SpecialData {
      SpecialData()
		  : baseTeam(-1),
			LinkSrcRebound(false),
			LinkSrcNoGlow(false),
			LinkSrcNoRadar(false),
			LinkSrcNoSound(false),
			LinkSrcNoEffect(false)
      {}

      unsigned short stateBits; // uses SpecialBits enum

      int baseTeam;

      std::string  linkName;
      LinkGeometry linkSrcGeo;
      LinkGeometry linkDstGeo;
      std::string  linkSrcShotFail;
      std::string  linkSrcTankFail;

	  bool LinkSrcRebound;
	  bool LinkSrcNoGlow;
	  bool LinkSrcNoRadar;
	  bool LinkSrcNoSound;
	  bool LinkSrcNoEffect;
    };

private:
	/**
	 * Helper method: convert an array of ints into a string
	 */

	string stringify(vector<int>& values);
	vector<int> vertices;
	vector<int> normals;
	vector<int> texcoords;
	physics* physicsDriver;
	material* mat;

	bool driveThrough;
	bool shootThrough;
	bool noClusters;
	bool smoothbounce;
	bool ricochet;

	// special data
	SpecialData specialData;
};

#endif /*MESHFACE_H_*/
