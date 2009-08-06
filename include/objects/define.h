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

#ifndef DEFINE_H_
#define DEFINE_H_

#include "bz2object.h"

class define : public DataEntry {
	
public:
	
	// constructor
	define();
	
	// static constructor
	static DataEntry* init() { return new define(); }
	
	// destructor
	~define();
	
	// getter
	string get(void);

	// bzw methods
	bool parse( std::string& line );
	void finalize();
	
	// toString
	string toString(void);
	
	// getters
	string& getName() { return this->name; }
	vector< osg::ref_ptr< bz2object > >& getObjects() { return this->objects; }
	
	// setters
	void setName( const string& _name );
	void setObjects( vector< osg::ref_ptr< bz2object > >& _objects ) { this->objects = _objects; }
	
private:
	// name
	string name;

	// the objects in the definition 
	vector< osg::ref_ptr< bz2object > > objects;

	// used for parsing bzw;
	bz2object* currentObject;
};

#endif /*DEFINE_H_*/
