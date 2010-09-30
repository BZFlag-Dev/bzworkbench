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

#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include <cstring>
#include <vector>

#include "DataEntry.h"
#include "model/BZWParser.h"

#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>

enum TransformType {
  ShiftTransform = 0,
  ScaleTransform = 1,
  ShearTransform = 2,
  SpinTransform  = 3
};

struct TransformData {
  TransformType type;
  osg::Vec4 data;
};


// a metadata entry that is used to describe a transformation such as spin, shear, shift, and scale
class BZTransform : public DataEntry, public osg::MatrixTransform {

	public:

		BZTransform();
		BZTransform( string _name, vector<TransformData>& _data );
		

		// getter
		string get(void);

		// setter
		int parse(string& newData);

		// toString
		string toString(void);

		// data getters
		string getName();
		vector<TransformData> getData() { return data; }

		// data setters
		void setName(string& s);
		void setData(vector<TransformData>& d) { data = d; }

		// make this operator public
		BZTransform operator =( const BZTransform& obj );

		// transformation adders
		void addShift( osg::Vec3& value );
		void addScale( osg::Vec3& value );
		void addSpin( osg::Vec3& value );
		void addShear( osg::Vec3& value );

		void refreshMatrix();

	private:

		// make this into a shift matrix
		void makeShift( osg::Vec4& value );

		// make this into a scale matrix
		void makeScale( osg::Vec4& value );

		// make this into a spin matrix
		void makeSpin( osg::Vec4& value );

		// make this into a shear matrix
		void makeShear( osg::Vec4& value );


		// member data
		string name;
		osg::Matrixd matrix;
		vector<TransformData> data;
};

#endif /*TRANSFORM_H_*/
