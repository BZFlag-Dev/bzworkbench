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

#ifndef TEXTUREMATRIX_H_
#define TEXTUREMATRIX_H_

#include "../DataEntry.h"
#include "../model/BZWParser.h"
#include "../render/TexCoord2D.h"

class texturematrix : public DataEntry {

public:

	// default constructor
	texturematrix();

	static DataEntry* init() { return new texturematrix(); }

	// getter
	string get(void);

	// bzw methods
	bool parse( std::string& line );
	void finalize();

	// toString
	string toString(void);

	// render
	int render(void);

	// getters
	string& getName() { return this->name; }

	// setters
	void setName( const string& _name );

	TexCoord2D getScale() { return texScale; }
	TexCoord2D getScaleFreq() { return texFreq; }
	TexCoord2D getShift() { return texShift; }
	TexCoord2D getCenter() { return texCenter; }
	TexCoord2D getFixedScale() { return texFixedScale; }
	TexCoord2D getFixedShift() { return texFixedShift; }
	TexCoord2D getFixedCenter() { return texFixedCenter; }
	float getSpin() { return spin; }
	float getFixedSpin() { return fixedSpin; }

	void setScale( TexCoord2D value ) { texScale = value; }
	void setScaleFreq( TexCoord2D value ) { texFreq = value; }
	void setShift( TexCoord2D value ) { texShift = value; }
	void setCenter( TexCoord2D value ) { texCenter = value; }
	void setFixedScale( TexCoord2D value ) { texFixedScale = value; }
	void setFixedShift( TexCoord2D value ) { texFixedShift = value; }
	void setFixedCenter( TexCoord2D value ) { texFixedCenter = value; }
	void setSpin( float value ) { spin = value; }
	void setFixedSpin( float value ) { fixedSpin = value; }

private:

	string name;
	TexCoord2D texFreq, texScale, texShift, texCenter, texFixedScale, texFixedShift, texFixedCenter;
	float spin, fixedSpin;

};


#endif /*TEXTUREMATRIX_H_*/
