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

#include "render/Selection.h"

#include "windows/View.h"

const float Selection::CENTER_RADIUS = 0.5f;
const float Selection::SHAFT_LENGTH = 10.0f;
const float Selection::SHAFT_RADIUS = 0.4f;
const float Selection::TIP_LENGTH = 3.0f;
const float Selection::TIP_RADIUS = 1.0f;


Selection::Selection( View* view, SelectionState state ) {
	this->view = view;

	state = state;
	
	objectAxisGroup = new Renderable();
	
	// build the axis geode and add it
	axes = buildAxes( osg::Vec3( 0.0, 0.0, 0.0 ) );
	
	// build the rotator geode
	rotator = buildRotator( osg::Vec3( 0.0, 0.0, 0.0 ) );
	
	// build the scale geode
	scaler = buildScaler( osg::Vec3( 0.0, 0.0, 0.0 ) );
	
	// build a copy of the local object axes
	objectAxes = buildLocalAxes( osg::Vec3( 0.0, 0.0, 0.0 ) );
	
	addChild( objectAxisGroup.get() );
	
	// assign the selector geode based on state
	switch( state ) {
		case TRANSLATE:
			selectionNode = axes;
			break;
		case ROTATE:
			selectionNode = rotator;
			break;
		case SCALE:
			selectionNode = scaler;
			break;
			
		default:
			selectionNode = axes;
			break;
	}
	
	setName( Selection_NODE_NAME );
	
}

