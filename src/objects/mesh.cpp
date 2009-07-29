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
	bz2object("mesh", "<name><vertex><normal><texcoord><inside><outside><shift><scale><shear><spin><phydrv><smoothbounce><noclusters><face><drawinfo><drivethrough><shootthrough>") {

	vertices = vector<Point3D>();
	texCoords = vector<Point2D>();
	normals = vector<Point3D>();
	insidePoints = vector<Point3D>();
	outsidePoints = vector<Point3D>();
	decorative = false;
	faces = vector<MeshFace*>();
	drawInfo = NULL;
	currentDrawInfo = NULL;
	currentFace = NULL;
	currentMaterial = NULL;
	noclusters = false;
	smoothbounce = false;
	drivethrough = false;
	shootthrough = false;
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

// bzw methods
bool mesh::parse( std::string& line ) {
	string key = BZWParser::key( line.c_str() );

	if ( currentDrawInfo ) {
		if ( !currentDrawInfo->parse( line ) ) {
			drawInfo = currentDrawInfo;
			currentDrawInfo = NULL;
		}
	}
	else if ( key == "mesh" )
		return true;
	else if ( key == "lod" ) {
		lodOptions.push_back( BZWParser::value( "lod", line.c_str() ) );
	}
	else if ( currentFace ) {
		if ( !currentFace->parse( line ) ) {
			faces.push_back(currentFace);
			currentFace = NULL;
		}
	}
	else if ( key == "face" ) {
		if (currentFace != NULL) {
			printf("mesh::update(): Warning! Discarding incomplete mesh face.\n");
			delete currentFace;
		}
		currentFace = new MeshFace( currentMaterial, phydrv, noclusters, smoothbounce, drivethrough, shootthrough );
	}
	else if ( key == "inside" ) {
		insidePoints.push_back( Point3D( BZWParser::value( "inside", line.c_str() ) ) );
	}
	else if ( key == "outside" ) {
		outsidePoints.push_back( Point3D( BZWParser::value( "outside", line.c_str() ) ) );
	}
	else if ( key == "vertex" ) {
		vertices.push_back( Point3D( BZWParser::value( "vertex", line.c_str() ) ) );
	}
	else if ( key == "normal" ) {
		normals.push_back( Point3D( BZWParser::value( "normal", line.c_str() ) ) );
	}
	else if ( key == "texcoord" ) {
		texCoords.push_back( Point2D( BZWParser::value( "texcoord", line.c_str() ) ) );
	}
	else if ( key == "phydrv" ) {
		string drvname = BZWParser::value( "phydrv", line.c_str() );
		physics* phys = (physics*)Model::command( MODEL_GET, "phydrv", drvname.c_str() );
		if (phys != NULL)
			phydrv = phys;
		else
			throw BZWReadError( this, string( "Couldn't find physics driver, " ) + drvname );
	}
	else if ( key == "noclusters" ) {
		noclusters = true;
	}
	else if ( key == "decorative" ) {
		decorative = true;
	}
	else if ( key == "drawinfo" ) {
		if (drawInfo != NULL) {
			throw BZWReadError( this, "Multiple drawinfo sections." );
		}
		else {
			currentDrawInfo = new DrawInfo();
		}
	}
	else if ( key == "matref" ) {
		string matName = BZWParser::value( "matref", line.c_str() );
		material* mat = dynamic_cast< material* >( Model::command( MODEL_GET, "material", matName ) );

		if ( mat )
			currentMaterial = mat;
		else
			throw BZWReadError( this, string( "Couldn't find material, " ) + matName );
	}
	else if ( key == "end" ) // need to check if we're at the end of the section
		return false;
	else {
		return bz2object::parse( line );
	}

	return true;
}

void mesh::finalize() {
	updateGeometry();
	bz2object::finalize();
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


		vector<int> vertIdx = face->getVertices();
		vector<int> texcoordIdx = face->getTexcoords();

		for (vector<int>::iterator vitr = vertIdx.begin(), titr = texcoordIdx.begin();
			vitr != vertIdx.end() && titr != texcoordIdx.end(); vitr++, titr++) {
				vertices->push_back( this->vertices[ *vitr ] );
				texcoords->push_back( this->texCoords[ *titr ] );
				indices->push_back( arrayPos++ );
		}
		geometry->addPrimitiveSet( indices );

		lastmat = mat;
		lastNoTexcoords = noTexcoords;
	}

	setThisNode(group);
}
