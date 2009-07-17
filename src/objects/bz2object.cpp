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

#include "objects/bz2object.h"

#include <cmath>

// default constructor
bz2object::bz2object(const char* name, const char* keys):
	Renderable(),
	DataEntry(name, keys)
{
	thisNode = NULL;

	transformations = vector< osg::ref_ptr<BZTransform> >();
	MaterialSlot mslot;
	mslot.defaultMaterial = NULL;
	materialSlots[""] = mslot;
	PhysicsSlot pslot;
	pslot.phydrv = NULL;
	physicsSlots[""] = pslot;
	setSelected( false );
	setName( "(unknown bz2object)" );

	savedStateSet = NULL;

};

// constructor with node data
bz2object::bz2object(const char* name, const char* keys, osg::Node* node ):
	Renderable( node ),
	DataEntry(name, keys)
{

	thisNode = node;

	transformations = vector< osg::ref_ptr<BZTransform> >();
	MaterialSlot mslot;
	mslot.defaultMaterial = NULL;
	materialSlots[""] = mslot;
	PhysicsSlot pslot;
	pslot.phydrv = NULL;
	physicsSlots[""] = pslot;
	setSelected( false );
	setName( "(unknown bz2object)" );

	savedStateSet = NULL;
}

// getter
string bz2object::get(void)
{
  return toString();
}

// parse a single bzw line
bool bz2object::parse( std::string& line ) {
	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );


	// get the name (break if there are more than one)
	if ( key == "name" && isKey( "name" ) ) {
		setName( value );
		return true;
	}

	// get the position
	else if ( key == "position" && isKey( "position" ) ) {
		setPosition( Point3D( value.c_str() ) );
		return true;
	}

	// get the rotation
	else if ( key == "rotation" && isKey( "rotation" ) ) {
		Renderable::setRotationZ( atof( value.c_str() ) );
		return true;
	}

	// get the size
	else if ( key == "size" && isKey( "size" ) ) {
		setSize( Point3D( value.c_str() ) );
		return true;
	}

	// get the transformation occurences
	else if( ( key == "shear" && isKey( "shear" ) ) ||
		( key == "shift" && isKey( "shift" ) ) ||
		( key == "scale" && isKey( "scale" ) ) ||
		( key == "spin" && isKey( "spin" ) ) ) {

		this->newTransformations.push_back( new BZTransform( line ) );
		return true;
	}

	// get phydrvs
	if ( isKey( "phydrv" ) ) {
		for ( map< string, PhysicsSlot >::iterator i = physicsSlots.begin(); i != physicsSlots.end(); i++ ) {
			// first check if it is a per face physics driver
			if ( key == i->first ) {
				string realKey = BZWParser::key( value.c_str() );

				if ( realKey == "phydrv" ) {
					string realValue = BZWParser::value( "phydrv", value.c_str() );

					physics* phys = (physics*)Model::command( MODEL_GET, "phydrv", realValue );

					if ( phys == NULL )
						throw BZWReadError( this, string( "Couldn't find phydrv, " ) + realValue );

					i->second.phydrv = phys;
				}
			}

			// otherwise it might apply to all faces
			else if ( key == "phydrv" ) {
				physics* phys = (physics*)Model::command( MODEL_GET, "phydrv", value );
				i->second.phydrv = phys;
			}
		}
	}

	if ( isKey( "matref" ) ) {
		for ( map< string, MaterialSlot >::iterator i = materialSlots.begin(); i != materialSlots.end(); i++ ) {

			// first check if it is a per face matref
			if ( key == i->first ) {
				string realKey = BZWParser::key( value.c_str() );

				if ( realKey == "matref" ) {
					string realValue = BZWParser::value( "matref", value.c_str() );

					// use the model to resolve the reference into a material pointer
					/*material* mat = (material*)Model::command( MODEL_GET, "material", realValue );
					if( mat )
						i->second.materials.push_back( mat );
					else
						throw BZWReadError( this, string( "Couldn't find material, " ) + realValue );*/
				}
			
			}

			// otherwise it might apply to all faces
			else if ( key == "matref" ) {
				/*material* mat = (material*)Model::command( MODEL_GET, "material", value );
				if( mat )
					i->second.materials.push_back( mat );
				else
					throw BZWReadError( this, string( "Couldn't find material, " ) + value );*/
			}
		}
	}

	return true;
}

