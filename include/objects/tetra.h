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

#ifndef TETRAHEDRON_H_
#define TETRAHEDRON_H_

#include "bz2object.h"

class tetra : public bz2object {
public:
	
	// no arg constructor
	tetra();
	
	static DataEntry* init() { return new tetra(); }
	
	// getter
	string get(void);

	// bzw methods
	bool parse( std::string& line );
	void finalize();
	
	// tostring
	string toString(void);
	
	// render
	int render(void);
	
private:
	Point3D vertexes[4];
};


#endif /*TETRAHEDRON_H_*/
