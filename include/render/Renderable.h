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

#ifndef RENDERABLE_H_
#define RENDERABLE_H_

#include <osg/Group>
#include <osg/PositionAttitudeTransform>
#include <osg/Node>

#include "../model/SceneBuilder.h"

// this is a subclass of osg::PositionAttitudeTransform.  It is designed to be an easily-manipulatible node

class Renderable : public osg::PositionAttitudeTransform {
	
public:
	
	// default constructor
	Renderable() : osg::PositionAttitudeTransform() {
		this->setName("");
	}
	
	// constructor with name
	Renderable( const char* name ) : osg::PositionAttitudeTransform() {
		this->setName( name );
	}
	
	// constructor with a child node to add
	Renderable( osg::Node* child ) : osg::PositionAttitudeTransform() {
		if(child) {
			this->setName( child->getName() );
			this->addChild( child );
		}
		else {
			this->setName( "(unknown)" );
		}
	}
	
	virtual ~Renderable() { }
	
	// getters
	osg::ref_ptr< osg::Node > getNode() { return this->getChild( 0 ); }
	
	// set the child node (there should only be one)
	void setNode( osg::Node* node ) {
		if(this->getNumChildren() > 0 && this->getChild(0) != NULL)
			this->removeChild( this->getChild(0) );
		
		this->addChild( node );
	}
	
	// angular rotators (easier than building quaterions and setting the attitude
	// angles is in degrees, not radians
	virtual void setRotationX( float r ) {
		this->rotation.set( r, this->rotation.y(), this->rotation.z() );
		this->setAttitude( osg::Quat( osg::DegreesToRadians( this->rotation.x() ), osg::Vec3( 1.0, 0.0, 0.0 ),
									  osg::DegreesToRadians( this->rotation.y() ), osg::Vec3( 0.0, 1.0, 0.0 ),
									  osg::DegreesToRadians( this->rotation.z() ), osg::Vec3( 0.0, 0.0, 1.0 ) ) );
	}
	virtual void setRotationY( float r ) {
		this->rotation.set( this->rotation.x(), r, this->rotation.z() );
		this->setAttitude( osg::Quat( osg::DegreesToRadians( this->rotation.x() ), osg::Vec3( 1.0, 0.0, 0.0 ),
									  osg::DegreesToRadians( this->rotation.y() ), osg::Vec3( 0.0, 1.0, 0.0 ),
									  osg::DegreesToRadians( this->rotation.z() ), osg::Vec3( 0.0, 0.0, 1.0 ) ) );
	}
	virtual void setRotationZ( float r ) {
		this->rotation.set( this->rotation.x(), this->rotation.y(), r );
		this->setAttitude( osg::Quat( osg::DegreesToRadians( this->rotation.x() ), osg::Vec3( 1.0, 0.0, 0.0 ),
									  osg::DegreesToRadians( this->rotation.y() ), osg::Vec3( 0.0, 1.0, 0.0 ),
									  osg::DegreesToRadians( this->rotation.z() ), osg::Vec3( 0.0, 0.0, 1.0 ) ) );
	}
	virtual void setRotation( float x, float y, float z ) {
		this->rotation.set( x, y, z);
		this->setAttitude( osg::Quat( osg::DegreesToRadians( this->rotation.x() ), osg::Vec3( 1.0, 0.0, 0.0 ),
									  osg::DegreesToRadians( this->rotation.y() ), osg::Vec3( 0.0, 1.0, 0.0 ),
									  osg::DegreesToRadians( this->rotation.z() ), osg::Vec3( 0.0, 0.0, 1.0 ) ) );
	}
	
	virtual void setRotation( const osg::Vec3d& rot ) { this->setRotation( rot.x(), rot.y(), rot.z() ); }
	
	// getters for rotation (make sure it can't change)
	virtual const osg::Vec3& getRotation() { return this->rotation; }
	
protected:
	
	// angular orientation around 3D axes
	osg::Vec3 rotation;
	
};

#endif /*RENDERABLE_H_*/
