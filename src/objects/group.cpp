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

#include "objects/group.h"
#include <FL/Fl_Progress.H>

// constructor
group::group() : 
	bz2object("group", "<name><position><size><rotation><shift><shear><scale><spin><team><tint><drivethrough><shootthrough><passable><phydrv><matref>") {
	this->team = 0;
	this->def = NULL;
	this->tintColor = RGBA(1, 1, 1, 1);
	this->setName("");
	this->setPos( osg::Vec3( 0.0, 0.0, 0.0 ) );
	
	this->container = new Renderable();
	this->bzw1_containers = map< osg::ref_ptr< bz2object >, osg::ref_ptr< Renderable > >();
	this->geoRing = NULL;
	this->setDataVariance( osg::Object::DYNAMIC );
}

// getter
string group::get(void) {
	return this->toString(); 
}

// event handler
int group::update( UpdateMessage& message ) {
	// superclass update (i.e. handle transformation changes)
	int result = bz2object::update( message );
	// NOW handle the messages
	switch( message.type ) {		
		case UpdateMessage::SET_POSITION: 
			setPos( *(message.getAsPosition()) );
			break;
		case UpdateMessage::SET_POSITION_FACTOR: 	// handle a translation
			setPos( this->getPos() + *(message.getAsPositionFactor()) );
			break;
		case UpdateMessage::SET_ROTATION:		// handle a new rotation
			setRotation( *(message.getAsRotation()) );
			break;
		case UpdateMessage::SET_ROTATION_FACTOR:	// handle an angular translation
			setRotation( this->getRotation() + *(message.getAsRotationFactor()) );
			break;
		case UpdateMessage::SET_SCALE:		// handle a new scale
			setSize( *(message.getAsScale()) );
			buildGeometry();			
			break;
		case UpdateMessage::SET_SCALE_FACTOR:	// handle a scaling factor
			setSize( getSize() +  *(message.getAsScale()) );
			buildGeometry();
			break;
		default:	// unknown event; don't handle
			return result;
	}
	return 1;	
}

// bzw methods
bool group::parse( std::string& line ) {
	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );
	
	// check if we reached the end of the section
	if ( key == "end" )
		return false;

	// parse keys
	if ( key == "tint" ) {
		tintColor = RGBA( value.c_str() );
	}
	else if ( key == "team" ) {
		team = atoi( value.c_str() );
	}
	else {
		return bz2object::parse( line );
	}

	return true;
}

void group::finalize() {
	updateObjects();
	bz2object::finalize();
}

// toString
string group::toString(void) {
	// don't bother saving if not associated with a define
	if ( def == NULL )
		return string();

	osg::Vec3 p = this->getPos();
	string tintString = string(""), teamString = string("");
	if(tintColor.r() > 0 && tintColor.g() > 0 && tintColor.b() > 0 && tintColor.a() > 0)
		tintString = "  tint " + tintColor.toString();
	if(team > 0)
		teamString = "  team " + string(itoa(team)) + "\n";
	
	string ret = string("group ") + this->def->getName() + "\n" +
				  tintString + 
				  teamString +
				  (drivethrough == true ? "  drivethrough\n" : "") +
				  (shootthrough == true ? "  shootThrough\n" : "") +
				  this->BZWLines( this ) +
				  "end\n";
	
	// return the string data
	return ret;
}