// build the axes geode
osg::ref_ptr< Renderable > Selection::buildAxes( osg::Vec3 localOrigin ) {
	// cone endings of the axes
	osg::ref_ptr< osg::Cone > x_tip, y_tip, z_tip;
	
	// the cylinders connecting the tips
	osg::ref_ptr< osg::Cylinder > x_shaft, y_shaft, z_shaft;
	
	// the center of the axes
	osg::ref_ptr< osg::Sphere > center = new osg::Sphere( localOrigin, Selection::CENTER_RADIUS );
	
	// create the shafts and translate them to their appropriate positions
	x_shaft = new osg::Cylinder( localOrigin + osg::Vec3( Selection::SHAFT_LENGTH / 2.0, 0, 0 ), Selection::SHAFT_RADIUS, Selection::SHAFT_LENGTH );
	x_shaft->setRotation( osg::Quat( osg::DegreesToRadians( 90.0 ), osg::Vec3( 0.0, 1.0, 0.0 ) ) );
	
	y_shaft = new osg::Cylinder( localOrigin + osg::Vec3( 0, Selection::SHAFT_LENGTH / 2.0, 0 ), Selection::SHAFT_RADIUS, Selection::SHAFT_LENGTH );
	y_shaft->setRotation( osg::Quat( osg::DegreesToRadians( 90.0 ), osg::Vec3( 1.0, 0.0, 0.0 ) ) );
	
	z_shaft = new osg::Cylinder( localOrigin + osg::Vec3( 0, 0, Selection::SHAFT_LENGTH / 2.0 ), Selection::SHAFT_RADIUS, Selection::SHAFT_LENGTH );
	
	// create the tips
	x_tip = new osg::Cone( localOrigin + osg::Vec3( Selection::SHAFT_LENGTH + Selection::TIP_LENGTH/4, 0, 0 ), Selection::TIP_RADIUS, Selection::TIP_LENGTH );
	x_tip->setRotation( osg::Quat( osg::DegreesToRadians( 90.0 ), osg::Vec3( 0.0, 1.0, 0.0 ) ) );
	
	y_tip = new osg::Cone( localOrigin + osg::Vec3( 0, Selection::SHAFT_LENGTH + Selection::TIP_LENGTH/4, 0 ), Selection::TIP_RADIUS, Selection::TIP_LENGTH );
	y_tip->setRotation( osg::Quat( osg::DegreesToRadians( -90.0 ), osg::Vec3( 1.0, 0.0, 0.0 ) ) );
	
	z_tip = new osg::Cone( localOrigin + osg::Vec3( 0, 0, Selection::SHAFT_LENGTH + Selection::TIP_LENGTH/4 ), Selection::TIP_RADIUS, Selection::TIP_LENGTH );
	// z_tip->setRotation( osg::Quat( osg::DegreesToRadians( 90.0 ), osg::Vec3( 1.0, 0.0, 0.0 ) ) );
	
	// make drawables
	osg::ShapeDrawable  *centerD	= new osg::ShapeDrawable( center.get() ),
						*x_shaftD	= new osg::ShapeDrawable( x_shaft.get() ),
						*y_shaftD	= new osg::ShapeDrawable( y_shaft.get() ),
						*z_shaftD	= new osg::ShapeDrawable( z_shaft.get() ),
						*x_tipD		= new osg::ShapeDrawable( x_tip.get() ),
						*y_tipD		= new osg::ShapeDrawable( y_tip.get() ),
						*z_tipD		= new osg::ShapeDrawable( z_tip.get() );
	
	// make geodes to contain the objects
	// We do this so we can tell in the selection handler which part of the object was selected
	// (i.e. which axis)
	osg::Geode *centerGeode = new osg::Geode(),
			   *xGeode		= new osg::Geode(),
			   *yGeode		= new osg::Geode(),
			   *zGeode		= new osg::Geode();
	
	// add the drawables
	centerGeode->addDrawable( centerD );
	xGeode->addDrawable( x_shaftD );
	xGeode->addDrawable( x_tipD );
	yGeode->addDrawable( y_shaftD );
	yGeode->addDrawable( y_tipD );
	zGeode->addDrawable( z_shaftD );
	zGeode->addDrawable( z_tipD );
	
	// colorize the geodes
	// x axis is green
	SceneBuilder::assignMaterial( osg::Vec4f( 0.0, 1.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 1.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.0 ),
								  0.0f,
								  0.5f,
								  xGeode );
								  
	// y axis is red
	SceneBuilder::assignMaterial( osg::Vec4f( 1.0, 0.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.5 ),
								  osg::Vec4f( 1.0, 0.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.0 ),
								  0.0f,
								  0.5f,
								  yGeode );
	
	// z axis is blue			  
	SceneBuilder::assignMaterial( osg::Vec4f( 0.0, 0.0, 1.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 1.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.0 ),
								  0.0f,
								  0.5f,
								  zGeode );
	
	// center is white
	SceneBuilder::assignMaterial( osg::Vec4f( 1.0, 1.0, 1.0, 1.0 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 1.0 ),
								  osg::Vec4f( 1.0, 1.0, 1.0, 1.0 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 1.0 ),
								  0.0f,
								  1.0f,
								  centerGeode );
	
	// give the geodes names
	centerGeode->setName( Selection_CENTER_NODE_NAME );
	xGeode->setName( Selection_X_AXIS_NODE_NAME );
	yGeode->setName( Selection_Y_AXIS_NODE_NAME );
	zGeode->setName( Selection_Z_AXIS_NODE_NAME );
	
	// add the geodes to a group
	osg::Group* axesGroup = new osg::Group();
	axesGroup->addChild( xGeode );
	axesGroup->addChild( yGeode );
	axesGroup->addChild( zGeode );
	axesGroup->addChild( centerGeode );
	
	// encase the group into a renderable
	osg::ref_ptr< Renderable > r = new Renderable( axesGroup );
	osg::StateSet* states = axesGroup->getOrCreateStateSet();
	
	// disable depth testing
	states->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );
	states->setMode(GL_CULL_FACE, osg::StateAttribute::ON );
	states->setMode(GL_BLEND, osg::StateAttribute::ON );
	
	// render after Opaque bin:0 and transparent bin:10
	states->setRenderBinDetails( 11, "RenderBin");
	
	axesGroup->setStateSet( states );
	
	return r;
}