// called after done parsing to finalize the changes
void bz2object::finalize() {
	for ( map< string, MaterialSlot >::iterator i = materialSlots.begin(); i != materialSlots.end(); i++ ) {
		// assign the material
		if ( i->second.materials.size() > 0 ) {
			if ( i->first != "" )
				i->second.node->setStateSet( material::computeFinalMaterial(i->second.materials) );
			else
				getThisNode()->setStateSet( material::computeFinalMaterial(i->second.materials) );
		}
	}

	// update the transformation stack
	this->recomputeTransformations( &newTransformations );
}

// toString
string bz2object::toString(void)
{
	return getHeader() + "\n" + BZWLines( this ) + "end\n";
}

// this method only returns the (indented) lines in the BZW text and is meant to be called by derived classes
string bz2object::BZWLines( bz2object* obj )
{
	string ret = string("");

	// add name key/value to the string if supported
	if(obj->isKey("name") && obj->getName().length() > 0)
		ret += "  name " + obj->getName() + "\n";

	// add position key/value to the string if supported
	if(obj->isKey("position"))
		ret += "  position " + Point3D( obj->getPosition() ).toString();

	// add rotation key/value to the string if supported
	if(obj->isKey("rotation") && !obj->isKey("spin"))
		ret += "  rotation " + string( ftoa(obj->getRotation().z()) ) + "\n";

	// add size key/value to the string if supported
	if(obj->isKey("size"))
		ret += "  size " + Point3D( obj->getSize() ).toString();

	// add a scale if size isn't supported (this is the case with groups, for example)
	if( !obj->isKey("size") && obj->isKey("scale") )
		ret += "  scale " + Point3D( obj->getSize() ).toString();

	// add all transformations to the string if they are supported
	for(vector< osg::ref_ptr<BZTransform> >::iterator i = obj->transformations.begin(); i != obj->transformations.end(); i++) {
		if(obj->isKey((*i)->getHeader().c_str()) && (*i)->isApplied())
			ret += "  " + (*i)->toString();
	}

	// add phydrv key/value to the string if supported and if defined
	if(obj->isKey("phydrv")) {
		for ( map< string, PhysicsSlot >::iterator i = obj->physicsSlots.begin(); i != obj->physicsSlots.end(); i++ ) {
			if ( i->second.phydrv.get() != NULL ) {
				if ( i->first == "" )
					ret += "  phydrv " + i->second.phydrv->getName() + "\n";
				else 
					ret += "  " + i->first + " phydrv " + i->second.phydrv->getName() + "\n";
			}
		}
	}

	// add all matref key/value pairs to the string if supported and defined
	if(obj->isKey("matref")) {
		for ( map< string, MaterialSlot >::iterator i = obj->materialSlots.begin(); i != obj->materialSlots.end(); i++ ) {
			for(vector< material* >::iterator j = i->second.materials.begin(); j != i->second.materials.end(); j++) {
				if ( *j != NULL ) {
					if ( i->first == "" )
						ret += "  matref " + (*j)->getName() + "\n";
					else 
						ret += "  " + i->first + " matref " + (*j)->getName() + "\n";
				}
			}
		}
	}

	// add unused text
	ret += obj->getUnusedText();

	return ret;
}

// event handler
int bz2object::update( UpdateMessage& message )
{
	switch( message.type ) {
		case UpdateMessage::SET_TRANSFORMATIONS: {		// update the transformation stack
			if( !( isKey("spin") || isKey("shift") || isKey("shear") || isKey("scale") ) )
				break;

			vector< osg::ref_ptr< BZTransform > >* newTransformations = message.getAsTransformationStack();
			recomputeTransformations( newTransformations );
			break;
		}

		case UpdateMessage::SET_MATERIALS: {		// update the material stack
			if( !isKey("matref") )
				break;

			vector< material* >* materialList = message.getAsMaterialList();
			if( materialList != NULL ) {
				materialSlots[""].materials = *materialList;
				refreshMaterial();
			}
			break;
		}

		case UpdateMessage::ADD_MATERIAL:	{ 		// push a material
			if( !isKey("matref") )
				break;

			addMaterial( message.getAsMaterial() );
			break;
		}

		case UpdateMessage::REMOVE_MATERIAL: {			// remove a material
			if( !isKey("matref") )
				break;

			removeMaterial( message.getAsMaterial() );
			break;
		}

		case UpdateMessage::UPDATE_MATERIAL: {		// update a material in the material list, or add it if it doesn't exist
			if( !isKey("matref") )
				break;

			material* mat = message.getAsMaterial();

			// search for the material in the list
			if( materialSlots[""].materials.size() > 0 ) {
				unsigned int i = 0;
				for( vector< material* >::iterator itr = materialSlots[""].materials.begin(); itr != materialSlots[""].materials.end(); itr++, i++) {
					if( *itr == mat ) {
						printf(" updating material...\n");
						materialSlots[""].materials[i] = mat;
						break;
					}
				}

				refreshMaterial();
			}
			else {
				materialSlots[""].materials.push_back( mat );
				printf(" adding material...\n" );
				refreshMaterial();
			}

			break;
		}
		default:
			return 0;		// not handled
	}

	return 1;
}

