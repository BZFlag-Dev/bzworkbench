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

#ifndef DRAWINFO_H_
#define DRAWINFO_H_

/**
 * This is an assistant class to "mesh".  It stores the drawinfo section of the mesh
 */

#include "DataEntry.h"
#include "model/BZWParser.h"
#include "render/Point3D.h"
#include "render/Index3D.h"
#include "render/TexCoord2D.h"
#include "render/Vector3D.h"
#include "LOD.h"
#include "LODCommand.h"

class DrawInfo : public DataEntry {

public:

	// default constructor
	DrawInfo();
	
	// constructor with data
	DrawInfo(string& data);
	
	// getter
	string get(void);
		
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);
	
	// binary getters
	vector<Point3D>& getVertices();

	bool parse( std::string& line );
	
private:

	/**
	 * Helper method: convert an array of strings into an array of floats
	 */
	vector<float> parseValues(vector<string>& values);

	/**
	 * Helper method: convert an array of floats into a string
	 */
	string stringify(vector<float>& values);

	vector<Point3D> vertices;
	vector<Vector3D> normals;
	vector<TexCoord2D> texcoords;
	vector<Index3D> corners;
	vector<LOD> lods;
	
	Point3D minExtends, maxExtends, spherePosition;
	
	float angvel, sphereRadius;
	
	bool dlist, decorative;
};

#endif /*DRAWINFO_H_*/