// build the axes geode
osg::ref_ptr< Renderable > Selection::buildLocalAxes( osg::Vec3 localOrigin ) {
	
	// the cylinders connecting the tips
	osg::ref_ptr< osg::Cylinder > x_shaft, y_shaft, z_shaft;
	
	// create the shafts and translate them to their appropriate positions
	x_shaft = new osg::Cylinder( localOrigin + osg::Vec3( Selection::SHAFT_LENGTH / 2.0, 0, 0 ), Selection::SHAFT_RADIUS / 10, Selection::SHAFT_LENGTH );
	x_shaft->setRotation( osg::Quat( osg::DegreesToRadians( 90.0 ), osg::Vec3( 0.0, 1.0, 0.0 ) ) );
	
	y_shaft = new osg::Cylinder( localOrigin + osg::Vec3( 0, Selection::SHAFT_LENGTH / 2.0, 0 ), Selection::SHAFT_RADIUS / 10, Selection::SHAFT_LENGTH );
	y_shaft->setRotation( osg::Quat( osg::DegreesToRadians( 90.0 ), osg::Vec3( 1.0, 0.0, 0.0 ) ) );
	
	z_shaft = new osg::Cylinder( localOrigin + osg::Vec3( 0, 0, Selection::SHAFT_LENGTH / 2.0 ), Selection::SHAFT_RADIUS / 10, Selection::SHAFT_LENGTH );
	
	// make drawables
	osg::ShapeDrawable	*x_shaftD	= new osg::ShapeDrawable( x_shaft.get() ),
						*y_shaftD	= new osg::ShapeDrawable( y_shaft.get() ),
						*z_shaftD	= new osg::ShapeDrawable( z_shaft.get() );
	
	// make geodes to contain the objects
	// We do this so we can tell in the selection handler which part of the object was selected
	// (i.e. which axis)
	osg::Geode *xGeode		= new osg::Geode(),
			   *yGeode		= new osg::Geode(),
			   *zGeode		= new osg::Geode();
	
	// add the drawables
	xGeode->addDrawable( x_shaftD );
	yGeode->addDrawable( y_shaftD );
	zGeode->addDrawable( z_shaftD );
	
	// colorize the geodes
	// x axis is green
	SceneBuilder::assignMaterial( osg::Vec4f( 0.0, 1.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 1.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.0 ),
								  0.0f,
								  0.5f,
								  xGeode );
								  
	// y axis is red
	SceneBuilder::assignMaterial( osg::Vec4f( 1.0, 0.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.5 ),
								  osg::Vec4f( 1.0, 0.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.0 ),
								  0.0f,
								  0.5f,
								  yGeode );
	
	// z axis is blue			  
	SceneBuilder::assignMaterial( osg::Vec4f( 0.0, 0.0, 1.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 1.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.0 ),
								  0.0f,
								  0.5f,
								  zGeode );
	
	xGeode->setName( string(Selection_X_AXIS_NODE_NAME) + "|local" );
	yGeode->setName( string(Selection_Y_AXIS_NODE_NAME) + "|local" );
	zGeode->setName( string(Selection_Z_AXIS_NODE_NAME) + "|local" );
	
	// add the geodes to a group
	osg::Group* axesGroup = new osg::Group();
	axesGroup->addChild( xGeode );
	axesGroup->addChild( yGeode );
	axesGroup->addChild( zGeode );
	
	// encase the group into a renderable
	osg::ref_ptr< Renderable > r = new Renderable( axesGroup );
	osg::StateSet* states = axesGroup->getOrCreateStateSet();
	
	// disable depth testing
	states->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );
	states->setMode(GL_CULL_FACE, osg::StateAttribute::ON );
	states->setMode(GL_BLEND, osg::StateAttribute::ON );
	
	// render after Opaque bin:0 and transparent bin:10
	states->setRenderBinDetails( 11, "RenderBin");
	
	axesGroup->setStateSet( states );
	
	
	
	return r;
}

