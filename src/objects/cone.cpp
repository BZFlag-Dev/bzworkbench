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

#include "objects/cone.h"

#include <osg/Node>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PrimitiveSet>
#include <osg/StateSet>
#include <osg/StateAttribute>
#include <osg/ShadeModel>

const char* cone::sideNames[MaterialCount] = {
  "edge",
  "bottom",
  "startside",
  "endside"
};

// default constructor
cone::cone() :
	bz2object("cone", "<position><rotation><size><angle><flatshading><name><texsize><flipz><divisions><shift><shear><spin><scale><smoothbounce><phydrv><matref><drivethrough><shootthrough><passable>") {
	setDefaults();
}

void cone::setDefaults() {
	// define some basic values
	divisions = 16;
	flatshading = false;
	smoothbounce = true;
	pyramidStyle = false;
	angle = 0.0f;
	sweepAngle = 360.0f;
	flipz = false;
	
	// make group and geodes
	osg::Group* group = new osg::Group();
	for (int i = 0; i < MaterialCount; i++) {
		group->addChild( new osg::Geode() );

		string texture;
		if ( i == Edge ) {
			texture = "boxwall";
		}
		else if ( i == Bottom ) {
			texture = "roof";
		}
		else {
			texture = "wall";
		}

		SceneBuilder::assignTexture( texture.c_str(), group->getChild( i ) );

		MaterialSlot mslot;
		mslot.defaultMaterial = group->getChild( i )->getStateSet();
		mslot.node = group->getChild( i );
		materialSlots[ string( sideNames[i] ) ] = mslot;
	}

	setThisNode( group );

	// default size is 10x10x10
	setSize( osg::Vec3( 10, 10, 10 ) );

	// build the geometry
	buildGeometry();
}

// getter
string cone::get(void) { return toString(); }

// bzw methods
bool cone::parse( std::string& line ) {
	// check if we reached the end of the section
	if ( line == "end" )
		return false;

	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );

	// parse keys
	if ( key == "flipz" ) {
		setFlipz(true);
	}
	else if ( key == "divisions" ) {
		divisions = atof( value.c_str() );
	}
	else if ( key == "angle" ) {
		sweepAngle = atof( value.c_str() );
	}
	else if ( key == "rotation" ) {
		angle = atof( value.c_str() );
	}
	else {
		return bz2object::parse( line );
	}

	return true;
}

void cone::finalize() {
	buildGeometry();
	bz2object::finalize();
}

// event handler
int cone::update( UpdateMessage& message ) {

	// superclass event handler
	int result = bz2object::update( message );

	switch( message.type ) {
		case UpdateMessage::SET_POSITION: 	// handle a new position
			setPos( *(message.getAsPosition()) );
			break;

		case UpdateMessage::SET_POSITION_FACTOR:	// handle a translation
			setPos( getPos() + *(message.getAsPositionFactor()) );
			break;

		case UpdateMessage::SET_ROTATION:		// handle a new rotation
			// an cone's rotation is not the z rotation of the cone object
			//setRotation( *(message.getAsRotation()) );
			setSweepRotation(message.getAsRotation()->z());
			buildGeometry();
			break;

		case UpdateMessage::SET_ROTATION_FACTOR:	// handle an angular translation
			//setRotation( getRotation() + *(message.getAsRotationFactor()) );
			setSweepRotation( getSweepRotation() + message.getAsRotation()->z());
			buildGeometry();
			break;

		case UpdateMessage::SET_SCALE:		// handle a new scale
			setSize( *(message.getAsScale()) );
			break;

		case UpdateMessage::SET_SCALE_FACTOR:	// handle a scaling factor
			setSize( getSize() + *(message.getAsScaleFactor()) );
			break;
		case UpdateMessage::REMOVE_MATERIAL: {
			material* mat = message.getAsMaterial();
			for ( int i = 0; i < MaterialCount; i++ ) 
				if ( ((osg::Group*)getThisNode())->getStateSet() == mat )
					((osg::Group*)getThisNode())->setStateSet( NULL );
			break;
		}
		default:	// unknown event; don't handle
			return result;
	}

	return 1;
}


