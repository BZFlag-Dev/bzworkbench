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

#ifndef MESH_H_
#define MESH_H_

#include "bz2object.h"
#include "render/Point3D.h"
#include "render/Index3D.h"
#include "render/Point2D.h"
#include "MeshFace.h"
#include "LOD.h"
#include "LODCommand.h"
#include "DrawInfo.h"

#include <map>

class mesh : public bz2object {
	
public:

	// constructor
	mesh(void);

	~mesh();
	
	static DataEntry* init() { return new mesh(); }
	
	// getter
	string get(void);

	// bzw methods
	bool parse( std::string& line );
	void finalize();
	
	// toString
	string toString(void);
	
	// render
	int render(void);
	
private:
	// vertices
	vector<Point3D> vertices;
	
	// texture coordinates
	vector<Point2D> texCoords;
	
	// normals
	vector<Point3D> normals;
	
	// inside points
	vector<Point3D> insidePoints;
	
	// outside points
	vector<Point3D> outsidePoints;
	
	// faces
	vector<MeshFace*> faces;
	
	std::vector<std::string> lodOptions;
	bool decorative;

	// draw info
	DrawInfo* drawInfo;

	void updateGeometry();

	// for parsing bzw
	MeshFace* currentFace;
	material* currentMaterial;
	DrawInfo* currentDrawInfo;
	physics* phydrv;
	bool noclusters;
};

#endif /*MESH_H_*/