// update the transformation stack with new ones
void bz2object::recomputeTransformations( vector< osg::ref_ptr< BZTransform > >* newTransformations )
{
	// clear all current transformations
	if( this->transformations.size() > 0 ) {
		this->removeChild( transformations[0].get() );
		this->transformations[ this->transformations.size() - 1 ]->removeChild( getThisNode() );
		this->transformations.clear();
	} else {
	  // if there are no current transformations, then try to remove the endShift from the startShift
	  this->removeChild( getThisNode() );
	}

	this->transformations = *newTransformations;	// copy the array over

	// add the new transformations
	if( this->transformations.size() > 0 ) {
		// add each transformation to the next
		this->addChild( transformations[0].get() );

		for( unsigned int i = 1; i < transformations.size(); i++ ) {
			transformations[i-1]->addChild( transformations[i].get() );
		}

		transformations[ transformations.size() - 1 ]->addChild( getThisNode() );
	} else {
		this->addChild( getThisNode() );	// connect the start and end shifts if there are no transformations in between
	}
}

// add a material to the object
void bz2object::addMaterial( material* mat, string slot ) {
	if( mat != NULL ) {
		materialSlots[ slot ].materials.push_back( mat );
		refreshMaterial();
	}
}

// insert a material
void bz2object::insertMaterial( unsigned int index, material* mat, string slot )
{
	if( index > materialSlots[ slot ].materials.size() - 1 )
		return;

	if( materialSlots[ slot ].materials.size() == 0 ) {
		materialSlots[ slot ].materials.push_back( mat );
	} else {
		vector< material* >::iterator itr = materialSlots[ slot ].materials.begin();
		for( unsigned int i = 0; i < materialSlots[ slot ].materials.size(); i++, itr++ ) {
			if( i == index ) {
				materialSlots[ slot ].materials.insert( itr, mat );
				break;
			}
		}
	}

	refreshMaterial();
}

// remove a material
void bz2object::removeMaterial( material* mat, string slot )
{
	if( materialSlots[ slot ].materials.size() == 0 || mat == NULL )
		return;

	for( vector< material* >::iterator i = materialSlots[ slot ].materials.begin(); i != materialSlots[ slot ].materials.end(); i++ ) {
		if( *i == mat ) {
			materialSlots[ slot ].materials.erase( i );
			break;
		}
	}

	refreshMaterial();
}

// remove a material by index
void bz2object::removeMaterial( unsigned int index, string slot )
{
	if( index >= materialSlots[ slot ].materials.size() || index < 0 )
		return;

	vector< material* >::iterator itr = materialSlots[ slot ].materials.begin();
	for( unsigned int i = 0; i < materialSlots[ slot ].materials.size(); i++, itr++ ) {
		if( i == index ) {
			materialSlots[ slot ].materials.erase( itr );
			break;
		}
	}

	refreshMaterial();
}

vector<string> bz2object::materialSlotNames() {
	vector<string> ret;
	for ( map< string, MaterialSlot >::iterator i = materialSlots.begin(); i != materialSlots.end(); i++ ) {
		ret.push_back( i->first );
	}
	return ret;
}

vector<string> bz2object::physicsSlotNames() {
	vector<string> ret;
	for ( map< string, PhysicsSlot >::iterator i = physicsSlots.begin(); i != physicsSlots.end(); i++ ) {
		ret.push_back( i->first );
	}
	return ret;
}

// recompute the material
void bz2object::refreshMaterial()
{
	int index = 0;
	for ( map< string, MaterialSlot >::iterator i = materialSlots.begin(); i != materialSlots.end(); i++ ) {
		osg::StateSet* mat = i->second.defaultMaterial;
		if ( i->second.materials.size() > 0 ) {
			mat = material::computeFinalMaterial( i->second.materials );
		}

		if ( i->first == "" )
			SceneBuilder::assignBZMaterial( mat, getThisNode() );
		else
			SceneBuilder::assignBZMaterial( mat, i->second.node );
	}
}

