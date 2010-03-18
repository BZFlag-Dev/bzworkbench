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

#ifndef BZ2OBJECT_H_
#define BZ2OBJECT_H_

//#include "DataEntry.h" - not needed - is included in Tranform.h
#include "render/Renderable.h"
#include "render/Point3D.h"
#include "render/Point2D.h"
#include "Transform.h"

#include "objects/material.h"
#include "objects/physics.h"

#include <vector>
#include <cstring>

using namespace std;

/**
 * This is the base class of a BZW renderable object.  All of these support name, position, rotation, size, matref, phydrv
 * and sets of shear, shift, scale, and spin key/value pairs.  box and pyramid are derived classes of this,
 * but simply do not support transformations
 */

class bz2object : public Renderable, public DataEntry
{
	// allow SceneBuilder to modify bz2objects
	friend class SceneBuilder;

	public:

		// default constructor
		bz2object(const char* name, const char* keys);

		// constructor with node
		bz2object( const char* name, const char* keys, osg::Node* node );

		// destructor
		virtual ~bz2object() { }

		// getter
		string get(void);

		// specific update message
		virtual int update(UpdateMessage& msg);

		// parse a single bzw line
		virtual bool parse( std::string& line );

		// called after done parsing to finalize the changes
		virtual void finalize();

		// toString
		virtual string toString(void);

		// this method only returns the (indented) lines in the BZW text and is meant to be called by derived classes
		static string BZWLines( bz2object* obj );

		// data getters (makes MasterConfigurationDialog code easier)
		osg::ref_ptr<physics> getPhyDrv( std::string slot = "" ) { return physicsSlots[ slot ].phydrv; }
		osg::ref_ptr<BZTransform> getTransformations() { return transformations; }
		vector< material* >& getMaterials( std::string slot = "" ) { return materialSlots[ slot ].materials; }
		bool isSelected() { return selected; }
		bool getFlatshading() { return flatshading; }
		bool getSmoothbounce() { return smoothbounce; }

		// use this instead of getPosition()
		virtual osg::Vec3 getPos() { return getPosition(); }

		// use this instead of getScale()
		virtual osg::Vec3 getSize() { return getScale(); }

		// use this instead of getAttitude()
		virtual osg::Quat getRot() { return getAttitude(); }

		// use this instead of setPosition()
		virtual void setPos( const osg::Vec3d& newPos ) {
			setPosition( newPos );
		}

		// use this instead of setScale()
		virtual void setSize( osg::Vec3 newSize ) { setScale( newSize ); }

		// use this instead of setAttitude()
		virtual void setRot( const osg::Quat& newRot ) { setAttitude( newRot ); }

		// override Renderable's setRotationX() method
		virtual void setRotationX( float x ) {
			return; // x rotation should only be done with transforms
		}

		// override Renderable's setRotationY() method
		virtual void setRotationY( float y ) {
			return; // y rotation should only be done with transforms
		}

		// override Renderable's setRotation() method
		virtual void setRotation( float x, float y, float z ) {
			// only set z rotation, other rotation should be done with a transform
			orientation->setRotation( 0, 0, z );
		}
		virtual void setRotation( const osg::Vec3& rot ) { setRotation( 0, 0, rot.z() ); }

		virtual const osg::Vec3& getRotation() { return orientation->getRotation(); }

		// override Renderable's setRotationZ() method
		virtual void setRotationZ( float r ) { orientation->setRotationZ( r ); }

		// data setters (makes MasterConfigurationDialog code easier)
		void setPhyDrv( physics* phydrv, std::string slot = "" ) { physicsSlots[ slot ].phydrv = phydrv; }
		void setTransforms( osg::ref_ptr<BZTransform> _transformations ) { this->transformations = _transformations; }
		void setMaterials( vector< material* >& _materials, std::string slot = ""  );
		void setSelected( bool value ) { selected = value; }
		void setFlatshading( bool value ) { flatshading = value; updateShadeModel(); }
		void setSmoothbounce( bool value ) { smoothbounce = value; }

		// set/set the thisNode
		osg::Node* getThisNode() { return thisNode.get(); }
		void setThisNode( osg::Node* node ) {
			orientation->removeChild( thisNode.get() );
			thisNode = node;
			orientation->addChild( thisNode.get() );
		}

		// make this public
		bz2object operator =( const bz2object& obj ) {
			bz2object newObj(NULL, NULL);
			memcpy(&newObj, &obj, sizeof(bz2object));
			return newObj;
		}

		// some basic control methods for materials
		void addMaterial( material* mat, std::string slot = "" );
		void insertMaterial( unsigned int index, material* mat, std::string slot = "" );
		void removeMaterial( material* mat, std::string slot = "" );
		void removeMaterial( unsigned int index, std::string slot = "" );

		// snap to grid methods
		void snapTranslate( float size, osg::Vec3 position );
		void snapScale( float size, osg::Vec3 scale );
		void snapRotate( float size, float rotation );

		// methods for material/physics slots
		int materialSlotCount() { return materialSlots.size(); }
		int physicsSlotCount() { return physicsSlots.size(); }
		std::vector<std::string> materialSlotNames();
		std::vector<std::string> physicsSlotNames();

		// struct for material slot data
		struct MaterialSlot {
			std::vector< std::string > alias;
			std::vector< material* > materials;
			osg::StateSet* defaultMaterial;
			osg::Node* node;
		};

		struct PhysicsSlot {
			std::vector< std::string > alias;
			osg::ref_ptr< physics > phydrv;
		};
		
		// set the material of this object from the list of materials
		void refreshMaterial();
	
	protected:
		osg::ref_ptr< BZTransform > transformations;
		// set true if selected in the 3D scene
		bool selected;

		// physics/material slot
		std::map< std::string, PhysicsSlot > physicsSlots;
		std::map< std::string, MaterialSlot > materialSlots; 

		// texsize/offset
		Point2D texsize;
		Point2D texoffset;

		bool drivethrough;
		bool shootthrough;
		bool flatshading;
		bool smoothbounce;

	private:
		// force these methods to be private, to guarantee that derived classes will use the given replacements
		osg::Vec3f getPosition() { return orientation->getPosition(); }
		osg::Vec3f getScale() { return orientation->getScale(); }
		osg::Quat getAttitude() { return orientation->getAttitude(); }
		void setPosition( const osg::Vec3d& newPosition ) { orientation->setPosition( newPosition ); }
		void setScale( const osg::Vec3d& newScale ) { orientation->setScale( newScale ); }
		void setAttitude( const osg::Quat& newAttitude ) { orientation->setAttitude( newAttitude ); }

		// update the shade model based on flatshading
		void updateShadeModel();

		// reference to node data inside the Renderable (for changing the transformation stack)
		osg::ref_ptr< osg::Node > thisNode;

		// saved state set
		osg::ref_ptr< osg::StateSet > savedStateSet;

		// the orientation of the bz2object
		osg::ref_ptr< Renderable > orientation;
};

#include "physics.h"
#include "material.h"

#endif /*BZ2OBJECT_H_*/
