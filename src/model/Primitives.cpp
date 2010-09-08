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

#include "model/Primitives.h"

#include "model/SceneBuilder.h"

#include <osg/Vec3>
#include <osg/Geometry>
#include <osg/StateSet>
#include <osg/Group>
#include <osgDB/ReadFile>

#include <cmath>

using namespace std;

// build a pyramid
osg::Node* Primitives::buildPyramid( osg::Vec3 size, bool flipz ) {
	osg::Group* pyramid = new osg::Group();
	osg::Geode* sides[5];
	osg::Geometry* geometry[5];
	for (int i = 0; i < 5; i++) {
		sides[i] = new osg::Geode();
		geometry[i] = new osg::Geometry();
		geometry[i]->setVertexAttribBinding( 0, osg::Geometry::BIND_PER_VERTEX );
		sides[i]->addDrawable(geometry[i]);
		pyramid->addChild( sides[i] );
	}
	
	float ztop = size.z();
	float zbottom = 0;
	if(flipz){
		ztop = 0;
		zbottom = size.z();
	}

	// generate vertices for triangular sides
	// +x
	osg::Vec3Array* pxVerts = new osg::Vec3Array();
	pxVerts->push_back( osg::Vec3( size.x(), -size.y(), zbottom ) );
	pxVerts->push_back( osg::Vec3( size.x(), size.y(), zbottom ) );
	pxVerts->push_back( osg::Vec3( 0, 0, ztop ) );
	geometry[0]->setVertexArray( pxVerts );

	// -x
	osg::Vec3Array* nxVerts = new osg::Vec3Array();
	nxVerts->push_back( osg::Vec3( -size.x(), size.y(), zbottom ) );
	nxVerts->push_back( osg::Vec3( -size.x(), -size.y(), zbottom ) );
	nxVerts->push_back( osg::Vec3( 0, 0, ztop ) );
	geometry[1]->setVertexArray( nxVerts );

	// +y
	osg::Vec3Array* pyVerts = new osg::Vec3Array();
	pyVerts->push_back( osg::Vec3( size.x(), size.y(), zbottom ) );
	pyVerts->push_back( osg::Vec3( -size.x(), size.y(), zbottom ) );
	pyVerts->push_back( osg::Vec3( 0, 0, ztop ) );
	geometry[2]->setVertexArray( pyVerts );

	// -y
	osg::Vec3Array* nyVerts = new osg::Vec3Array();
	nyVerts->push_back( osg::Vec3( -size.x(), -size.y(), zbottom ) );
	nyVerts->push_back( osg::Vec3( size.x(), -size.y(), zbottom ) );
	nyVerts->push_back( osg::Vec3( 0, 0, ztop ) );
	geometry[3]->setVertexArray( nyVerts );

	// generate verts for base (-z)
	osg::Vec3Array* nzVerts = new osg::Vec3Array();
	nzVerts->push_back( osg::Vec3( -size.x(), -size.y(), zbottom ) );
	nzVerts->push_back( osg::Vec3( size.x(), -size.y(), zbottom ) );
	nzVerts->push_back( osg::Vec3( size.x(), size.y(), zbottom ) );
	nzVerts->push_back( osg::Vec3( -size.x(), size.y(), zbottom ) );
	geometry[4]->setVertexArray( nzVerts );

	// generate texture coordinates
	rebuildPyramidUV( pyramid, size );

	// generate vertex indices for sides
	osg::DrawElementsUInt* side =
			new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLES, 0 );
	side->push_back( 0 );
	side->push_back( 1 );
	side->push_back( 2 );

	// generate vertex indices for base
	osg::DrawElementsUInt* bottom =
		new osg::DrawElementsUInt( osg::PrimitiveSet::QUADS, 0 );
	bottom->push_back( 0 );
	bottom->push_back( 1 );
	bottom->push_back( 2 );
	bottom->push_back( 3 );


	for (int i = 0; i < 4; i++)
		geometry[i]->addPrimitiveSet( side );
	geometry[4]->addPrimitiveSet( bottom );

	// assign the texture
	for (int i = 0; i < 5; i++)
		SceneBuilder::assignTexture( "pyrwall", sides[i], osg::StateAttribute::ON );

	return pyramid;
}