// build the scaler geode
osg::ref_ptr< Renderable > Selection::buildScaler( osg::Vec3 localOrigin ) {
	
	// box endings of the axes
	osg::ref_ptr< osg::Box > x_tip, y_tip, z_tip;
	
	// the cylinders connecting the tips
	osg::ref_ptr< osg::Cylinder > x_shaft, y_shaft, z_shaft;
	
	// the center of the axes
	osg::ref_ptr< osg::Sphere > center = new osg::Sphere( localOrigin, Selection::CENTER_RADIUS );
	
	// create the shafts and translate them to their appropriate positions
	x_shaft = new osg::Cylinder( localOrigin + osg::Vec3( Selection::SHAFT_LENGTH / 2.0, 0, 0 ), Selection::SHAFT_RADIUS, Selection::SHAFT_LENGTH );
	x_shaft->setRotation( osg::Quat( osg::DegreesToRadians( 90.0 ), osg::Vec3( 0.0, 1.0, 0.0 ) ) );
	
	y_shaft = new osg::Cylinder( localOrigin + osg::Vec3( 0, Selection::SHAFT_LENGTH / 2.0, 0 ), Selection::SHAFT_RADIUS, Selection::SHAFT_LENGTH );
	y_shaft->setRotation( osg::Quat( osg::DegreesToRadians( 90.0 ), osg::Vec3( 1.0, 0.0, 0.0 ) ) );
	
	z_shaft = new osg::Cylinder( localOrigin + osg::Vec3( 0, 0, Selection::SHAFT_LENGTH / 2.0 ), Selection::SHAFT_RADIUS, Selection::SHAFT_LENGTH );
	
	// create the tips
	x_tip = new osg::Box( localOrigin + osg::Vec3( Selection::SHAFT_LENGTH + Selection::TIP_LENGTH/4, 0, 0 ), Selection::TIP_LENGTH/2, Selection::TIP_LENGTH/2, Selection::TIP_LENGTH/2 );
	
	y_tip = new osg::Box( localOrigin + osg::Vec3( 0, Selection::SHAFT_LENGTH + Selection::TIP_LENGTH/4, 0 ), Selection::TIP_LENGTH/2, Selection::TIP_LENGTH/2, Selection::TIP_LENGTH/2 );
	
	z_tip = new osg::Box( localOrigin + osg::Vec3( 0, 0, Selection::SHAFT_LENGTH + Selection::TIP_LENGTH/4 ), Selection::TIP_LENGTH/2, Selection::TIP_LENGTH/2, Selection::TIP_LENGTH/2 );
	
	// make drawables
	osg::ShapeDrawable  *centerD	= new osg::ShapeDrawable( center.get() ),
						*x_shaftD	= new osg::ShapeDrawable( x_shaft.get() ),
						*y_shaftD	= new osg::ShapeDrawable( y_shaft.get() ),
						*z_shaftD	= new osg::ShapeDrawable( z_shaft.get() ),
						*x_tipD		= new osg::ShapeDrawable( x_tip.get() ),
						*y_tipD		= new osg::ShapeDrawable( y_tip.get() ),
						*z_tipD		= new osg::ShapeDrawable( z_tip.get() );
	
	// make geodes to contain the objects
	// We do this so we can tell in the selection handler which part of the object was selected
	// (i.e. which axis)
	osg::Geode *centerGeode = new osg::Geode(),
			   *xGeode		= new osg::Geode(),
			   *yGeode		= new osg::Geode(),
			   *zGeode		= new osg::Geode();
	
	// add the drawables
	centerGeode->addDrawable( centerD );
	xGeode->addDrawable( x_shaftD );
	xGeode->addDrawable( x_tipD );
	yGeode->addDrawable( y_shaftD );
	yGeode->addDrawable( y_tipD );
	zGeode->addDrawable( z_shaftD );
	zGeode->addDrawable( z_tipD );
	
	// colorize the geodes
	// x axis is green
	SceneBuilder::assignMaterial( osg::Vec4f( 0.0, 1.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 1.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.0 ),
								  0.0f,
								  0.5f,
								  xGeode );
								  
	// y axis is red
	SceneBuilder::assignMaterial( osg::Vec4f( 1.0, 0.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.5 ),
								  osg::Vec4f( 1.0, 0.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.0 ),
								  0.0f,
								  0.5f,
								  yGeode );
	
	// z axis is blue			  
	SceneBuilder::assignMaterial( osg::Vec4f( 0.0, 0.0, 1.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 1.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.0 ),
								  0.0f,
								  0.5f,
								  zGeode );
	
	// center is white
	SceneBuilder::assignMaterial( osg::Vec4f( 1.0, 1.0, 1.0, 1.0 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 1.0 ),
								  osg::Vec4f( 1.0, 1.0, 1.0, 1.0 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.0 ),
								  0.0f,
								  1.0f,
								  centerGeode );
	
	// give the geodes names
	centerGeode->setName( Selection_CENTER_NODE_NAME );
	xGeode->setName( Selection_X_AXIS_NODE_NAME );
	yGeode->setName( Selection_Y_AXIS_NODE_NAME );
	zGeode->setName( Selection_Z_AXIS_NODE_NAME );
	
	// add the geodes to a group
	osg::Group* axesGroup = new osg::Group();
	axesGroup->addChild( xGeode );
	axesGroup->addChild( yGeode );
	axesGroup->addChild( zGeode );
	axesGroup->addChild( centerGeode );
	
	// encase the group into a renderable
	osg::ref_ptr< Renderable > r = new Renderable( axesGroup );
	osg::StateSet* states = axesGroup->getOrCreateStateSet();
	
	// disable depth testing
	states->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );
	states->setMode(GL_CULL_FACE, osg::StateAttribute::ON );
	states->setMode(GL_BLEND, osg::StateAttribute::ON );
	
	// render after Opaque bin:0 and transparent bin:10
	states->setRenderBinDetails( 11, "RenderBin");
	
	axesGroup->setStateSet( states );
	
	return r;
}

