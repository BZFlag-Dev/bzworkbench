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

#include "objects/mesh.h"

// default constructor
mesh::mesh(void) :
	bz2object("mesh", "<vertex><normal><texcoord><inside><outside><shift><scale><shear><spin><phydrv><smoothbounce><noclusters><face><drawinfo>") {

	vertices = vector<Point3D>();
	texCoords = vector<Point2D>();
	normals = vector<Point3D>();
	insidePoints = vector<Point3D>();
	outsidePoints = vector<Point3D>();
	decorative = false;
	materials = vector< material* >();
	faces = vector<MeshFace*>();
	drawInfo = NULL;
	materialMap = map<string, string>();
}

// constructor with data
mesh::mesh(string& data) :
	bz2object("mesh", "<vertex><normal><texcoord><inside><outside><shift><scale><shear><spin><phydrv><smoothbounce><noclusters><face><drawinfo>", data.c_str()) {

	vertices = vector<Point3D>();
	texCoords = vector<Point2D>();
	normals = vector<Point3D>();
	insidePoints = vector<Point3D>();
	outsidePoints = vector<Point3D>();
	decorative = false;
	materials = vector< material* >();
	faces = vector<MeshFace*>();
	drawInfo = NULL;
	materialMap = map<string, string>();

	update(data);
}

mesh::~mesh() {
	// clean up
	/*if (drawInfo != NULL)
		delete drawInfo;

	for (vector<MeshFace*>::iterator itr = faces.begin(); itr != faces.end(); itr++) {
		if ( *itr != NULL )
			delete *itr;
	}*/
}

// getter
string mesh::get(void) { return toString(); }

// setter
int mesh::update(string& data) {
	const char* header = getHeader().c_str();

	// get lines
	vector<string> chunks = BZWParser::getSectionsByHeader(header, data.c_str(), "end");

	// break if there are none
	if(chunks[0] == BZW_NOT_FOUND) {
		printf("mesh not found\n");
		return 0;
	}

	// break if too many
	if(!hasOnlyOne(chunks, header))
		return 0;

	// get the data
	const char* meshData = chunks[0].c_str();

	vector<string> lines = BZWParser::getLines(header, data.c_str());

	MeshFace* face = NULL;
	DrawInfo* drawInfo = NULL;
	string mat;
	physics* phydrv = NULL;
	bool smoothbounce = false;
	bool noClusters = false;
	bool driveThrough = false;
	bool shootThrough = false;
	for (vector<string>::iterator itr = lines.begin(); itr != lines.end(); itr++) {
		const char* line = (*itr).c_str();

		string key = BZWParser::key(line);

		if ( key == "lod" ) {
			lodOptions.push_back( BZWParser::value( "lod", line ) );
		}
		else if ( key == "endface" ) {
			if (face == NULL) {
				printf("mesh::update(): Warning! Extra 'endface' keyword found\n");
			} else {
				faces.push_back(face);
				face = NULL;
			}
		}
		else if ( face ) {
			face->parse( line );
		}
		else if ( key == "face" ) {
			if (face != NULL) {
				printf("mesh::update(): Warning! Discarding incomplete mesh face.\n");
				delete face;
			}
			face = new MeshFace( mat, phydrv, noClusters, smoothbounce, driveThrough, shootThrough );
		}
		else if ( key == "inside" ) {
			insidePoints.push_back( Point3D( BZWParser::value( "inside", line ) ) );
		}
		else if ( key == "outside" ) {
			outsidePoints.push_back( Point3D( BZWParser::value( "outside", line ) ) );
		}
		else if ( key == "vertex" ) {
			vertices.push_back( Point3D( BZWParser::value( "vertex", line ) ) );
		}
		else if ( key == "normal" ) {
			normals.push_back( Point3D( BZWParser::value( "normal", line ) ) );
		}
		else if ( key == "texcoord" ) {
			texCoords.push_back( Point2D( BZWParser::value( "texcoord", line ) ) );
		}
		else if ( key == "phydrv" ) {
			string drvname = BZWParser::value( "phydrv", line );
			physics* phys = (physics*)Model::command( MODEL_GET, "phydrv", drvname.c_str() );
			if (phys != NULL)
				phydrv = phys;
			else
				printf("mesh::update(): Error! Couldn't find physics driver %s\n", drvname.c_str());
		}
		else if ( key == "smoothbounce" ) {
			smoothbounce = true;
		}
		else if ( key == "noclusters" ) {
			noClusters = true;
		}
		else if ( key == "decorative" ) {
			decorative = true;
		}
		else if ( key == "drawinfo" ) {
			if (drawInfo != NULL) {
				printf("mesh::update(): Warning! Multiple drawinfo sections, using first.\n");
			}
			else {
				drawInfo = new DrawInfo();
				if ( !drawInfo->parse( itr ) ) {
					printf("mesh::update(): Error! Invalid drawInfo.\n");
					delete drawInfo;
					return 0;
				}
			}
		}
		else if ( key == "matref" ) {
			mat = BZWParser::value( "matref", line );
		}
		else {
			printf("mesh::update(): Warning! Unrecognized key: %s\n", key.c_str());
		}
	}

	updateGeometry();

	return 1;
}