// rebuild UVs for a pyr, should be called whenever the box is scaled
void Primitives::rebuildPyramidUV( osg::Group* pyr, osg::Vec3 size ) {
	osg::Vec2Array* texcoords[5];

	for (int i = 0; i < 5; i++) {
		texcoords[i] = new osg::Vec2Array();
		osg::Geode* geode = (osg::Geode*)pyr->getChild( i );
		osg::Geometry* geometry = (osg::Geometry*)geode->getDrawable( 0 );
		geometry->setTexCoordArray( 0, texcoords[i] );
	}
	
	float xSideUV = size.x()*2*0.125f;
	float ySideUV = size.y()*2*0.125f;
	float zSideUV = size.z()*0.125f;
	float xzSideUV = 0.125f*sqrtf(size.x()*size.x()+size.z()*size.z());
	float yzSideUV = 0.125f*sqrtf(size.y()*size.y()+size.z()*size.z());

	// +x
	texcoords[0]->push_back( osg::Vec2( 0, 0 ) );
	texcoords[0]->push_back( osg::Vec2( ySideUV, 0 ) );
	texcoords[0]->push_back( osg::Vec2( ySideUV*0.5, xzSideUV ) );
	// -x
	texcoords[1]->push_back( osg::Vec2( 0, 0 ) );
	texcoords[1]->push_back( osg::Vec2( ySideUV, 0 ) );
	texcoords[1]->push_back( osg::Vec2( ySideUV*0.5, xzSideUV ) );
	// +y
	texcoords[2]->push_back( osg::Vec2( 0, 0 ) );
	texcoords[2]->push_back( osg::Vec2( xSideUV, 0 ) );
	texcoords[2]->push_back( osg::Vec2( xSideUV*0.5, yzSideUV ) );
	// -y
	texcoords[3]->push_back( osg::Vec2( 0, 0 ) );
	texcoords[3]->push_back( osg::Vec2( xSideUV, 0 ) );
	texcoords[3]->push_back( osg::Vec2( xSideUV*0.5, yzSideUV ) );
	// -z
	texcoords[4]->push_back( osg::Vec2( xSideUV, ySideUV ) );
	texcoords[4]->push_back( osg::Vec2( 0, ySideUV ) );
	texcoords[4]->push_back( osg::Vec2( 0, 0 ) );
	texcoords[4]->push_back( osg::Vec2( xSideUV, 0 ) );
}