// build the rotator geode
osg::ref_ptr< Renderable > Selection::buildRotator( osg::Vec3 localOrigin ) {
	// cone endings of the axes
	osg::ref_ptr< osg::Cone > /*x_tip, y_tip,*/ z_tip;
	
	// the cylinders connecting the tips
	osg::ref_ptr< osg::Cylinder > /*x_shaft, y_shaft,*/ z_shaft;
	
	// the center of the axes
	osg::ref_ptr< osg::Sphere > center = new osg::Sphere( localOrigin, Selection::CENTER_RADIUS );
	
	// spin disks
	osg::ref_ptr< osg::Cylinder > /*x_spin, y_spin,*/ z_spin;
	
	// spin cones
	osg::ref_ptr< osg::Cone > /*x_spin_up, x_spin_down, y_spin_up, y_spin_down,*/ z_spin_up, z_spin_down;
	
	// create the shafts and translate them to their appropriate positions
	/*x_shaft = new osg::Cylinder( localOrigin + osg::Vec3( Selection::SHAFT_LENGTH / 2.0, 0, 0 ), Selection::SHAFT_RADIUS, Selection::SHAFT_LENGTH );
	x_shaft->setRotation( osg::Quat( osg::DegreesToRadians( 90.0 ), osg::Vec3( 0.0, 1.0, 0.0 ) ) );
	
	y_shaft = new osg::Cylinder( localOrigin + osg::Vec3( 0, Selection::SHAFT_LENGTH / 2.0, 0 ), Selection::SHAFT_RADIUS, Selection::SHAFT_LENGTH );
	y_shaft->setRotation( osg::Quat( osg::DegreesToRadians( 90.0 ), osg::Vec3( 1.0, 0.0, 0.0 ) ) );*/
	
	z_shaft = new osg::Cylinder( localOrigin + osg::Vec3( 0, 0, Selection::SHAFT_LENGTH / 2.0 ), Selection::SHAFT_RADIUS, Selection::SHAFT_LENGTH );
	
	// create the tips
	/*x_tip = new osg::Cone( localOrigin + osg::Vec3( Selection::SHAFT_LENGTH, 0, 0 ), Selection::TIP_RADIUS, Selection::TIP_LENGTH );
	x_tip->setRotation( osg::Quat( osg::DegreesToRadians( 90.0 ), osg::Vec3( 0.0, 1.0, 0.0 ) ) );
	
	y_tip = new osg::Cone( localOrigin + osg::Vec3( 0, Selection::SHAFT_LENGTH, 0 ), Selection::TIP_RADIUS, Selection::TIP_LENGTH );
	y_tip->setRotation( osg::Quat( osg::DegreesToRadians( -90.0 ), osg::Vec3( 1.0, 0.0, 0.0 ) ) );*/
	
	z_tip = new osg::Cone( localOrigin + osg::Vec3( 0, 0, Selection::SHAFT_LENGTH + Selection::TIP_LENGTH/4 ), Selection::TIP_RADIUS, Selection::TIP_LENGTH );
	
	// create the disks
	/*x_spin = new osg::Cylinder( localOrigin + osg::Vec3( 3 * Selection::SHAFT_LENGTH / 4.0, 0, 0), Selection::TIP_RADIUS * 2.0, Selection::TIP_LENGTH / 8.0 );
	x_spin->setRotation( osg::Quat( osg::DegreesToRadians( 90.0 ), osg::Vec3( 0.0, 1.0, 0.0 ) ) );
	
	y_spin = new osg::Cylinder( localOrigin + osg::Vec3( 0, 3 * Selection::SHAFT_LENGTH / 4.0, 0), Selection::TIP_RADIUS * 2.0, Selection::TIP_LENGTH / 8.0 );
	y_spin->setRotation( osg::Quat( osg::DegreesToRadians( -90.0 ), osg::Vec3( 1.0, 0.0, 0.0 ) ) );*/
	
	z_spin = new osg::Cylinder( localOrigin + osg::Vec3( 0, 0, 3 * Selection::SHAFT_LENGTH / 4.0), Selection::TIP_RADIUS * 2.0, Selection::TIP_LENGTH / 8.0 );
	
	// create the directional arrows (cones) on the disks
	/*x_spin_up = new osg::Cone( localOrigin + osg::Vec3( 3 * Selection::SHAFT_LENGTH / 4.0, Selection::TIP_RADIUS * 2.95, 0), Selection::TIP_RADIUS / 2.0, Selection::TIP_LENGTH / 2.0 );
	x_spin_down = new osg::Cone( localOrigin + osg::Vec3( 3 * Selection::SHAFT_LENGTH / 4.0, -Selection::TIP_RADIUS * 2.95, 0), Selection::TIP_RADIUS / 2.0, Selection::TIP_LENGTH / 2.0);
	x_spin_down->setRotation( osg::Quat( osg::DegreesToRadians( 180.0 ), osg::Vec3( 1.0, 0.0, 0.0 ) ) );
	
	y_spin_up = new osg::Cone( localOrigin + osg::Vec3( Selection::TIP_RADIUS * 2.95, 3 * Selection::SHAFT_LENGTH / 4.0, 0 ), Selection::TIP_RADIUS / 2.0, Selection::TIP_LENGTH / 2.0 );
	y_spin_down = new osg::Cone( localOrigin + osg::Vec3( -Selection::TIP_RADIUS * 2.95, 3 * Selection::SHAFT_LENGTH / 4.0, 0), Selection::TIP_RADIUS / 2.0, Selection::TIP_LENGTH / 2.0 );
	y_spin_down->setRotation( osg::Quat( osg::DegreesToRadians( 180.0 ), osg::Vec3( 0.0, 1.0, 0.0) ) );*/
	
	z_spin_up = new osg::Cone( localOrigin + osg::Vec3( 0, Selection::TIP_RADIUS * 2.95, 3 * Selection::SHAFT_LENGTH / 4.0 ), Selection::TIP_RADIUS / 2.0, Selection::TIP_LENGTH / 2.0 );
	z_spin_up->setRotation( osg::Quat( osg::DegreesToRadians( 90.0 ), osg::Vec3( 0.0, 1.0, 0.0 ) ) );
	z_spin_down = new osg::Cone( localOrigin + osg::Vec3( 0, -Selection::TIP_RADIUS * 2.95, 3 * Selection::SHAFT_LENGTH / 4.0), Selection::TIP_RADIUS / 2.0, Selection::TIP_LENGTH / 2.0 );
	z_spin_down->setRotation( osg::Quat( osg::DegreesToRadians( -90.0 ), osg::Vec3( 0.0, 1.0, 0.0 ) ) );
	
	// make drawables
	osg::ShapeDrawable  *centerD	= new osg::ShapeDrawable( center.get() ),
						/**x_shaftD	= new osg::ShapeDrawable( x_shaft.get() ),
						*y_shaftD	= new osg::ShapeDrawable( y_shaft.get() ),*/
						*z_shaftD	= new osg::ShapeDrawable( z_shaft.get() ),
						/**x_tipD		= new osg::ShapeDrawable( x_tip.get() ),
						*y_tipD		= new osg::ShapeDrawable( y_tip.get() ),*/
						*z_tipD		= new osg::ShapeDrawable( z_tip.get() ),
						/**x_spinD	= new osg::ShapeDrawable( x_spin.get() ),
						*y_spinD	= new osg::ShapeDrawable( y_spin.get() ),*/
						*z_spinD	= new osg::ShapeDrawable( z_spin.get() ),
						/**x_spin_upD = new osg::ShapeDrawable( x_spin_up.get() ),
						*x_spin_downD = new osg::ShapeDrawable( x_spin_down.get() ),
						*y_spin_upD = new osg::ShapeDrawable( y_spin_up.get() ),
						*y_spin_downD = new osg::ShapeDrawable( y_spin_down.get() ),*/
						*z_spin_upD = new osg::ShapeDrawable( z_spin_up.get() ),
						*z_spin_downD = new osg::ShapeDrawable( z_spin_down.get() );
	
	// make geodes to contain the objects
	// We do this so we can tell in the selection handler which part of the object was selected
	// (i.e. which axis)
	osg::Geode *centerGeode = new osg::Geode(),
			   /**xGeode		= new osg::Geode(),
			   *yGeode		= new osg::Geode(),*/
			   *zGeode		= new osg::Geode();
	
	// add the drawables
	centerGeode->addDrawable( centerD );
	/*xGeode->addDrawable( x_shaftD );
	xGeode->addDrawable( x_tipD );
	xGeode->addDrawable( x_spinD );
	xGeode->addDrawable( x_spin_upD );
	xGeode->addDrawable( x_spin_downD );
	yGeode->addDrawable( y_shaftD );
	yGeode->addDrawable( y_tipD );
	yGeode->addDrawable( y_spinD );
	yGeode->addDrawable( y_spin_upD );
	yGeode->addDrawable( y_spin_downD);*/
	zGeode->addDrawable( z_shaftD );
	zGeode->addDrawable( z_tipD );
	zGeode->addDrawable( z_spinD );
	zGeode->addDrawable( z_spin_upD );
	zGeode->addDrawable( z_spin_downD );
	
	// colorize the geodes
	// x axis is green
	/*SceneBuilder::assignMaterial( osg::Vec4f( 0.0, 1.0, 0.0, 1.0 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 1.0 ),
								  osg::Vec4f( 0.0, 1.0, 0.0, 1.0 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.0 ),
								  0.0f,
								  1.0f,
								  xGeode );
								  
	// y axis is red
	SceneBuilder::assignMaterial( osg::Vec4f( 1.0, 0.0, 0.0, 1.0 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 1.0 ),
								  osg::Vec4f( 1.0, 0.0, 0.0, 1.0 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.0 ),
								  0.0f,
								  1.0f,
								  yGeode );*/
	
	// z axis is blue			  
	SceneBuilder::assignMaterial( osg::Vec4f( 0.0, 0.0, 1.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 1.0, 0.5 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.0 ),
								  0.0f,
								  0.5f,
								  zGeode );
	
	// center is white
	SceneBuilder::assignMaterial( osg::Vec4f( 1.0, 1.0, 1.0, 1.0 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 1.0 ),
								  osg::Vec4f( 1.0, 1.0, 1.0, 1.0 ),
								  osg::Vec4f( 0.0, 0.0, 0.0, 0.0 ),
								  0.0f,
								  1.0f,
								  centerGeode );
	
	// give the geodes names
	centerGeode->setName( Selection_CENTER_NODE_NAME );
	/*xGeode->setName( Selection_X_AXIS_NODE_NAME );
	yGeode->setName( Selection_Y_AXIS_NODE_NAME );*/
	zGeode->setName( Selection_Z_AXIS_NODE_NAME );
	
	// add the geodes to a group
	osg::Group* axesGroup = new osg::Group();
	/*axesGroup->addChild( xGeode );
	axesGroup->addChild( yGeode );*/
	axesGroup->addChild( zGeode );
	axesGroup->addChild( centerGeode );
	
	// encase the group into a renderable
	osg::ref_ptr< Renderable > r = new Renderable( axesGroup );
	osg::StateSet* states = axesGroup->getOrCreateStateSet();
	
	// disable depth testing
	states->setMode(GL_DEPTH_TEST, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );
	states->setMode(GL_CULL_FACE, osg::StateAttribute::ON );
	states->setMode(GL_BLEND, osg::StateAttribute::ON );
	
	// render after Opaque bin:0 and transparent bin:10
	states->setRenderBinDetails( 11, "RenderBin");
	
	axesGroup->setStateSet( states );
	
	return r;
}

