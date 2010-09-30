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

#ifndef DRAWINFO_H_
#define DRAWINFO_H_

/**
 * This is an assistant class to "mesh".  It stores the drawinfo section of the mesh
 */

#include "DataEntry.h"
#include "model/Model.h"
#include "model/BZWParser.h"
#include "render/Point2D.h"
#include "render/Point3D.h"
#include "render/Point4D.h"
#include "render/Index3D.h"
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
	vector<Point3D>& getVertices() { return vertices; }
	vector<Point3D>& getNormals() { return normals; }
	vector<Point2D>& getTexcoords() { return texcoords; }
	vector<Index3D>& getCorners() { return corners; }
	vector<LOD>& getLods() { return lods; }
	Point3D getMinExtents() { return minExtents; }
	Point3D getMaxExtents() { return maxExtents; }
	Point3D getSpherePosition() { return spherePosition; }
	float getAngvel() { return angvel; }
	float getSphereRadius() { return sphereRadius; }
	bool isDlist() { return dlist; }
	bool isDecorative() { return decorative; }

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
	vector<Point3D> normals;
	vector<Point2D> texcoords;
	vector<Index3D> corners;
	vector<LOD> lods;
	
	Point3D minExtents, maxExtents, spherePosition;
	
	float angvel, sphereRadius;
	
	bool dlist, decorative;
	
	LOD* currentLOD;
};

#endif /*DRAWINFO_H_*/