void Primitives::rebuildBoxUV(osg::Group* box, osg::Vec3 size)
{
	// generate UVs
	osg::Vec2Array* sideUVs[6];
	for (int i = 0; i < 6; i++)
		sideUVs[i] = new osg::Vec2Array();

	float xSideUV = size.x()*2*0.125f;
	float ySideUV = size.y()*2*0.125f;
	float zSideUV = size.z()*0.125f;
	float xTopUV = xSideUV*2;
	float yTopUV = ySideUV*2;

	// +x -x
	for ( int i = 0; i < 2; i++ ) {
		sideUVs[i]->push_back( osg::Vec2( 0, 0 ) );
		sideUVs[i]->push_back( osg::Vec2( 0, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( ySideUV, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( ySideUV, 0 ) );
	}

	// +y -y
	for ( int i = 2; i < 4; i++ ) {
		sideUVs[i]->push_back( osg::Vec2( 0, 0 ) );
		sideUVs[i]->push_back( osg::Vec2( 0, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( xSideUV, zSideUV ) );
		sideUVs[i]->push_back( osg::Vec2( xSideUV, 0 ) );
	}

	// +z -z
	for ( int i = 4; i < 6; i++ ) {
		sideUVs[i]->push_back( osg::Vec2( 0, 0 ) );
		sideUVs[i]->push_back( osg::Vec2( 0, yTopUV ) );
		sideUVs[i]->push_back( osg::Vec2( xTopUV, yTopUV ) );
		sideUVs[i]->push_back( osg::Vec2( xTopUV, 0 ) );
	}

	for ( int i = 0; i < 6; i++ ) {
		osg::Geode* geode = (osg::Geode*)box->getChild( i );
		osg::Geometry* geom = (osg::Geometry*)geode->getDrawable( 0 );
		geom->setTexCoordArray( 0, sideUVs[i] );
	}
}

osg::Group* Primitives::buildUntexturedBox( osg::Vec3 size ) {
	osg::Group* group = new osg::Group();
	// separate geometry nodes are needed so that each side
	// can have a separate material
	// array is in the order +x -x +y -y +z -z in bzflag coordinates
	osg::Geode* sideNodes[6];
	for ( int i = 0; i < 6; i++ )
		sideNodes[i] = new osg::Geode();

	// assign geometry nodes to group
	for ( int i = 0; i < 6; i++ )
		group->addChild( sideNodes[i] );

	// create geometry and assign it to the nodes
	osg::Geometry* sideGeometry[6];
	for ( int i = 0; i < 6; i++ ) {
		sideGeometry[i] = new osg::Geometry();
		sideNodes[i]->addDrawable(sideGeometry[i]);
	}

	// add vertices for all sides
	osg::Vec3Array* pxVerts = new osg::Vec3Array();
	pxVerts->push_back( osg::Vec3( size.x(), -size.y(), 0 ) );
	pxVerts->push_back( osg::Vec3( size.x(), -size.y(), size.z() ) );
	pxVerts->push_back( osg::Vec3( size.x(), size.y(), size.z() ) );
	pxVerts->push_back( osg::Vec3( size.x(), size.y(), 0 ) );
	sideGeometry[0]->setVertexArray(pxVerts);

	osg::Vec3Array* nxVerts = new osg::Vec3Array();
	nxVerts->push_back( osg::Vec3( -size.x(), size.y(), 0 ) );
	nxVerts->push_back( osg::Vec3( -size.x(), size.y(), size.z() ) );
	nxVerts->push_back( osg::Vec3( -size.x(), -size.y(), size.z() ) );
	nxVerts->push_back( osg::Vec3( -size.x(), -size.y(), 0 ) );
	sideGeometry[1]->setVertexArray(nxVerts);

	osg::Vec3Array* pyVerts = new osg::Vec3Array();
	pyVerts->push_back( osg::Vec3( size.x(), size.y(), 0 ) );
	pyVerts->push_back( osg::Vec3( size.x(), size.y(), size.z() ) );
	pyVerts->push_back( osg::Vec3( -size.x(), size.y(), size.z() ) );
	pyVerts->push_back( osg::Vec3( -size.x(), size.y(), 0 ) );
	sideGeometry[2]->setVertexArray(pyVerts);

	osg::Vec3Array* nyVerts = new osg::Vec3Array();
	nyVerts->push_back( osg::Vec3( -size.x(), -size.y(), 0 ) );
	nyVerts->push_back( osg::Vec3( -size.x(), -size.y(), size.z() ) );
	nyVerts->push_back( osg::Vec3( size.x(), -size.y(), size.z() ) );
	nyVerts->push_back( osg::Vec3( size.x(), -size.y(), 0 ) );
	sideGeometry[3]->setVertexArray(nyVerts);

	osg::Vec3Array* pzVerts = new osg::Vec3Array();
	pzVerts->push_back( osg::Vec3( size.x(), size.y(), size.z() ) );
	pzVerts->push_back( osg::Vec3( size.x(), -size.y(), size.z() ) );
	pzVerts->push_back( osg::Vec3( -size.x(), -size.y(), size.z() ) );
	pzVerts->push_back( osg::Vec3( -size.x(), size.y(), size.z() ) );
	sideGeometry[4]->setVertexArray(pzVerts);

	osg::Vec3Array* nzVerts = new osg::Vec3Array();
	nzVerts->push_back( osg::Vec3( size.x(), size.y(), 0 ) );
	nzVerts->push_back( osg::Vec3( size.x(), -size.y(), 0 ) );
	nzVerts->push_back( osg::Vec3( -size.x(), -size.y(), 0 ) );
	nzVerts->push_back( osg::Vec3( -size.x(), size.y(), 0 ) );
	sideGeometry[5]->setVertexArray(nzVerts);

	// specify the vertex indices, this is the same for all sides
	osg::DrawElementsUInt* side =
		new osg::DrawElementsUInt( osg::PrimitiveSet::QUADS, 0 );
	side->push_back( 0 );
	side->push_back( 1 );
	side->push_back( 2 );
	side->push_back( 3 );

	// add vertex indices for each side
	for ( int i = 0; i < 6; i++ ) {
		sideGeometry[i]->setVertexAttribBinding( 0, osg::Geometry::BIND_PER_VERTEX );
		sideGeometry[i]->addPrimitiveSet( side );
	}

	return group;
}