// to string
string mesh::toString(void) {
	// string-ify the vertices, normals, texcoords, inside points, outside points, and faces
	/*string vertexString(""), normalString(""), texcoordString(""), insideString(""), outsideString(""), faceString("");

	if(vertices.size() > 0) {
		for(vector<Point3D>::iterator i = vertices.begin(); i != vertices.end(); i++) {
			vertexString += "  vertex " + i->toString();
		}
	}

	if(normals.size() > 0) {
		for(vector<Vector3D>::iterator i = normals.begin(); i != normals.end(); i++) {
			normalString += "  normal " + i->toString();
		}
	}

	if(texCoords.size() > 0) {
		for(vector<TexCoord2D>::iterator i = texCoords.begin(); i != texCoords.end(); i++) {
			texcoordString += "  texcoord " + i->toString() + "\n";
		}
	}

	// special instance:
	// make sure to keep the order of faces and materials constant
	if(faces.size() > 0) {
		string currMaterial = "";
		for(vector<MeshFace>::iterator i = faces.begin(); i != faces.end(); i++) {
			// see if we came across a new material
			if(currMaterial != materialMap[ i->toString() ]) {
				// if so, set the current material and append the matref to the faceString
				currMaterial = materialMap[ i->toString() ];
				faceString += "  matref " + currMaterial + "\n";
			}
			faceString += "  " + i->toString();
		}
	}

	if(insidePoints.size() > 0) {
		for(vector<Point3D>::iterator i = insidePoints.begin(); i != insidePoints.end(); i++) {
			insideString += "  inside " + i->toString();
		}
	}

	if(outsidePoints.size() > 0) {
		for(vector<Point3D>::iterator i = outsidePoints.begin(); i != outsidePoints.end(); i++) {
			outsideString += "  outside " + i->toString();
		}
	}

	return string("mesh\n") +
				  (noClusters == true ? "  noclusters\n" : "") +
				  (smoothBounce == true ? "  smoothbounce\n" : "") +
				  insideString +
				  outsideString +
				  vertexString +
				  normalString +
				  texcoordString +
				  faceString +
				  (useDrawInfo == true ? "  " + drawInfo.toString() : "") + "\n" +
				  "end\n";*/

	return string(); // FIXME: correct code
}

// render
int mesh::render(void) {
	return 0;
}

void mesh::updateGeometry() {
	osg::Group* group = new osg::Group();

	osg::Geode* geode = NULL;
	osg::Geometry* geometry = NULL;
	osg::Vec3Array* vertices = NULL;
	osg::Vec2Array* texcoords = NULL;
	int arrayPos = 0;
	material* lastmat = NULL;
	bool lastNoTexcoords = false;
	bool first = true;

	for (vector<MeshFace*>::iterator itr = faces.begin(); itr != faces.end(); itr++) {
		MeshFace* face = *itr;
		material* mat = face->getMaterial();
		bool noTexcoords = false;
		if (face->getTexcoords().size() == 0)
			noTexcoords = true;

		// need to make a new geode if the material changes
		if (first || mat != lastmat || lastNoTexcoords != noTexcoords) {
			geode = new osg::Geode();
			geometry = new osg::Geometry();
			vertices = new osg::Vec3Array();

			if (!noTexcoords) {
				texcoords = new osg::Vec2Array();
				geometry->setTexCoordArray( 0, texcoords );
			}

			group->addChild( geode );
			geode->addDrawable( geometry );
			geode->setStateSet( mat );
			geometry->setVertexArray( vertices );

			arrayPos = 0;
			first = false;
		}

		osg::DrawElementsUInt* indices = new osg::DrawElementsUInt( osg::DrawElements::TRIANGLE_STRIP, 0 );
		geometry->addPrimitiveSet( indices );

		vector<int> vertIdx = face->getVertices();
		vector<int> texcoordIdx = face->getTexcoords();

		for (vector<int>::iterator vitr = vertIdx.begin(), titr = texcoordIdx.begin();
			vitr != vertIdx.end() && titr != texcoordIdx.end(); vitr++, titr++) {
				vertices->push_back( this->vertices[ *vitr ] );
				texcoords->push_back( this->texCoords[ *titr ] );
				indices->push_back( arrayPos++ );
		}

		lastmat = mat;
		lastNoTexcoords = noTexcoords;
	}

	setThisNode(group);
}