// toString
string cone::toString(void) {
    string ret;
	if (!pyramidStyle)
		ret += "cone\n";
	else
		ret += "meshpyr\n";

	ret += BZWLines( this );

	// materials
	for (int i = 0; i < MaterialCount; i++) {
		osg::Group* group = (osg::Group*)getThisNode();
		osg::StateSet* stateset = group->getChild(i)->getStateSet();
		if (stateset->getName() != "")
			ret += "  " + string(sideNames[i]) + " matref " + stateset->getName();
	}

	// some options shouldn't be included for a meshpyr
	if (!pyramidStyle)
		ret += "  divisions " + string(itoa(divisions)) + "\n" +
			   "  angle " + string(ftoa(sweepAngle) ) + "\n";

	if(getSweepRotation() != 0.0f){
		ret += "  rotation " + string(ftoa(getSweepRotation()) ) + "\n"; 
	}
	ret += string("") +
		   (flatshading == true ? "  flatshading\n" : "") +
		   (smoothbounce == true ? "  smoothbounce\n" : "") +
		   "  texsize " + texsize.toString() + //"\n" +
		   (flipz == true ? "  flipz\n" : "") +
		   "end\n";

	return ret;
}

void cone::setSweepAngle(float value) {
	if( value != sweepAngle ) {		// refresh the geometry
		buildGeometry();
	}	
	sweepAngle = value;
}

void cone::setSweepRotation(float value) {
	if( value != angle ) {		// refresh the geometry
		buildGeometry();
	}	
	angle = value;
}
	
void cone::setDivisions(int value) {
	if( value != divisions ) {	// refresh the geometry
		buildGeometry();
	}
		
	divisions = value;
}

