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

#include "objects/pyramid.h"

#include "model/Primitives.h"

const char* pyramid::faceNames[FaceCount] = {
  "x+",
  "x-",
  "y+",
  "y-",
  "bottom"
};

pyramid::pyramid() : bz2object("pyramid", "<name><position><rotation><size><shift><shear><scale><spin><matref><phydrv><drivethrough><shootthrough><passable><flipz>") {
	setDefaults();
}

void pyramid::setDefaults() {
	flipz = false;
	updateGeometry();

	osg::Group* group = (osg::Group*)getThisNode();

	for (int i = 0; i < FaceCount; i++) {
		texSizes[i] = Point2D(-8.0f, -8.0f);
		texOffsets[i] = Point2D(0.0f, 0.0f);
		driveThroughs[i] = false;
		shootThroughs[i] = false;
		ricochets[i] = false;

		// set default textures
		SceneBuilder::assignTexture( "pyrwall", group->getChild(i) );

		// add material/physics slot
		MaterialSlot mslot;
		PhysicsSlot pslot;
		if (i <= YN) {
			mslot.alias.push_back("sides");
			pslot.alias.push_back("sides");
			mslot.alias.push_back("edge");
			pslot.alias.push_back("edge");
		}
		else if (i == ZN) {
			pslot.alias.push_back("bottom");
		}
		mslot.defaultMaterial = group->getChild(i)->getStateSet();
		mslot.node = group->getChild(i);
		pslot.phydrv = NULL;

		materialSlots[ string( faceNames[i] ) ] = mslot;
		physicsSlots[ string( faceNames[i] ) ] = pslot;	
	}

	this->setPos( osg::Vec3(0.0, 0.0, 0.0) );
	this->setSize( osg::Vec3(10.0, 10.0, 10.0) );

}

// getter
string pyramid::get() { return toString(); }

// setter with messaging
// NOTE: don't call the superclass method, because it deals solely with transformations (which are n/a here)
int pyramid::update(UpdateMessage& message) {
	int result = bz2object::update( message );

	switch( message.type ) {
		case UpdateMessage::SET_POSITION: 	// handle a new position
			setPos( *(message.getAsPosition()) );
			break;

		case UpdateMessage::SET_POSITION_FACTOR:	// handle a translation
			setPos( getPos() + *(message.getAsPositionFactor()) );
			break;

		case UpdateMessage::SET_ROTATION:		// handle a new rotation
			setRotationZ( message.getAsRotation()->z() );
			break;

		case UpdateMessage::SET_ROTATION_FACTOR:	// handle an angular translation
			setRotationZ( getRotation().z() + message.getAsRotationFactor()->z() );
			break;

		case UpdateMessage::SET_SCALE:		// handle a new scale
			setSize( *(message.getAsScale()) );
			break;

		case UpdateMessage::SET_SCALE_FACTOR:	// handle a scaling factor
			setSize( getSize() + *(message.getAsScaleFactor()) );
			break;

		case UpdateMessage::REMOVE_MATERIAL: {
			material* mat = message.getAsMaterial();
			for ( int i = 0; i < FaceCount; i++ ) 
				if ( ((osg::Group*)getThisNode())->getChild( i )->getStateSet() == mat )
					((osg::Group*)getThisNode())->getChild( i )->setStateSet( NULL );
			break;
		}

		default:	// unknown event; don't handle
			return result;
	}

	return 1;
}

// bzw methods
bool pyramid::parse( std::string& line ) {
	// first check if this is the end
	if ( line == "end" )
		return false;

	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );

	// meshpyr is just an alias for pyramid
	if ( key == "meshpyr" )
		return true;
	
	if ( key == "flipz" ) {
		setFlipz(true);
		return true;
	}
	
	// first parse per face keys
	for ( int i = 0; i < FaceCount; i++ ) {
		if ( key == faceNames[i] ) {
			string realKey = BZWParser::key( value.c_str() );

			if ( realKey == "texsize" ) {
				texSizes[i] = Point2D( value.c_str() );
			}
			else if ( realKey == "texoffset" ) {
				texOffsets[i] = Point2D( value.c_str() );
			}
			else if ( realKey == "drivethrough" ) {
				driveThroughs[i] = true;
			}
			else if ( realKey == "shootthrough" ) {
				shootThroughs[i] = true;
			}
			else if ( realKey == "passable" ) {
				driveThroughs[i] = true;
				shootThroughs[i] = true;
			}
			else if ( realKey == "ricochet" ) {
				ricochets[i] = true;
			}
			else {
				break;
			}

			return true;
		}
	}

	return bz2object::parse( line );
}

void pyramid::finalize() {
	// just regen UV coords based on any size changes
	Primitives::rebuildPyramidUV( (osg::Group*)getThisNode(), getSize() );
	refreshMaterial();
}

// toString
string pyramid::toString(void) {
	string ret = "pyramid\n";
	ret += BZWLines( this );
	if (flipz)
		ret += "  flipz\n";
	ret += "end\n";
	return ret;
}

void pyramid::setSize( osg::Vec3 newSize ) {
	Primitives::rebuildPyramidUV( (osg::Group*)getThisNode(), newSize );
	bz2object::setSize( newSize );
}

Point2D pyramid::getTexsize( int face ) {
	return texSizes[face];
}

Point2D pyramid::getTexoffset( int face ) {
	return texOffsets[face];
}

bool pyramid::getDrivethrough( int face ) {
	return driveThroughs[face];
}

bool pyramid::getShootthrough( int face ) {
	return shootThroughs[face];
}

bool pyramid::getRicochet( int face ) {
	return ricochets[face];
}

bool pyramid::getFlipz() {
	return flipz;
}


void pyramid::setTexsize( int face, Point2D value ) {
	texSizes[face] = value;
}

void pyramid::setTexoffset( int face, Point2D value ) {
	texOffsets[face] = value;
}

void pyramid::setDrivethrough( int face, bool value ) {
	driveThroughs[face] = value;
}

void pyramid::setShootthrough( int face, bool value ) {
	shootThroughs[face] = value;
}

void pyramid::setRicochet( int face, bool value ) {
	ricochets[face] = value;
}

void pyramid::setFlipz( bool value ) {
	flipz = value;
	updateGeometry();
	finalize();
}

void pyramid::updateGeometry() {
	osg::Node* node = Primitives::buildPyramid( osg::Vec3( 1, 1, 1 ), getFlipz() );
	setThisNode( node );
	
	//loop thru material slots an update the nodes
	for (int i = 0; i < FaceCount; i++) {
		materialSlots[ string( faceNames[i] ) ].node = ((osg::Group*)getThisNode())->getChild(i);
	}
}