// build the ring geometry around the objects
void group::buildGeometry() {
	// compute the maximum radius outside the center
	float maxRadius2 = 25.0f;	// radius squared (saves sqrt() calls)
	float maxDim = 0.0f;
	if( this->container->getNumChildren() > 0 ) {
		// get each child
		for( unsigned int i = 0; i < this->container->getNumChildren(); i++ ) {
			osg::Node* child = this->container->getChild( i );
			bz2object* obj = dynamic_cast< bz2object* >(child);
			
			// this cast will only work, literally, for BZW2 objects (BZW1 objects are contained within separate
			// transformation containers of type Renderable.)
			if( obj ) {
				// get position and size
				osg::Vec3 p = obj->getPos();
				osg::Vec3 size = obj->getSize();
				
				// compute the largest dimension
				float maxSizeDim = max( size.x(), max( size.y(), size.z() ) );
				maxDim = max( maxSizeDim, maxDim );
				
				// the group's position relative to the objects is (0,0,0), so just square the position and take the length
				float len2 = p.x()*p.x() + p.y()*p.y() + p.z()*p.z();
				
				// see if it's bigger than the maximum radius
				if( len2 > maxRadius2 ) {
					maxRadius2 = len2;
				}
			}
			// see if this is a BZW1 object
			else {
				Renderable* r = dynamic_cast< Renderable* >(child);
				// if this is a container node, then it should have a child that is a bz2object.
				if( r && r->getName() == BZW1_CONTAINER_NAME ) {
					bz2object* obj = dynamic_cast< bz2object* >( r->getChild(0) );
					// if this cast worked, then try to get its position and size
					if( obj ) {
						// get position and size
						osg::Vec3 p = obj->getPos();
						osg::Vec3 size = obj->getSize();
						
						// compute the largest dimension
						float maxSizeDim = max( size.x(), max( size.y(), size.z() ) );
						maxDim = max( maxSizeDim, maxDim );
						
						// the group's position relative to the objects is (0,0,0), so just square the position and take the length
						float len2 = p.x()*p.x() + p.y()*p.y() + p.z()*p.z();
						
						// see if it's bigger than the maximum radius
						if( len2 > maxRadius2 ) {
							maxRadius2 = len2;
						}
					}
				}
			}
		}
	}
	
	// now find the maximum radius
	float maxRadius = sqrt( maxRadius2 ) + maxDim;
	
	// NOW we can build the geometry
	osg::Vec3Array* points = new osg::Vec3Array();
	
	// primitive set
	osg::DrawElementsUInt* primitives = new osg::DrawElementsUInt( osg::PrimitiveSet::TRIANGLE_STRIP, 0 );
	
	int index = 0;
	// increment by degrees (less float-point error)
	for( float angle = 0.0f; angle < 360.0f; angle += 5.0f, index += 2 ) {
		float radAngle = osg::DegreesToRadians( angle );
		
		// add mesh data
		points->push_back( osg::Vec3( maxRadius * cos( radAngle ), maxRadius * sin( radAngle ), getPos().z() ));
		points->push_back( osg::Vec3( maxRadius * cos( radAngle ), maxRadius * sin( radAngle ), getPos().z() + 3 ));
	
		// add primitive indexes
		primitives->push_back( index );
		primitives->push_back( index + 1 );
	}
	
	// make it loop back
	primitives->push_back( 0 );
	primitives->push_back( 1 );
	
	// remove the pink ring if it already exists
	if( this->containsNode( geoRing.get() ) && this->geoRing.get() != NULL )
		this->removeChild( geoRing.get() );
	
	// make a new pink ring node
	this->geoRing = new osg::Geode();
	
	// build that geode!
	osg::Geometry* geometry = new osg::Geometry();
	geometry->setVertexArray( points );
	geometry->addPrimitiveSet( primitives );
	this->geoRing->addDrawable( geometry );
	
	// just name it
	this->geoRing->setName("group_container");
	
	// make it fushia
	SceneBuilder::assignMaterial( osg::Vec4( 1.0, 0.0, 1.0, 1.0 ),
								  osg::Vec4( 1.0, 0.0, 1.0, 1.0 ),
								  osg::Vec4( 1.0, 0.0, 1.0, 1.0 ),
								  osg::Vec4( 0.0, 0.0, 0.0, 0.0 ),
								  0.0,
								  0.0,
								  this->geoRing.get(),
								  osg::StateAttribute::OVERRIDE );
	
	this->addChild( this->geoRing.get() );
}

// re-compute the list of objects contained in the group
void group::updateObjects() {
	
	// get the "define" reference
	define* _def = dynamic_cast< define* >(Model::command( MODEL_GET, "define", getName() ));
	
	// reload the children
	setDefine( _def );
	
	// update the geometry
	buildGeometry();
}

// set the associated definition
void group::setDefine( define* def ) {
	this->def = def;
	//this->setName( def->getName() ); 
	
	// reload the children
	this->computeChildren();
	
	// recompute the geometry
	this->buildGeometry();
}

