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
	mat = NULL;
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
	this->mat = NULL;
	makeMaterial();

	//this->update(data);
}

MeshFace::~MeshFace() {
	mat->unref();
}

// getter
string MeshFace::get(void) { return this->toString(); }

bool MeshFace::parse( const char* line ) {
	string key = BZWParser::key( line );
	string value = BZWParser::value( key.c_str(), line );

	if ( key == "vertices" ) {
		vertices = BZWParser::getIntList( value.c_str() );
		if ( vertices.size() < 3 ) {
			printf("MeshFace::parse(): Faces need at least 3 vertices.\n");
			return false;
		}
	}
	else if ( key == "normals" ) {
		normals = BZWParser::getIntList( value.c_str() );
		if ( normals.size() < 3 ) {
			printf("MeshFace::parse(): Faces need at least 3 normals.\n");
			return false;
		}
	}
	else if ( key == "texcoords" ) {
		texcoords = BZWParser::getIntList( value.c_str() );
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
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf("MeshFace::parse(): Error! Missing baseteam parameter.\n");
			return false;
		}
		specialData.baseTeam = atoi( value.c_str() );
	}
	else if ( key == "linkname" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf("MeshFace::parse(): Error! Missing linkname parameter.\n");
			return false;
		}
		specialData.linkName = value;
	}
	else if ( key == "linksrcrebound") {
		specialData.LinkSrcRebound = true;
	}
	else if ( key == "linksrcnoglow" ) {
		specialData.LinkSrcNoGlow = true;
	}
	else if ( key == "linksrcnoradar" ) {
		specialData.LinkSrcNoRadar = true;
	}
	else if ( key == "linksrcnosound" ) {
		specialData.LinkSrcNoSound = true;
	}
	else if ( key == "linksrcnoeffect" ) {
		specialData.LinkSrcNoEffect = true;
	}
	else if ( key == "linksrccenter" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf( "MeshFace::update(): Error! Missing linkSrcCenter index.\n" );
			return false;
		}
		specialData.linkSrcGeo.centerIndex = atoi( value.c_str() );
	}
	else if ( key == "linksrcsdir" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf( "MeshFace::update(): Error! Missing linkSrcSdir index.\n" );
			return false;
		}
		specialData.linkSrcGeo.sDirIndex = atoi( value.c_str() );
	}
	else if ( key == "linksrctdir" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf( "MeshFace::update(): Error! Missing linkSrcTdir index\n" );
			return false;
		}
		specialData.linkSrcGeo.tDirIndex = atoi( value.c_str() );
	}
	else if ( key == "linksrcpdir" ) {
		string value = BZWParser::value( "linksrcpdir", line );
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf( "MeshFace::update(): Error! Missing linkSrcPdir index\n" );
			return false;
		}
		specialData.linkSrcGeo.pDirIndex = atoi( value.c_str() );
	}
	else if ( key == "linksrcsscale" ) {
		string value = BZWParser::value( "linksrcsscale", line );
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf( "MeshFace::update(): Error! Missing linkSrcSscale parameter\n" );
			return false;
		}
		specialData.linkSrcGeo.sScale = atof( value.c_str() );
		specialData.linkSrcGeo.LinkAutoSscale = false;
	}
	else if ( key == "linksrctscale" ) {
		string value = BZWParser::value( "linksrctscale", line );
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf( "MeshFace::update(): Error! Missing linkSrcTscale parameter\n" );
			return false;
		}
		specialData.linkSrcGeo.tScale = atof( value.c_str() );
		specialData.linkSrcGeo.LinkAutoTscale = false;
	}
	else if ( key == "linksrcpscale" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf( "MeshFace::update(): Error! Missing linkSrcPscale parameter\n" );
			return false;
		}
		specialData.linkSrcGeo.pScale = atof( value.c_str() );
		specialData.linkSrcGeo.LinkAutoPscale = false;
	}
	//
	//  Link destination parameters
	//
	else if ( key == "linkdstcenter" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf( "MeshFace::update(): Error! Missing linkDstCenter index\n" );
			return false;
		}
		specialData.linkDstGeo.centerIndex = atoi( value.c_str() );
	}
	else if ( key == "linkdstsdir" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf( "MeshFace::update(): Error! Missing linkDstSdir index\n" );
			return false;
		}
		specialData.linkDstGeo.sDirIndex = atoi( value.c_str() );
	}
	else if ( key == "linkdsttdir" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf( "MeshFace::update(): Error! Missing linkDstTdir index\n" );
			return false;
		}
		specialData.linkDstGeo.tDirIndex = atoi( value.c_str() );
	}
	else if ( key == "linkdstpdir" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf( "MeshFace::update(): Error! Missing linkDstPdir index\n" );
			return false;
		}
		specialData.linkDstGeo.pDirIndex = atoi( value.c_str() );
	}
	else if ( key == "linkdstsscale" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf( "MeshFace::update(): Error! Missing linkDstSscale parameter\n" );
			return false;
		}
		specialData.linkDstGeo.sScale = atof( value.c_str() );
		specialData.linkDstGeo.LinkAutoSscale = false;
	}
	else if ( key == "linkdsttscale" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf( "MeshFace::update(): Error! Missing linkDstTscale parameter\n" );
			return false;
		}
		specialData.linkDstGeo.tScale = atof( value.c_str() );
		specialData.linkDstGeo.LinkAutoTscale = false;
	}
	else if ( key == "linkdstpscale" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			printf( "MeshFace::update(): Error! Missing linkDstPscale parameter\n" );
			return false;
		}
		specialData.linkDstGeo.pScale = atof( value.c_str() );
		specialData.linkDstGeo.LinkAutoPscale = false;
	}
	//
	//  Link failure messages
	//
	else if ( key == "linkSrcShotFail" ) {
		// FIXME: add parsing linkSrcShotFail
	}
	else if ( key == "linkSrcTankFail" ) {
		// FIXME: add parsing linkSrcTankFail
	}
	//
	//  Material
	//
	else if (key == "matref") {
		materials.push_back( value );
		makeMaterial();
	}
	else {
		printf( "MeshFace::update(): Error! unknown mesh face property: %s\n", key.c_str() );
		return false;
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

	// FIXME: special data is read in but not written out

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

void MeshFace::makeMaterial() {
	if (this->mat != NULL)
		mat->unref();

	vector<material*> realmaterials;
	for (vector<string>::iterator itr = materials.begin(); itr != materials.end(); itr++ ) {
		material* mat = (material*)Model::command( MODEL_GET, "material", *itr );
		if (mat != NULL)
			realmaterials.push_back( mat );
		else
			printf("MeshFace::makeMaterial(): Error! Couldn't find material %s\n", (*itr).c_str());
	}

	this->mat = material::computeFinalMaterial( realmaterials );
	this->mat->ref();
}