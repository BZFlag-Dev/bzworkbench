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

#include "objects/bz2object.h"

#include <cmath>
#include <osg/ShadeModel>

// default constructor
bz2object::bz2object(const char* name, const char* keys):
	Renderable(),
	DataEntry(name, keys)
{
	thisNode = NULL;

	transformations = new BZTransform();
	orientation = new Renderable();
	addChild( transformations.get() );
	transformations->addChild( orientation.get() );
	MaterialSlot mslot;
	mslot.defaultMaterial = NULL;
	materialSlots[""] = mslot;
	PhysicsSlot pslot;
	pslot.phydrv = NULL;
	physicsSlots[""] = pslot;
	setSelected( false );
	setName( SceneBuilder::makeUniqueName( getHeader().c_str() ) );

	savedStateSet = NULL;
	drivethrough = false;
	shootthrough = false;
	flatshading = false;
	smoothbounce = false;
};

// constructor with node data
bz2object::bz2object(const char* name, const char* keys, osg::Node* node ):
	Renderable( node ),
	DataEntry(name, keys)
{

	thisNode = node;

	transformations = new BZTransform();
	orientation = new Renderable();
	addChild( transformations.get() );
	transformations->addChild( orientation.get() );
	MaterialSlot mslot;
	mslot.defaultMaterial = NULL;
	materialSlots[""] = mslot;
	PhysicsSlot pslot;
	pslot.phydrv = NULL;
	physicsSlots[""] = pslot;
	setSelected( false );
	setName( "(unknown bz2object)" );

	savedStateSet = NULL;
	drivethrough = false;
	shootthrough = false;
	flatshading = false;
	smoothbounce = false;
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
	if ( (key == "name" || key == header) && isKey( "name" ) ) {
		Object::setName( value );
		return true;
	}

	else if ( key == header ) {
		return true; // just ignore
	}

	// get the position
	else if ( ( key == "pos" || key == "position" ) && isKey( "position" ) ) {
		orientation->setPosition( Point3D( value.c_str() ) );
		return true;
	}

	// get the rotation
	else if ( ( key == "rot" || key == "rotation" ) && isKey( "rotation" ) ) {
		orientation->setRotationZ( atof( value.c_str() ) );
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

		this->transformations->parse( line );
		return true;
	}

	else if ( key == "texsize" && isKey( "texsize" ) ) {
		texsize = Point2D( value.c_str() );
		return true;
	}

	else if ( key == "texoffset" && isKey( "texoffset" ) ) {
		texoffset = Point2D( value.c_str() );
		return true;
	}
	
	else if ( key == "passable"  && isKey( "passable" ) ) {
		drivethrough = true;
		shootthrough = true;
		return true;
	}

	else if ( key == "drivethrough" && isKey( "drivethrough" ) ) {
		drivethrough = true;
		return true;
	}

	else if ( key == "shootthrough" && isKey( "shootthrough" ) ) {
		shootthrough = true;
		return true;
	}

	else if ( key == "flatshading" && isKey("flatshading" ) ) { 
		flatshading = true;
		return true;
	}

	else if ( key == "smoothbounce" && isKey( "smoothbounce" ) ) {
		smoothbounce = true;
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

					return true;
				}
			}

			// otherwise it might apply to all faces
			else if ( key == "phydrv" ) {
				physics* phys = (physics*)Model::command( MODEL_GET, "phydrv", value );
				i->second.phydrv = phys;

				return true;
			}
		}
	}
	
	if ( key == "ambient" ) {
		material* mat = new material();
		mat->setMatType(material::MAT_AMBIENT);
		mat->setAmbient( RGBA(value.c_str()) );
		for ( map< string, MaterialSlot >::iterator i = materialSlots.begin(); i != materialSlots.end(); i++ ) {
			//only apply to ALL slot
			if (mat && i->first == "")
				i->second.materials.push_back( mat );
		}
		return true;
	}
	
	if ( key == "color" || key == "diffuse" ) {
		material* mat = new material();
		mat->setMatType(material::MAT_DIFFUSE);
		mat->setDiffuse( RGBA(value.c_str()) );
		for ( map< string, MaterialSlot >::iterator i = materialSlots.begin(); i != materialSlots.end(); i++ ) {
			//only apply to ALL slot
			if (mat && i->first == "")
				i->second.materials.push_back( mat );
		}
		return true;
	}
	
	if ( key == "specular" ) {
		material* mat = new material();
		mat->setMatType(material::MAT_SPECULAR);
		mat->setSpecular( RGBA(value.c_str()) );
		for ( map< string, MaterialSlot >::iterator i = materialSlots.begin(); i != materialSlots.end(); i++ ) {
			//only apply to ALL slot
			if (mat && i->first == "")
				i->second.materials.push_back( mat );
		}
		return true;
	}
	
	if ( key == "emission" ) {
		material* mat = new material();
		mat->setMatType(material::MAT_EMISSION);
		mat->setEmission( RGBA(value.c_str()) );
		for ( map< string, MaterialSlot >::iterator i = materialSlots.begin(); i != materialSlots.end(); i++ ) {
			//only apply to ALL slot
			if (mat && i->first == "")
				i->second.materials.push_back( mat );
		}
		return true;
	}
	
	if ( key == "texture" ) {
		material* mat = new material();
		mat->setMatType(material::MAT_TEXTURE);
		mat->setTexture( value.c_str() );
		for ( map< string, MaterialSlot >::iterator i = materialSlots.begin(); i != materialSlots.end(); i++ ) {
			//only apply to ALL slot
			if (mat && i->first == "")
				i->second.materials.push_back( mat );
		}
		return true;
	}
	
	if ( isKey( "matref" ) ) {
		bool wasHandled = false;
		for ( map< string, MaterialSlot >::iterator i = materialSlots.begin(); i != materialSlots.end(); i++ ) {
			//printf("MaterialSlot first: %s\n", i->first.c_str());
			// check for matching alias
			bool aliasMatch = false;
			for ( vector< std::string >::iterator a = i->second.alias.begin(); a != i->second.alias.end(); a++ ) {
				//printf("MaterialSlot second alias: %s\n", (*a).c_str());
				if ( key == *a ) {
					aliasMatch = true;
				}
			}
			// first check if it is a per face matref
			if ( key == i->first || aliasMatch ) {
				string realKey = BZWParser::key( value.c_str() );

				if ( realKey == "matref" ) {
					string realValue = BZWParser::value( "matref", value.c_str() );

					// use the model to resolve the reference into a material pointer
					material* mat = (material*)Model::command( MODEL_GET, "material", realValue );
					if( mat )
						i->second.materials.push_back( mat );
					else
						throw BZWReadError( this, string( "Couldn't find material, " ) + realValue );

					wasHandled = true;
				}
			
			}

			// otherwise it might apply to all faces
			else if ( key == "matref" ) {
				material* mat = (material*)Model::command( MODEL_GET, "material", value );
				if( mat )
					i->second.materials.push_back( mat );
				else
					throw BZWReadError( this, string( "Couldn't find material, " ) + value );

				return true;
			}
		}
		if(wasHandled)
			return true;
	}

	// ran out of possibilities throw an error
	throw BZWReadError( this, string( "Unknown key, " ) + key );
}

