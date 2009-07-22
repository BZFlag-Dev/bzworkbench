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

#include "Transform.h"

#include "model/Model.h"

using namespace std;

BZTransform::BZTransform() : DataEntry("", ""), osg::MatrixTransform() {
	this->name = string("");
	this->data = vector<TransformData>();
	//this->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
}

BZTransform::BZTransform( string _name, vector<TransformData>& _data ) : DataEntry("", ""), osg::MatrixTransform() {
	this->name = _name;
	this->data = _data;
	this->refreshMatrix();
	//this->setReferenceFrame( osg::Transform::ABSOLUTE_RF );
}

// getter
string BZTransform::get(void) {
	return toString();
}

// setter
int BZTransform::parse(string& newData) {
	// expect just one line
	string key = BZWParser::key(newData.c_str());

	// break up the line
	vector<string> elements = BZWParser::getLineElements(newData.c_str());

	TransformData d;

	// parse transform type
	if ( key == "shift" )
		d.type == ShiftTransform;
	else if ( key == "spin" )
		d.type = SpinTransform;
	else if ( key == "shear" )
		d.type = ShearTransform;
	else if ( key == "scale" )
		d.type == ScaleTransform;
	else
		throw BZWReadError( this, string( "Unknown transform type, " ) + key );


	// get the numbers (don't start with elements.begin(), because this is what name is)
	osg::Vec4 vec;
	for(int i = 0; i < elements.size() && i < 4; i++) {
		vec[ i ] = atof( elements[ i ].c_str() );
	}

	d.data = vec;

	data.push_back( d );

	return true;
}

// toString
string BZTransform::toString(void) {
	string ret = string(name);
	/*for(vector<float>::iterator i = data.begin(); i != data.end(); i++) {
		ret += " " + string(ftoa(*i));
	}*/

	return ret + "\n";
}

// data getters
string BZTransform::getName() { return name; }

// data setters
void BZTransform::setName(string& s) { name = s; refreshMatrix(); }

// make this operator public
BZTransform BZTransform::operator =( const BZTransform& obj ) {
	BZTransform newObj = BZTransform();
	memcpy(&newObj, &obj, sizeof(BZTransform));
	return newObj;
}

// make this into a shift matrix
void BZTransform::makeShift( osg::Vec4& value ) {

	osg::Matrixd theMatrix;
	theMatrix.makeTranslate( osg::Vec3( value[0], value[1], value[2] ) );

	matrix *= theMatrix;
}

// make this into a scale matrix
void BZTransform::makeScale( osg::Vec4& value ) {
	osg::Matrixd theMatrix;
	theMatrix.makeScale( value[0], value[1], value[2] );

	matrix *= theMatrix;
}

// make this into a spin matrix
void BZTransform::makeSpin( osg::Vec4& value ) {
	// normalize
	osg::Vec3 n( value[1], value[2], value[3] );
	n.normalize();

	// setup
	const float cos_val = cosf(value[0]);
	const float sin_val = sinf(value[0]);
	const float icos_val = (1.0f - cos_val);
	osg::Matrixd m;

	m(3, 3) = 1.0f;
	m(0, 3) = m(1, 3) = m(2, 3) = 0.0f;
	m(3, 0) = m(3, 1) = m(3, 2) = 0.0f;
	m(0, 0) = (n[0] * n[0] * icos_val) + cos_val;
	m(0, 1) = (n[0] * n[1] * icos_val) - (n[2] * sin_val);
	m(0, 2) = (n[0] * n[2] * icos_val) + (n[1] * sin_val);
	m(1, 0) = (n[1] * n[0] * icos_val) + (n[2] * sin_val);
	m(1, 1) = (n[1] * n[1] * icos_val) + cos_val;
	m(1, 2) = (n[1] * n[2] * icos_val) - (n[0] * sin_val);
	m(2, 0) = (n[2] * n[0] * icos_val) - (n[1] * sin_val);
	m(2, 1) = (n[2] * n[1] * icos_val) + (n[0] * sin_val);
	m(2, 2) = (n[2] * n[2] * icos_val) + cos_val;

	// execute
	matrix *= m;
}

// make this into a shear matrix
void BZTransform::makeShear( osg::Vec4& value ) {
	double matvals[] = {
		1.0,		0.0,		value[2],	0.0,
		0.0,		1.0,		0.0,		0.0,
		value[0],	value[1],	1.0,		0.0,
		0.0,		0.0,		0.0,		1.0
	};

	osg::Matrixd m = osg::Matrixd( matvals );
	matrix *= m;
}


void BZTransform::refreshMatrix() {
	// reset matrix
	matrix = osg::Matrixd::identity();

	for ( vector<TransformData>::iterator i = data.begin(); i != data.end(); i++ ) {
		if( i->type == ShiftTransform )
			makeShift( i->data );
		else if( i->type == SpinTransform )
			makeSpin( i->data );
		else if( i->type == ShearTransform )
			makeShear( i->data );
		else if( i->type == ScaleTransform )
			makeScale( i->data );
	}

	setMatrix( matrix );
}