// get the local origin
osg::Vec3 Selection::computeLocalOrigin( Model::objRefList& objects ) {
	double x = 0.0, y = 0.0, z = 0.0;
	
	// compute the average x, y, and z values of all renderables
	if( objects.size() > 0 ) {
		for( Model::objRefList::iterator i = objects.begin(); i != objects.end(); i++) {
			x += (*i)->getPos().x();
			y += (*i)->getPos().y();
			z += (*i)->getPos().z();
		}
		
		int numElements = objects.size();
		x /= numElements;
		y /= numElements;
		z /= numElements;
	}
	
	return osg::Vec3( x, y, z );
}

// rebuild the axes
void Selection::rebuildAxes( Model::objRefList& objects ) {
	selectionNode->setPosition( computeLocalOrigin( objects ) );
	
	objectAxisGroup->setPosition( osg::Vec3( 0, 0, 0 ) );
	
	// remove all local axes (destructors are called automatically)
	objectAxisGroup->removeChildren( 0, objectAxisGroup->getNumChildren() );
	
	// recompute them
	for( Model::objRefList::iterator i = objects.begin(); i != objects.end(); i++ ) {
		Renderable* r = new Renderable( objectAxes.get() );
		
		r->setPosition( (*i)->getPos() );
		r->setAttitude( (*i)->getRot() );
		
		objectAxisGroup->addChild( r );
	}
}