// build the cone geometry
void cone::buildGeometry() {
	// make the group
	osg::Group* theCone = (osg::Group*)getThisNode();
	osg::Geode* coneNode = (osg::Geode*)theCone->getChild( 0 );
	if (coneNode->getNumDrawables() > 0) coneNode->removeDrawables( 0 );
	osg::Geode* baseNode = (osg::Geode*)theCone->getChild( 1 );
	if (baseNode->getNumDrawables() > 0) baseNode->removeDrawables( 0 );
	osg::Geode* startNode = (osg::Geode*)theCone->getChild( 2 );
	if (startNode->getNumDrawables() > 0) startNode->removeDrawables( 0 );
	osg::Geode* endNode = (osg::Geode*)theCone->getChild( 3 );
	if (endNode->getNumDrawables() > 0) endNode->removeDrawables( 0 );

	// geometry data for the conical component
	osg::Vec3Array* points = new osg::Vec3Array();
	osg::DrawElementsUInt* indices = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLE_FAN, 0 );
	osg::Vec2Array* texCoords = new osg::Vec2Array();

	// geometry for the base of the cone
	osg::DrawElementsUInt* baseIndices = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLE_STRIP, 0 );
	osg::Vec2Array* baseTexCoords = new osg::Vec2Array();

	float ztop = 1;
	float zbottom = 0;
	if(flipz){
		ztop = 0;
		zbottom = 1;
	}
	
   	// add the tip of the cone to the conical geometry
   	points->push_back( osg::Vec3( 0.0, 0.0, ztop ) );	// the tip of the cone
   	indices->push_back( 0 );	// the index of the tip of the cone
   	texCoords->push_back( osg::Vec2( 0.5, 0.5 ) );	// texture coordinate of the tip of the cone
   	baseTexCoords->push_back( osg::Vec2( 0.5, 0.5) );	// just a space-holder here

   	// build the base
   	float radius_x = 1.0;
   	float radius_y = 1.0;
   	float ang;
	
	// setup the angles
	float r = getSweepRotation() * (float)(M_PI / 180.0); // convert to radians
	float a = getSweepAngle();
	if (a > +360.0f) {
		a = +360.0f;
	}
	if (a < -360.0f) {
		a = -360.0f;
	}
	a = a * (float)(M_PI / 180.0); // convert to radians
	if (a < 0.0f) {
		r = r + a;
		a = -a;
	}
	
	const float astep = a / (float) divisions;

	// build the geodes
	osg::Geometry* coneGeometry = new osg::Geometry();
	osg::Geometry* baseGeometry = new osg::Geometry();
	coneNode->addDrawable( coneGeometry );
	coneGeometry->setVertexArray( points );
	coneGeometry->setTexCoordArray( 0, texCoords );
	coneGeometry->addPrimitiveSet( indices );
	baseNode->addDrawable( baseGeometry );
	baseGeometry->setVertexArray( points );
	baseGeometry->setTexCoordArray( 0, baseTexCoords );
	baseGeometry->addPrimitiveSet( baseIndices );

   	// build a full cone if the sweep angle is >= 360.0 degrees
   	if( sweepAngle >= 360.0f ) {
	   	for( int i = 0; i < divisions; i++ ) {
	   		//ang = 2.0 * osg::PI * ((float)i / (float)divisions);
			ang = r + (astep * (float)i);

	   		// add the vertex
	   		points->push_back( osg::Vec3( cos(ang) * radius_x, sin(ang) * radius_y, zbottom ) );

	   		// add the index of that vertex to the conical geometry
	   		indices->push_back( i+1 );

	   		// add the texture coordinate of that vertex to the concial geometry
	   		texCoords->push_back( osg::Vec2( 0.5 + 0.5 * cos(ang), 0.5 + 0.5 * sin(ang) ) );

	   		// add the texture coordinate of that vertex to the base geometry
	   		baseTexCoords->push_back( osg::Vec2( 0.5 + 0.5 * cos(ang), 0.5 + 0.5 * sin(ang) ) );
	   	}

	   	// add the final face to connect the last index to the first
	   	indices->push_back( 1 );

	   	// build the base indices
	   	baseIndices->push_back(1);
	   	baseIndices->push_back(2);
	   	for( int i = 3; i <= (divisions >> 1) + 1; i++ ) {
	   		baseIndices->push_back( divisions - i + 3 );
	   		baseIndices->push_back( i );
	   	}
	   	// if we have an odd # of divisions, add the final face
	   	if( divisions % 2 == 1 ) {
	   		baseIndices->push_back( (divisions>>1) + 2 );
	   	}
   	}
   	else {			// build a section of a cone
   		// add the center of the cone
   		points->push_back( osg::Vec3( 0, 0, zbottom ) );

   		// it's texture coordinate
   		baseTexCoords->push_back( osg::Vec2( 0.5, 0.5 ) );

   		// place-holder texture coordinate
   		texCoords->push_back( osg::Vec2( 0.5, 0.5 ) );

   		for( int i = 0; i <= divisions; i++ ) {
	   		//ang = a * ((float)i / (float)divisions);
			ang = r + (astep * (float)i);
			
	   		// add the vertex
	   		points->push_back( osg::Vec3( cos(ang) * radius_x, sin(ang) * radius_y, zbottom ) );

	   		// add the index of that vertex to the conical geometry
	   		indices->push_back( i+2 );

	   		// add the texture coordinate of that vertex to the concial geometry
	   		texCoords->push_back( osg::Vec2( 0.5 + 0.5 * cos(ang), 0.5 + 0.5 * sin(ang) ) );

	   		// add the texture coordinate of that vertex to the base geometry
	   		baseTexCoords->push_back( osg::Vec2( 0.5 + 0.5 * cos(ang), 0.5 + 0.5 * sin(ang) ) );
	   	}


	   	// build the base indices
	   	// switch to a triangle fan
	   	baseIndices->setMode( osg::PrimitiveSet::TRIANGLE_FAN );

	   	// make the indices
	   	// the first point should index the center of the cone
	   	baseIndices->push_back( 1 );	// the 1st point is the center
	   	for( int i = 2; i <= divisions + 2; i++ ) {
	   		baseIndices->push_back( i );
	   	}

	   	// make the cross-sections
	   	osg::DrawElementsUInt* startIndices = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, 0 );
		osg::DrawElementsUInt* endIndices = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, 0 );


	   	// add the indices to (1) the last point in the base, (2) the center, (3) the tip, and (4) the first point in the base
	   	endIndices->push_back( divisions + 2 );	// the (divisions+2)-th point is the last point in the base
	   	endIndices->push_back( 1 );	// the 1st point is the center
	   	endIndices->push_back( 0 );			// the 0th point is the tip

		startIndices->push_back( 2 );			// the 2st point is the first point in the base
		startIndices->push_back( 0 );			// the 0th point is the tip
		startIndices->push_back( 1 );	// the 1st point is the center
	   	
	   	// make the cross-section texture coordinates
	   	osg::Vec2Array* crossSectionTexCoords = new osg::Vec2Array();

	   	// make sure we correspond to the indices
	   	crossSectionTexCoords->push_back( osg::Vec2( 0.0, 0.0 ) );
	   	crossSectionTexCoords->push_back( osg::Vec2( 0.0, 0.5 ) );
	   	crossSectionTexCoords->push_back( osg::Vec2( 1.0, 0.5 ) );
	   	// insert place-holders
	   	for( int i = 0; i <= divisions + 3; i++ )
	   		crossSectionTexCoords->push_back( osg::Vec2( 1.0, 0.0 ) );

	   	crossSectionTexCoords->push_back( osg::Vec2( 0.0, 0.0 ) );

	   	// make the crossSection geode
	   	osg::Geometry* startGeometry = new osg::Geometry();
		osg::Geometry* endGeometry = new osg::Geometry();
		startNode->addDrawable( startGeometry );
		startGeometry->setVertexArray( points );
		startGeometry->setTexCoordArray( 0, crossSectionTexCoords );
		startGeometry->addPrimitiveSet( startIndices );
		endNode->addDrawable( endGeometry );
		endGeometry->setVertexArray( points );
		endGeometry->setTexCoordArray( 0, crossSectionTexCoords );
		endGeometry->addPrimitiveSet( endIndices );
   	}
}