void bz2object::setMaterials( vector< material* >& _materials, std::string slot ) { 
	this->materialSlots[ slot ].materials = _materials; 
}

// add a transformation
void bz2object::addTransformation( BZTransform* t ) {
	if( t != NULL ) {
		if( this->transformations.size() > 0 ) {
			// if we actually have othet transformations, we'll need to insert the transformation between
			// endShift and the last transformation
			t->addChild( getThisNode() );
			this->transformations[ this->transformations.size() - 1 ]->removeChild( getThisNode() );
			this->transformations.push_back( t );
		}
		else {
			// if there are no transformations, then insert this one betweens startShift and endShift
			t->addChild( getThisNode() );
			this->removeChild( getThisNode() );
			this->addChild( t );
			this->transformations.push_back( t );
		}
	}
}

// insert a transformation
void bz2object::insertTransformation( unsigned int index, BZTransform* t ) {
	if( t == NULL )
		return;		// don't deal with NULL transformations

	// don't deal with out-of-bounds indexes when there are already transformations defined.
	if( this->transformations.size() >= index )
		return;

	// if there are no transformations, then insert the transformation between startShift and endShift
	if( this->transformations.size() == 0 ) {
		t->addChild( getThisNode() );
		this->removeChild( getThisNode() );
		this->addChild( t );
		this->transformations.push_back( t );
	}
	// otherwise...
	else {
		// if the index is 0, then insert it between startShift and the start of the transformation list
		if( index == 0 ) {
			osg::ref_ptr< BZTransform > t_ref = osg::ref_ptr< BZTransform >( t );
			// remove the head of the list from startShift
			this->removeChild( this->transformations[0].get() );
			// insert the transformation to the head of the list
			this->transformations.insert( this->transformations.begin(), t );
			// add the transformation as a child of startShift
			this->addChild( t_ref.get() );
		}
		// if the index is the last one...
		else if( index == transformations.size()-1 ) {
			// just invoke the addTransformation() method, since it appends transformations
			this->addTransformation( t );
		}
		// otherwise, the index is somewhere in between, so we need to advance an iterator over to that spot
		// and insert it
		else {
			// count over to the point of insertion
			vector< osg::ref_ptr< BZTransform > >::iterator itr = this->transformations.begin();
			unsigned int i = 0;
			// increment the iterator
			for(; i < index; i++, ++itr );
			// now insert the transformation
			this->transformations[i]->removeChild( this->transformations[i+1].get() );
			this->transformations.insert( itr, t );
			// now, transformations[i+1] is the transformation we just added
			this->transformations[i]->addChild( this->transformations[i+1].get() );
			this->transformations[i+1]->addChild( this->transformations[i+2].get() );
		}
	}
}

// remove a transformation by pointer (NOTE: removes only the 1st occurence)
void bz2object::removeTransformation( BZTransform* t ) {
	if( this->transformations.size() == 0 )
		return;

	// iterate over the vector and pull the first occurence
	for( vector< osg::ref_ptr< BZTransform > >::iterator i = this->transformations.begin(); i != this->transformations.end(); ++i ) {
		if( i->get() == t ) {
			this->transformations.erase( i );
			return;
		}
	}

}

// remove a transformation by index
void bz2object::removeTransformation( unsigned int index ) {
	if( this->transformations.size() == 0 )
		return;

	// iterate over the vector to find the index
	unsigned int i = 0;
	vector< osg::ref_ptr< BZTransform > >::iterator itr = this->transformations.begin();
	for(; i < index; i++, ++itr );

	// remove the transformation at itr
	this->transformations.erase( itr );
}

void bz2object::snapTranslate( float size, osg::Vec3 position ) {
	osg::Vec3 tmp = position / size;
	tmp = osg::Vec3( osg::round( tmp.x() ), osg::round( tmp.y() ), osg::round( tmp.z() ) );
	tmp *= size;
	setPosition( tmp );
}

void bz2object::snapScale( float size, osg::Vec3 scale ) {
	osg::Vec3 tmp = scale / size;
	tmp = osg::Vec3( osg::round( tmp.x() ), osg::round( tmp.y() ), osg::round( tmp.z() ) );
	tmp *= size;
	setSize( tmp );
}

void bz2object::snapRotate( float size, float rotation ) {
	float tmp = rotation / size;
	tmp = osg::round( tmp );
	tmp *= size;
	setRotationZ( tmp );
}