// determine which child of this node was picked
// (used by selectHandler)
osg::Node* Selection::getPickedNode( Renderable* r, const osg::NodePath& nodes, unsigned int startIndex ) {
	if(nodes.size() <= 0)
		return NULL;
		
	for(unsigned int i = startIndex; i < nodes.size(); i++) {
		if( nodes[i]->getName().length() > 0 )
			return nodes[i];
	}
	
	return NULL;
}

// inhereted Observer method: update().
// This method queries the Model (i.e. the passed observer) for the selected objects
// and recomputes the axis location
void Selection::update( Observable* observable, void* data ) {
	// see if this was the Model
	Model* model = dynamic_cast< Model* >( observable );
	if(!model)
		return;
	
	// update the axes
	Model::objRefList selectedObjects = model->_getSelection();
	
	// remove the axes if there are no objects
	if( selectedObjects.size() <= 0 && containsNode( selectionNode.get() )) {
		removeChild( selectionNode.get() );
		removeChild( objectAxisGroup.get() );
		objectAxisGroup->removeChildren( 0, objectAxisGroup->getNumChildren() );
	}
	
	// add the axes if there are some objects
	if( selectedObjects.size() > 0 && !containsNode( selectionNode.get() )) {
		addChild( selectionNode.get() );
		addChild( objectAxisGroup.get() );
	}
		
	// recompute the center
	if( selectedObjects.size() > 0 )
		rebuildAxes( selectedObjects );
	
}

