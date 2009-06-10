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

#include "MeshFace.h"

MeshFace::MeshFace() : 
DataEntry("face", "<vertices><normals><texcoords><phydrv><matref><drivethrough><shootthrough><passable>") {
	vertices = vector<int>();
	normals = vector<int>();
	texcoords = vector<int>();
	physicsDriver = string("");
	materials = vector<string>();
	driveThrough = shootThrough = smoothbounce = false;
}

// constructor with data
MeshFace::MeshFace(string mat, string phydrv, bool noclusters, bool smoothbounce, bool drivethrough, bool shootthrough) : DataEntry("face", "<vertices><normals><texcoords><phydrv><matref><drivethrough><shootthrough><passable>") {
	vertices = vector<int>();
	normals = vector<int>();
	texcoords = vector<int>();
	physicsDriver = phydrv;
	materials = vector<string>();
	materials.push_back(mat);
	this->driveThrough = drivethrough;
	this->shootThrough = shootthrough;
	this->smoothbounce = smoothbounce;

	//this->update(data);
}

// getter
string MeshFace::get(void) { return this->toString(); }

bool MeshFace::parse( const char* line ) {
	string key = BZWParser::key( line );

	if ( key == "vertices" ) {
		vertices = BZWParser::getIntList( BZWParser::value( "vertices", line ).c_str() );
		if ( vertices.size() < 3 ) {
			printf("MeshFace::parse(): Faces need at least 3 vertices.\n");
			return false;
		}
	}
	else if ( key == "normals" ) {
		normals = BZWParser::getIntList( BZWParser::value( "normals", line ).c_str() );
		if ( normals.size() < 3 ) {
			printf("MeshFace::parse(): Faces need at least 3 normals.\n");
			return false;
		}
	}
	else if ( key == "texcoords" ) {
		texcoords = BZWParser::getIntList( BZWParser::value( "texcoords", line ).c_str() );
		if ( texcoords.size() < 3 ) {
			printf("MeshFace::parse(): Faces need at least 3 texcoords.\n");
			return false;
		}
	}
	else if ( key == "phydrv" ) {
		physicsDriver = BZWParser::value( "phydrv", line );
	}
	else if ( key == "noclusters" ) {
		noClusters = true;
	}
	else if ( key == "drivethrough" ) {
		driveThrough = true;
	}
	else if ( key == "shootthrough" ) {
		shootThrough = true;
	}
	else if ( key == "passable" ) {
		driveThrough = true;
		shootThrough = true;
	}
	else if ( key == "ricochet" ) {
		ricochet = true;
	}
	else if ( key == "baseteam" ) {
		string value = BZWParser::value( "baseteam", line );
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf("MeshFace::parse(): Error! Missing baseteam parameter.\n");
			return false;
		}
		specialData.baseTeam = atoi( value.c_str() );
	}
	else if ( key == "linkname" ) {
		string value = BZWParser::value( "linkname", line );
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf("MeshFace::parse(): Error! Missing linkname parameter.\n");
			return false;
		}
		specialData.linkName = value;
	}
	else if ( key == "" ) {
		// FIXME: finish adding keys
	}

	return true;
}

// toString
string MeshFace::toString(void) {
	// string-ify the material list
	string matstring = string("");
	if(materials.size() > 0) {
		for(vector<string>::iterator i = materials.begin(); i != materials.end(); i++) {
			matstring += "    matref " + (*i) + "\n";	
		}
	}

	return string("face\n") +
		(vertices.size() > 0 ? "    vertices " + stringify(vertices) + "\n" : "") +
		(texcoords.size() > 0 ? "    texcoords " + stringify(texcoords) + "\n" : "") +
		(normals.size() > 0 ? "    normals " + stringify(normals) + "\n" : "") +
		matstring +
		(physicsDriver.length() != 0 ? "    phydrv " + physicsDriver + "\n" : "") +
		(shootThrough == true ? "    shootthrough\n" : "") +
		(driveThrough == true ? "    drivethrough\n" : "") +
		"  endface\n";

}

// render
int MeshFace::render(void) {
	return 0;
}

string MeshFace::stringify(vector<int>& values) {
	string ret = string("");

	if(values.size() > 0) {
		for(vector<int>::iterator i = values.begin(); i != values.end(); i++) {
			ret += string(itoa(*i)) + " ";	
		}	
	}

	return ret;
}