// called after done parsing to finalize the changes
void bz2object::finalize() {
	
	refreshMaterial();
	
	// update the transformation stack
	this->transformations->refreshMatrix();

	// update the shade model
	updateShadeModel();
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
		if ( obj->getPosition() != osg::Vec3( 0, 0, 0 ) )
			ret += "  position " + Point3D( obj->getPosition() ).toString();

	// add rotation key/value to the string if supported
	if(obj->isKey("rotation"))
		if ( obj->orientation->getRotation().z() != 0.0f )
			ret += "  rotation " + string( ftoa(obj->orientation->getRotation().z()) ) + "\n";

	// add size key/value to the string if supported
	if(obj->isKey("size"))
		if ( obj->getSize() != osg::Vec3( 0, 0, 0 ) )
			ret += "  size " + Point3D( obj->getSize() ).toString();

	// add obstacle key/value(s) to the string if supported
	if(obj->isKey("passable") || obj->isKey("drivethrough") || obj->isKey("shootthrough") ){
		if ( obj->drivethrough && obj->shootthrough && obj->isKey("passable") ){
			ret += "  passable\n";
		}else{
			if ( obj->drivethrough && obj->isKey("drivethrough") )
				ret += "  drivethrough\n";
			if ( obj->shootthrough && obj->isKey("shootthrough") )
				ret += "  shootthrough\n";
		}
	}
	
	// add all transformations to the string if they are supported
	ret += obj->transformations->toString();

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
					// see if we are a reference or an object material
					if((*j)->getMatType() == material::MAT_REF){
						//output matref
						if ( i->first == "" )
							ret += "  matref " + (*j)->getName() + "\n";
						else 
							ret += "  " + i->first + " matref " + (*j)->getName() + "\n";
					}else{
						//output object's material attributes
						if ( i->first == "" ){
							ret += (*j)->toStringClean();
						}else{
							//FIXME - prefix each line, not just the first
							// also trim off extra indent spaces
							ret += "  " + i->first + " " + (*j)->toStringClean();
						}
					}
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

			vector< TransformData >* newTransformations = message.getAsTransformationStack();
			transformations->setData( *newTransformations );
			finalize();
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
	bool didAllSides = false;
	osg::Texture2D* defaultTexture;
	for ( map< string, MaterialSlot >::iterator i = materialSlots.begin(); i != materialSlots.end(); i++ ) {
		defaultTexture = NULL;
		osg::StateSet* mat = i->second.defaultMaterial;
		if(mat != NULL)
			defaultTexture = dynamic_cast< osg::Texture2D* >((mat->getTextureAttribute( 0,  osg::StateAttribute::TEXTURE ) ));
		if ( i->second.materials.size() > 0 ) {
			mat = material::computeFinalMaterial( i->second.materials );
			if(!((material*)mat)->getNoTextures() && ((material*)mat)->getCurrentTexture() == NULL && defaultTexture != NULL){
				mat->setTextureMode( 0, GL_TEXTURE_2D, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
				mat->setTextureAttribute( 0, defaultTexture );
			}
		}
		if ( i->first == "" ){
			//SceneBuilder::assignBZMaterial( mat, getThisNode() );
			// apply to all slots - this must happen prior to appling the individual sides
			for ( map< string, MaterialSlot >::iterator ii = materialSlots.begin(); ii != materialSlots.end(); ii++ ) {
				if(ii->first != ""){
					defaultTexture = NULL;
					osg::StateSet* dmat = ii->second.defaultMaterial;
					if(dmat != NULL)
						defaultTexture = dynamic_cast< osg::Texture2D* >((dmat->getTextureAttribute( 0,  osg::StateAttribute::TEXTURE ) ));
					if ( i->second.materials.size() > 0 ) {
						mat = material::computeFinalMaterial( i->second.materials );
						if(!((material*)mat)->getNoTextures() && ((material*)mat)->getCurrentTexture() == NULL && defaultTexture != NULL){
							mat->setTextureMode( 0, GL_TEXTURE_2D, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
							mat->setTextureAttribute( 0, defaultTexture );
						}
					}
					SceneBuilder::assignBZMaterial( mat, ii->second.node );
				}
			}
			didAllSides = true;
		}else{
			if( didAllSides == true ){
				if(mat != i->second.defaultMaterial){
					SceneBuilder::assignBZMaterial( mat, i->second.node );
				}
			}else{
				SceneBuilder::assignBZMaterial( mat, i->second.node );
			}
		}
	}
}

void bz2object::setMaterials( vector< material* >& _materials, std::string slot ) { 
	this->materialSlots[ slot ].materials = _materials; 
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
	orientation->setRotationZ( tmp );
}

// set the shade model based on the value of flatShading
void bz2object::updateShadeModel() {
	// get state set
	osg::StateSet* states = getOrCreateStateSet();

	// get the shade model
	osg::ShadeModel* shadeModel = dynamic_cast< osg::ShadeModel* >( states->getAttribute( osg::StateAttribute::SHADEMODEL ) );
	if( shadeModel == NULL ) {
		shadeModel = new osg::ShadeModel();		// if one doesn't exist, then make one
	}

	if( flatshading ) {
		shadeModel->setMode( osg::ShadeModel::FLAT );
	}
	else {
		shadeModel->setMode( osg::ShadeModel::SMOOTH );
	}

	// set the shade model
	states->setAttribute( shadeModel );
}