// set the state of the selector
Selection::SelectionState Selection::setState( SelectionState newState ) {
	
	bool addBack = removeChild( selectionNode.get() );
	
	switch( newState ) {
		case ROTATE:
			rotator->setPosition( selectionNode->getPosition() );
			selectionNode = rotator;
			break;
		case SCALE:
			scaler->setPosition( selectionNode->getPosition() );
			selectionNode = scaler;
			break;
		
		default:
			axes->setPosition( selectionNode->getPosition() );
			selectionNode = axes;
			break;
	}
	
	if( addBack )
		addChild( selectionNode.get() );
	
	// make sure the view redraws and resizes the widget
	view->updateSelection();

	SelectionState ret = state;
	state = newState;
	return ret;
}

// set the state of the selector
Selection::SelectionState Selection::setStateByKey( unsigned char key ) {
	
	bool addBack = removeChild( selectionNode.get() );
	SelectionState newState;

	switch( key ) {
		
		case BZ_ROTATE_KEY:
			rotator->setPosition( selectionNode->getPosition() );
			selectionNode = rotator;
			newState = ROTATE;
			break;
		case BZ_SCALE_KEY:
			scaler->setPosition( selectionNode->getPosition() );
			selectionNode = scaler;
			newState = SCALE;
			break;
		case BZ_TRANSLATE_KEY:
			axes->setPosition( selectionNode->getPosition() );
			selectionNode = axes;
			newState = TRANSLATE;
			break;
		default:
			// don't change the state if the key wasn't recognized
			newState = state;
			break;
	}
	
	if( addBack )
		addChild( selectionNode.get() );

	// make sure the view redraws and resizes the widget
	view->updateSelection();
	
	SelectionState ret = state;
	state = newState;
	return ret;
}
