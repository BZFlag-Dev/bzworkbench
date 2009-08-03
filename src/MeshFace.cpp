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
	physicsDriver = NULL;
	driveThrough = shootThrough = smoothbounce = false;
	mat = NULL;
}

// constructor with data
MeshFace::MeshFace(material* mat, physics* phydrv, bool noclusters, bool smoothbounce, bool drivethrough, bool shootthrough) : DataEntry("face", "<vertices><normals><texcoords><phydrv><matref><drivethrough><shootthrough><passable>") {
	vertices = vector<int>();
	normals = vector<int>();
	texcoords = vector<int>();
	physicsDriver = phydrv;
	this->driveThrough = drivethrough;
	this->shootThrough = shootthrough;
	this->smoothbounce = smoothbounce;
	this->mat = mat;

	//this->update(data);
}

MeshFace::~MeshFace() {
	mat->unref();
}

// getter
string MeshFace::get(void) { return this->toString(); }

bool MeshFace::parse( string& line ) {
	// check if we're at the end of the face
	if ( line == "endface" )
		return false;

	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );

	if ( key == "vertices" ) {
		vertices = BZWParser::getIntList( value.c_str() );
		if ( vertices.size() < 3 ) {
			throw BZWReadError( this, "Faces need at least 3 vertices." );
		}
	}
	else if ( key == "normals" ) {
		normals = BZWParser::getIntList( value.c_str() );
		if ( normals.size() < 3 ) {
			throw BZWReadError( this, "Faces need at least 3 normals." );
		}
	}
	else if ( key == "texcoords" ) {
		texcoords = BZWParser::getIntList( value.c_str() );
		if ( texcoords.size() < 3 ) {
			throw BZWReadError( this, "Faces need at least 3 texcoords." );
		}
	}
	else if ( key == "phydrv" ) {
		string drvname = BZWParser::value( "phydrv", line.c_str() );
		physics* phys = (physics*)Model::command( MODEL_GET, "phydrv", drvname.c_str() );
		if (phys != NULL)
			physicsDriver = phys;
		else
			throw BZWReadError( this, string( "Couldn't find physics driver, " ) + drvname );
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
			throw BZWReadError( this, "Missing baseteam parameter." );
		}
		specialData.baseTeam = atoi( value.c_str() );
	}
	else if ( key == "linkname" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			throw BZWReadError( this, "Missing linkname parameter." );
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
			throw BZWReadError( this, "Missing linkSrcCenter index." );
		}
		specialData.linkSrcGeo.centerIndex = atoi( value.c_str() );
	}
	else if ( key == "linksrcsdir" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			throw BZWReadError( this, "Missing linkSrcSdir index." );
		}
		specialData.linkSrcGeo.sDirIndex = atoi( value.c_str() );
	}
	else if ( key == "linksrctdir" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			throw BZWReadError( this, "Missing linkSrcTdir index" );
		}
		specialData.linkSrcGeo.tDirIndex = atoi( value.c_str() );
	}
	else if ( key == "linksrcpdir" ) {
		string value = BZWParser::value( "linksrcpdir", line.c_str() );
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			throw BZWReadError( this, "Missing linkSrcPdir index" );
		}
		specialData.linkSrcGeo.pDirIndex = atoi( value.c_str() );
	}
	else if ( key == "linksrcsscale" ) {
		string value = BZWParser::value( "linksrcsscale", line.c_str() );
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			throw BZWReadError( this, "Missing linkSrcSscale parameter" );
		}
		specialData.linkSrcGeo.sScale = atof( value.c_str() );
		specialData.linkSrcGeo.LinkAutoSscale = false;
	}
	else if ( key == "linksrctscale" ) {
		string value = BZWParser::value( "linksrctscale", line.c_str() );
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			throw BZWReadError( this, "Missing linkSrcTscale parameter" );
		}
		specialData.linkSrcGeo.tScale = atof( value.c_str() );
		specialData.linkSrcGeo.LinkAutoTscale = false;
	}
	else if ( key == "linksrcpscale" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			throw BZWReadError( this, "Missing linkSrcPscale parameter" );
		}
		specialData.linkSrcGeo.pScale = atof( value.c_str() );
		specialData.linkSrcGeo.LinkAutoPscale = false;
	}
	//
	//  Link destination parameters
	//
	else if ( key == "linkdstcenter" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			throw BZWReadError( this, "Missing linkDstCenter index" );
		}
		specialData.linkDstGeo.centerIndex = atoi( value.c_str() );
	}
	else if ( key == "linkdstsdir" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			throw BZWReadError( this, "Missing linkDstSdir index" );
		}
		specialData.linkDstGeo.sDirIndex = atoi( value.c_str() );
	}
	else if ( key == "linkdsttdir" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			throw BZWReadError( this, "Missing linkDstTdir index" );
		}
		specialData.linkDstGeo.tDirIndex = atoi( value.c_str() );
	}
	else if ( key == "linkdstpdir" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			throw BZWReadError( this, "Missing linkDstPdir index" );
		}
		specialData.linkDstGeo.pDirIndex = atoi( value.c_str() );
	}
	else if ( key == "linkdstsscale" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			throw BZWReadError( this, "Missing linkDstSscale parameter" );
		}
		specialData.linkDstGeo.sScale = atof( value.c_str() );
		specialData.linkDstGeo.LinkAutoSscale = false;
	}
	else if ( key == "linkdsttscale" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			throw BZWReadError( this, "Missing linkDstTscale parameter" );
		}
		specialData.linkDstGeo.tScale = atof( value.c_str() );
		specialData.linkDstGeo.LinkAutoTscale = false;
	}
	else if ( key == "linkdstpscale" ) {
		if ( BZWParser::allWhitespace( value.c_str() ) ) {
			throw BZWReadError( this, "Missing linkDstPscale parameter" );
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
	else if ( key == "matref" ) {
		material* mat = (material*)Model::command( MODEL_GET, "material", value );
		if (mat != NULL)
			this->mat = mat;
		else
			throw BZWReadError( this, string( "Couldn't find material, " ) + value );
	}
	else {
		throw BZWReadError( this, string( "Unknown key, " ) + key );
	}

	return true;
}

// toString
string MeshFace::toString(void) {
	// FIXME: some data is read in but not written out
	string ret("face\n");
	ret += (vertices.size() > 0 ? "    vertices " + stringify(vertices) + "\n" : "");
	ret += (texcoords.size() > 0 ? "    texcoords " + stringify(texcoords) + "\n" : "");
	ret += (normals.size() > 0 ? "    normals " + stringify(normals) + "\n" : "");
	ret += (mat != NULL ? "    matref " + mat->getName() + "\n" : "");
	ret += (physicsDriver != NULL ? "    phydrv " + physicsDriver->getName() + "\n" : "");
	ret += (shootThrough == true ? "    shootthrough\n" : "");
	ret += (driveThrough == true ? "    drivethrough\n" : "");
	ret += "  endface\n";

	return ret;
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