// set the children
void group::computeChildren() {
	// remove all current objects
	if( this->container->getNumChildren() > 0 )
		this->container->removeChildren(0, this->container->getNumChildren());
		
	// remove all bzw1 objects (and de-alloc them)
	if( this->bzw1_containers.size() > 0 ) {
		for( map< osg::ref_ptr< bz2object >, osg::ref_ptr< Renderable > >::iterator i = this->bzw1_containers.begin(); i != this->bzw1_containers.end(); ++i) {
			if( i->second != NULL )
				i->second->removeChildren( 0, i->second->getNumChildren());
		}
		
		// clear the map
		this->bzw1_containers.clear();
	}
	
	// if the def is valid, add the objects
	if( def != NULL ) {
		// get the objects
		vector< osg::ref_ptr< bz2object > > objects = this->def->getObjects();
		// put each object inside a PositionAttitudeTransform
		// add them as children of this object
		if( objects.size() > 0 ) {
			//setup progress bar window
			Fl_Window* progressWin = new Fl_Window(320,90, "Constructing Group");
			progressWin->begin();                         // add progress bar to it..
			Fl_Box* pbox = new Fl_Box(FL_FLAT_BOX,10,20,300,30,"...");
			pbox->align(FL_ALIGN_LEFT | FL_ALIGN_TOP| FL_ALIGN_INSIDE| FL_ALIGN_CLIP | FL_ALIGN_WRAP);
			pbox->labelfont(FL_BOLD);
			pbox->labelsize(12);
			Fl_Progress* progress = new Fl_Progress(10,50,300,30);
			progress->minimum(0);               // set progress bar attribs..
			progress->maximum(objects.size());
			progressWin->end();                           // end of adding to window
			
			//show progress
			progress->value(0);
			progressWin->set_modal();
			string progressText = "Calculating Center";
			progressWin->show();
			progressWin->position(progressWin->x(), progressWin->y() + 130);
			
			// first, compute the group's center
			/*
			 float x = 0.0f, y = 0.0f, z = 0.0f;
			for( vector< osg::ref_ptr< bz2object > >::iterator i = objects.begin(); i != objects.end(); i++ ) {
				x += i->get()->getPos().x();
				y += i->get()->getPos().y();
				z += i->get()->getPos().z();
				
				// update progress bar
				progress->value(progress->value()+1);
				float percentage = ((float)progress->value()/(float)objects.size())*100;
				printf("%f\n", percentage);
				string progressLabel = itoa((int)percentage) + "%";
				progress->label( progressLabel.c_str() );
				pbox->label(progressText.c_str());
				Fl::check();
			}
			
			x /= objects.size();
			y /= objects.size();
			z /= objects.size();
			
			// average positions of children
			container->setPosition( osg::Vec3( x, y, z ) );
			*/
			 
			progressText = "Adding Children";
			// update progress bar
			progress->value(0);
			float percentage = ((float)progress->value()/(float)objects.size())*100;
			printf("%f\n", percentage);
			string progressLabel = itoa((int)percentage) + "%";
			progress->label( progressLabel.c_str() );
			pbox->label(progressText.c_str());
			Fl::check();
			
			// add the children to the container node, but offset their positions by the group's position
			// (i.e. apply a SHIFT transformation
			for( vector< osg::ref_ptr< bz2object > >::iterator i = objects.begin(); i != objects.end(); i++ ) {
				// if this is a bzw2 object, then just add it to the container
				if( (*i)->isKey("spin") || (*i)->isKey("shift") || (*i)->isKey("scale") || (*i)->isKey("shear") ) {
					progressText = "Adding " + (*i)->getHeader() + " named " + (*i)->getName();
					// update progress bar
					pbox->label(progressText.c_str());
					Fl::check();
					// make a clone of the object
					bz2object* obj = SceneBuilder::cloneBZObject( i->get() );
					
					// add the object to the container
					this->container->addChild( obj );

					// set the position of this object relative to the center of the group
					//obj->setPos( obj->getPos() - container->getPosition() );
					
					//printf(" added %s\n", (*i)->getName().c_str() );
					progressText = "Added: " + (*i)->getHeader() + " named " + (*i)->getName();
				}
					
				// otherwise, create a bzw1 container (i.e. a transformation node that has the inverse
				// transformation of the container, because bzw1 objects do NOT share all of the group's transformations.
				// This is for cosmetic purposes only.
				/*else {
					// make a new node
					Renderable* r = new Renderable( i->get() );
					// name it as such
					r->setName( BZW1_CONTAINER_NAME );
					
					// add the current node as a child
					r->addChild( i->get() );
					
					// map the child to this node
					this->bzw1_containers[ i->get() ] = r;
					
					// add the bzw1 container to the main container.
					this->container->addChild( r );
				}*/
				
				// update progress bar
				progress->value(progress->value()+1);
				float percentage = ((float)progress->value()/(float)objects.size())*100;
				printf("%f\n", percentage);
				string progressLabel = itoa((int)percentage) + "%";
				progress->label( progressLabel.c_str() );
				pbox->label(progressText.c_str());
				Fl::check();
			}
			//cleanup progress bar window
			progress->value(progress->maximum());
			progress->label( "100%" );
			Fl::wait(0.22);
			progressWin->hide();
			delete(progress);
			delete(progressWin);
		}
	}
	
	setThisNode( container.get() );
}

// propogate messages to child objects when appropriate
void group::propogateEventToChildren( UpdateMessage& message ) {
	// don't do anything if there isn't a container node
	if( !this->container.get() )
		return;
		
	// iterate over the children
	for( unsigned int i = 0; i < this->container->getNumChildren(); i++ ) {
		// get an object
		bz2object* obj = dynamic_cast< bz2object* >( this->container->getChild( i ) );
		if( !obj )
			continue;
			
		// send it the event
		obj->update( message );
	}
}


