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

#include "objects/texturematrix.h"

#include "model/SceneBuilder.h"
#include "model/Model.h"

// default constructor
texturematrix::texturematrix() :
	DataEntry("texturematrix", "<name><scale><spin><shift><center><fixedscale><fixedspin><fixedshift><fixedcenter>") {


	name = SceneBuilder::makeUniqueName("texturematrix");
	fixedSpin = 0.0f;
	texCenter = TexCoord2D();
	texFixedScale = TexCoord2D();
	texFixedShift = TexCoord2D();
	texFreq = TexCoord2D();
	texScale = TexCoord2D();
	texShift = TexCoord2D();
	spin = 0.0f;
}

// getter
string texturematrix::get(void) { return toString(); }

// bzw methods
bool texturematrix::parse( std::string& line ) {
	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );
	
	// check if we reached the end of the section
	if ( key == "end" )
		return false;

	if ( key == "name" ) {
		name = value;
	}
	else if ( key == "scale" ) {
		vector<string> texScaleLineElements = BZWParser::getLineElements( value.c_str() );
		string texFreqs = texScaleLineElements[0] + " " + texScaleLineElements[1];
		string texScales = texScaleLineElements[2] + " " + texScaleLineElements[3];
		texScale = TexCoord2D( texScales.c_str() );
		texFreq = TexCoord2D( texFreqs.c_str() );
	}
	else if ( key == "spin" ) {
		spin = atof( value.c_str() );
	}
	else if ( key == "shift" ) {
		texShift = TexCoord2D( value.c_str() );
	}
	else if ( key == "center" ) {
		texCenter = TexCoord2D( value.c_str() );
	}
	else if ( key == "fixedscale" ) {
		texFixedScale = TexCoord2D( value.c_str() );
	}
	else if ( key == "fixedspin" ) {
		fixedSpin = atof( value.c_str() );
	}
	else if ( key == "fixedshift" ) {
		texFixedShift = TexCoord2D( value.c_str() );
	}
	else if ( key == "fixedcenter" ) {
		texFixedCenter = TexCoord2D( value.c_str());
	}
	else {
		throw BZWReadError( this, string( "Unknown key, " ) + key );
	}

	return true;
}

void texturematrix::finalize() {

}

// toString
string texturematrix::toString(void) {
	return string("texturematrix\n") +
				  "  name " + name + "\n" +
				  "  scale " + texFreq.toString() + " " + texScale.toString() + "\n" +
				  "  spin " + string(ftoa(spin)) + "\n" +
				  "  shift " + texShift.toString() + "\n" +
				  "  center " + texCenter.toString() + "\n" +
				  "  fixedscale " + texFixedScale.toString() + "\n" +
				  "  fixedspin " + string(ftoa(fixedSpin)) + "\n" +
				  "  fixedshift " + texFixedShift.toString() + "\n" +
				  "  fixedcenter " + texFixedCenter.toString() + "\n" +
				  "end\n";
}

// render
int texturematrix::render(void) {
	return 0;

}

void texturematrix::setName( const string& _name ) {
	if (_name != getName()){
		if (Model::renameTextureMatrix( getName(), _name )) {
			this->name = _name;
		}
	}
}
