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

#include "objects/material.h"

#include "model/SceneBuilder.h"

#include "objects/texturematrix.h"
#include "objects/dynamicColor.h"

using namespace std;

// default constructor
material::material() :
	DataEntry("material", "<name><texture><addtexture><matref><notextures><notexcolor><notexalpha><texmat><dyncol><ambient><diffuse><color><specular><emission><shininess><resetmat><spheremap><noshadow><noculling><nosort><noradar><nolighting><groupalpha><occluder><alphathresh>"),
	osg::StateSet() {
	name = SceneBuilder::makeUniqueName("material");
	type = MAT_REF;
	dynCol = NULL;
	_ambient = RGBA();
	_diffuse = RGBA();
	_specular = RGBA();
	_emission = RGBA();
	_shininess = 0.0f;
	hasAmbient = false;
	hasDiffuse = false;
	hasSpecular = false;
	hasEmission = false;
	hasShininess = false;
	hasAlphaThreshold = false;
	textures = vector< TextureInfo >();
	noTextures = false;
	noRadar = noShadow = noCulling = noLighting = noSorting = groupAlpha = occluder = false;
	alphaThreshold = 1.0f;
	
	setMode(GL_BLEND, osg::StateAttribute::ON);
	setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

	// allocate a material
	osg::Material* finalMaterial = new osg::Material();
	setAttribute( finalMaterial, osg::StateAttribute::ON  | osg::StateAttribute::OVERRIDE );

	// deactivate texturing
	setTextureMode( 0, GL_TEXTURE_2D, osg::StateAttribute::OFF  | osg::StateAttribute::OVERRIDE );
}

// getter
string material::get(void) { return toString(); }

// bzw methods
bool material::parse( std::string& line ) {
	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );
	
	// check if we reached the end of the section
	if ( key == "end" )
		return false;

	if ( key == "name") {
		name = value;
	}
	else if ( key == "resetmat" ) {
		reset();
	}
	else if ( key == "dyncol" ) { // get the dynamic color
		if(strcmp(value.c_str(), "-1") != 0){ // or not - should we do something for a default?
			dynamicColor* d = dynamic_cast<dynamicColor*>( Model::command( MODEL_GET, "dynamicColor", value ) );
			if ( d != NULL )
				dynCol = d;
			else
				throw BZWReadError( this, string( "Couldn't find dynamic color, " ) + value );
		}
	}
	else if ( key == "texmat" ) { // get the texture matrix
		//string value = BZWParser::value( "texmat", line.c_str() );
		if(strcmp(value.c_str(), "-1") != 0){ // or not - should we do something for a default?
			texturematrix* texmat = (texturematrix*)Model::command( MODEL_GET, "texturematrix", value);
			if ( texmat != NULL)
				setTextureMatrix( texmat );
			else
				throw BZWReadError( this, string( "Could not find texmat, " ) + value );
		}
	}
	else if ( key == "color" || key == "diffuse" ) { // get the diffuse colors
		setDiffuse( RGBA( value.c_str() ) );
	}
	else if ( key == "ambient" ) { // get the ambient colors
		setAmbient( RGBA( value.c_str() ) );
	}
	else if ( key == "emission" ) { // get the emission colors
		setEmission( RGBA( value.c_str() ) );
	}
	else if ( key == "specular" ) { // get the specular colors
		setSpecular( RGBA( value.c_str() ) );
	}
	else if ( key == "shininess" ) { // get the specular colors
		setShininess( atof( value.c_str() ) );
	}
	else if ( key == "texture" ) { // get the textures
		setTexture( value );
	}
	else if ( key == "addtexture" ) { // get the addtextures
		addTexture( value);
	}
	else if ( key == "notextures" ) { // get notextures
		noTextures = true;
	}
	else if ( key == "notexcolor" ) { // get notexcolor
		setNoTexColor( true );
	}
	else if ( key == "notexalpha" ) { // get notexalpha
		setNoTexAlpha( true );
	}
	else if ( key == "spheremap" ) { // get spheremap
		setSphereMap( true );
	}
	else if ( key == "noshadow" ) { // get noshadow
		setNoShadows( true );
	}
	else if ( key == "noculling" ) { // get noculling
		setNoCulling( true );
	}
	else if ( key == "nolighting" ) { // get nolighting
		setNoLighting( true );
	}
	else if ( key == "nosorting" ) { // get nosorting
		setNoSorting( true );
	}
	else if ( key == "noradar" ) { // get noradar
		setNoRadar( true );
	}
	else if ( key == "groupalpha" ) { // get groupalpha
		setGroupAlpha( true );
	}
	else if ( key == "occluder" ) { // get occluder
		setOccluder( true );
	}
	else if ( key == "alphathresh" ) { // get alpha threshold
		setAlphaThreshold( atof( value.c_str() ) );
	}
	else if ( key == "matref" ) {
		// get the materials from the model
		material* mat = (material*)Model::command( MODEL_GET, "material", value );
		if( mat != NULL )
			materials.push_back( mat );
		else
			throw BZWReadError( this, string( "Could not find matref, " ) + value );
	}
	else {
		throw BZWReadError( this, string( "Unknown key, " ) + key );
	}

	return true;
}

void material::finalize() {
	// compute the final material
	computeFinalMaterial();

	// compute the final texture
	computeFinalTexture();
}


string material::toStringClean(void) {
	string texString = string("");
	if( textures.size() > 0) {
		for(vector< TextureInfo >::iterator i = textures.begin(); i != textures.end(); i++) {
			TextureInfo info = *i;
			if( info.name.compare("") != 0 ){
				if (i == textures.begin())
					texString += "  texture " + info.name + "\n";
				else
					texString += "  addtexture " + info.name + "\n";
			}
			texString += 
			(info.matrix != NULL ? string("") + "  texmat " + info.matrix->getName() + "\n" : "") +
			(info.noColor == true ? "  notexcolor\n" : "") +
			(info.noAlpha == true ? "  notexalpha\n" : "") +
			(info.sphereMap == true ? "  spheremap\n" : "");
		}
	}
	
	string matString = string("");
	if( materials.size() > 0 ) {
		for(list< material* >::iterator i = materials.begin(); i != materials.end(); i++) {
			matString += "  matref " + (*i)->getName() + "\n";
		}
	}
	// colors
	string ambientStr, diffuseStr, specularStr, emissionStr;
	
	ambientStr = ( hasAmbient ? "  ambient " + RGBA(getAmbient()).toString() : "");
	diffuseStr = ( hasDiffuse ? "  diffuse " + RGBA(getDiffuse()).toString() : "");
	specularStr = ( hasSpecular ? "  specular " + RGBA(getSpecular()).toString() : "");
	emissionStr = ( hasEmission ? "  emission " + RGBA(getEmission()).toString() : "");
	
	return (dynCol != NULL ? string("  dyncol ") + dynCol->getName() + "\n" : string("")) +
		(noTextures == true ? "  notextures\n" : "") +
		(noShadow == true ? "  noshadow\n" : "") +
		(noCulling == true ? "  noculling\n" : "") +
		(noSorting == true ? "  nosorting\n" : "") +
		(noLighting == true ? "  nolighting\n" : "") +
		(noRadar == true ? "  noradar\n" : "") +
		(groupAlpha == true ? "  groupalpha\n" : "") +
		(occluder == true ? "  occluder\n" : "") +
		ambientStr +
		diffuseStr +
		specularStr +
		emissionStr +
		(hasShininess ? "  shininess " + string(ftoa(getShininess())) + "\n" : "") +
		(hasAlphaThreshold ?   "  alphathresh " + string(ftoa(alphaThreshold)) + "\n" : "") +
		texString +
		matString ;
}

// tostring
string material::toString(void) {
	return string("material\n") +
				  (name.length() == 0 ? string("# name\n") : "  name " + name + "\n")  +
				  toStringClean() +
				  getUnusedText() +
				  "end\n";
}

void material::addTexture(const std::string& name) {
	TextureInfo info;
	info.name = name;
	info.matrix = NULL;
	info.combineMode = osg::TexEnv::DECAL;
	info.noAlpha = false;
	info.noColor = false;
	info.sphereMap = false;

	textures.push_back( info );
}

void material::setTexture(const std::string& name) {
	if (textures.size() <= 0)
		addTexture( name );
	else
		textures[ textures.size() - 1 ].name = name;

	computeFinalTexture();
}

void material::setTextureMatrix( texturematrix* texmat ) {
	textures[ textures.size() - 1 ].matrix = texmat;
}

void material::setCombineMode( osg::TexEnv::Mode value ) {
	textures[ textures.size() - 1 ].combineMode = value;
}

void material::setNoTexAlpha( bool value ) {
	textures[ textures.size() - 1 ].noAlpha = value;
}

void material::setNoTexColor( bool value) {
	textures[ textures.size() - 1 ].noColor = value;
}

void material::setSphereMap( bool value ) {
	textures[ textures.size() - 1 ].sphereMap = value;
}

void material::setName( const string& _name ) {
	if (_name != getName()){
		if ( Model::renameMaterial( getName(), _name ) ) {
			name = _name;
		}
	}
}

void material::reset() {
	dynCol = NULL;
	_ambient = RGBA();
	_diffuse = RGBA();
	_specular = RGBA();
	_emission = RGBA();
	_shininess = 0.0f;
	hasAmbient = false;
	hasDiffuse = false;
	hasSpecular = false;
	hasEmission = false;
	hasShininess = false;
	
	alphaThreshold = 0.0f;

	occluder = false;
	groupAlpha = false;
	noRadar    = false;
	noShadow   = false;
	noCulling  = false;
	noSorting  = false;
	noLighting = false;

	textures.clear();

	shaders.clear();
}

material& material::operator=(material const &rhs) {
	shaders.clear();
	for (vector<string>::const_iterator itr = rhs.shaders.begin(); itr != rhs.shaders.end(); itr++) {
		shaders.push_back( *itr );
	}

	materials.clear();
	for (list<material*>::const_iterator itr = rhs.materials.begin(); itr != rhs.materials.end(); itr++) {
		materials.push_back( *itr );
	}

	textures.clear();
	for (vector<TextureInfo>::const_iterator itr = rhs.textures.begin(); itr != rhs.textures.end(); itr++) {
		textures.push_back( *itr );
	}

	name = rhs.name;
	_ambient = rhs._ambient;
	_diffuse = rhs._diffuse;
	_specular = rhs._specular;
	_emission = rhs._emission;
	_shininess = rhs._shininess;
	hasAmbient = rhs.hasAmbient;
	hasDiffuse = rhs.hasDiffuse;
	hasSpecular = rhs.hasSpecular;
	hasEmission = rhs.hasEmission;
	hasShininess = rhs.hasShininess;
	dynCol = rhs.dynCol;
	noTextures = rhs.noTextures;
	noShadow = rhs.noShadow;
	noCulling = rhs.noCulling;
	noSorting = rhs.noSorting;
	noRadar = rhs.noRadar;
	noLighting = rhs.noLighting;
	noShaders = rhs.noShaders;
	groupAlpha = rhs.groupAlpha;
	occluder = rhs.occluder;
	alphaThreshold = rhs.alphaThreshold;

	computeFinalMaterial();
	computeFinalTexture();

	return *this;
};

// compute the final material from a list of materials
material* material::computeFinalMaterial( vector< material* >& materialList ) {
	osg::Vec4 ambient = osg::Vec4( 0, 0, 0, 0),
			  diffuse = osg::Vec4( 0, 0, 0, 0),
			  specular = osg::Vec4( 0, 0, 0, 0),
			  emission = osg::Vec4( 0, 0, 0, 0);
	float shiny = 0.0;
	float alphaThreshold = 1.0;
	bool nocull = false;
	string tex;
	bool foundTexture = false;
	bool notex = false;
	if( materialList.size() > 0 ) {
		for( vector< material* >::iterator i = materialList.begin(); i != materialList.end(); i++ ) {

			(*i)->computeFinalMaterial();
			if((*i)->getNoTextures())
				notex = true;
			string texName = (*i)->computeFinalTexture();
			if(!foundTexture && texName.compare("") != 0){
				tex = texName;
				foundTexture = true;
			}

			// get OSG's material from the material class
			osg::Material* mat = dynamic_cast< osg::Material* >(((*i)->getAttribute( osg::StateAttribute::MATERIAL ) ));

			// if it's valid, get the colors
			// NOTE: BZFlag pays attention only to the LAST occurence of a color
			if( mat ) {
				if(IS_VALID_COLOR( mat->getAmbient( osg::Material::FRONT ) ) ) {
					ambient = mat->getAmbient( osg::Material::FRONT );
				}

				if(IS_VALID_COLOR( mat->getDiffuse( osg::Material::FRONT ) ) ) {
					diffuse = mat->getDiffuse( osg::Material::FRONT );
				}

				if(IS_VALID_COLOR( mat->getSpecular( osg::Material::FRONT ) ) ) {
					specular = mat->getSpecular( osg::Material::FRONT );
				}

				if(IS_VALID_COLOR( mat->getEmission( osg::Material::FRONT ) ) ) {
					emission = mat->getEmission( osg::Material::FRONT );
				}
				if((*i)->getHasShininess())
					shiny = mat->getShininess( osg::Material::FRONT );
			}
			if((*i)->getHasAlphaThreshold())
				alphaThreshold = (*i)->getAlphaThreshold();
			if( (*i)->getNoCulling() )
				nocull = true;
		}
	}

	// build the material
	material* mat = new material();
	mat->setNoTextures(notex);

	osg::Material* finalMaterial = new osg::Material();
	
	//bzflag uses the diffuse color for the ambient color
	finalMaterial->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
	
	finalMaterial->setAmbient( osg::Material::FRONT, ambient );
	finalMaterial->setDiffuse( osg::Material::FRONT, diffuse );
	finalMaterial->setSpecular( osg::Material::FRONT, specular );
	finalMaterial->setEmission( osg::Material::FRONT, emission );
	finalMaterial->setShininess( osg::Material::FRONT, shiny );
	
	if( ambient.w() > alphaThreshold 
	   || diffuse.w() > alphaThreshold 
	   || specular.w() > alphaThreshold
	   || emission.w() > alphaThreshold )
		finalMaterial->setAlpha( osg::Material::FRONT, alphaThreshold );
	
	mat->setAttribute( finalMaterial, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
	
	if(!nocull)
	 mat->setMode(GL_CULL_FACE, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
	
	if( foundTexture ) {
		mat->setTexture( tex );
	}
	return mat;
}

// compute the final osg material
void material::computeFinalMaterial() {
	osg::Vec4 ambient = osg::Vec4( 1.0, 1.0, 1.0, 1.0),
		diffuse = osg::Vec4( 1.0, 1.0, 1.0, 1.0),
		specular = osg::Vec4( 0, 0, 0, 0),
		emission = osg::Vec4( 0, 0, 0, 0);
	float shiny = 0.0;
	float alphaThreshold = 1.0;
	bool nocull = false;
	
	// apply reference material color's if any
	if( materials.size() > 0 ) {
		for( list< material* >::iterator i = materials.begin(); i != materials.end(); i++ ) {
			material* mat = *i;
			if( mat ) {
				(*i)->computeFinalMaterial();
				if( mat->getHasAmbient() )
					ambient = mat->getAmbient();
				if( mat->getHasDiffuse() ) 
					diffuse = mat->getDiffuse();
				if( mat->getHasSpecular() )
					specular = mat->getSpecular();
				if( mat->getHasEmission() )
					emission = mat->getEmission();
				if( mat->getHasEmission() )
					shiny = mat->getShininess();
				if( mat->getHasAlphaThreshold() )
					alphaThreshold = (*i)->getAlphaThreshold();
				if( mat->getNoCulling() )
					nocull = true;
			}
		}
	}
	// apply this material color's if any
	if(hasAmbient)
		ambient = getAmbient();
	if(hasDiffuse)
		diffuse = getDiffuse();
	if(hasSpecular)
		specular = getSpecular();
	if(hasEmission)
		emission = getEmission();
	if(hasShininess)
		shiny = getShininess();
	if(hasAlphaThreshold)
		alphaThreshold = getAlphaThreshold();
	
	osg::Material* finalMaterial = new osg::Material();
	
	//bzflag uses the diffuse color for the ambient color
	finalMaterial->setColorMode( osg::Material::AMBIENT_AND_DIFFUSE );
	
	finalMaterial->setAmbient( osg::Material::FRONT, ambient );
	finalMaterial->setDiffuse( osg::Material::FRONT, diffuse );
	finalMaterial->setSpecular( osg::Material::FRONT, specular );
	finalMaterial->setEmission( osg::Material::FRONT, emission );
	finalMaterial->setShininess( osg::Material::FRONT, shiny );
	
	if( ambient.w() > alphaThreshold 
	   || diffuse.w() > alphaThreshold 
	   || specular.w() > alphaThreshold
	   || emission.w() > alphaThreshold )
		finalMaterial->setAlpha( osg::Material::FRONT, alphaThreshold );
	
	setAttribute( finalMaterial, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
	
	if(!nocull)
	  setMode(GL_CULL_FACE, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
}

// compute the final texture
// simple: BZFlag only pays attention to the first texture declared
string material::computeFinalTexture() {
	osg::Texture2D* finalTexture = NULL;
	osg::Texture2D* tempTexture = NULL;
	string finalTexName;
	if( textures.size() > 0 && textures[ 0 ].name.compare("") != 0) {
		finalTexture = SceneBuilder::buildTexture2D( textures[ 0 ].name.c_str() );		
		if ( finalTexture ) {
			//printf("material %s using final Texture: %s\n", name.c_str(), textures[ 0 ].name.c_str());
			setTextureMode( 0, GL_TEXTURE_2D, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
			setTextureAttribute( 0, finalTexture );
			finalTexName = textures[ 0 ].name.c_str();
			return finalTexName;
		}
	}
	// no texture in this material so check reference materials for a texture
	// use last texture found
	if( materials.size() > 0 ) {
		string tempTexName;
		string tempMatName;
		for( list< material* >::iterator i = materials.begin(); i != materials.end(); i++ ) {
			material* mat = *i;
			if(mat){
				tempTexName = mat->computeFinalTexture();
				tempTexture = mat->getCurrentTexture();
				if ( tempTexture ) {
					tempMatName = mat->getName();
					finalTexture = tempTexture;
					finalTexName = tempTexName;
				}
			}
		}
		if( finalTexture ){
			//printf("ref material %s using final Texture: %s\n", tempMatName.c_str(),finalTexName.c_str());
			setTextureMode( 0, GL_TEXTURE_2D, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
			setTextureAttribute( 0, finalTexture );
			return finalTexName;
		}
	}
	// no texture found
	setTextureMode( 0, GL_TEXTURE_2D, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );
	return finalTexName;
}

// get the current material
osg::Material* material::getCurrentMaterial() {
	return (osg::Material*)getAttribute( osg::StateAttribute::MATERIAL );
}

// get the current texture
osg::Texture2D* material::getCurrentTexture() {
	return (osg::Texture2D*)getTextureAttribute( 0, osg::StateAttribute::TEXTURE );
}

vector< string > material::getMaterials() {
	vector< string > ret;
	list< material* >::const_iterator i;
	for ( i = materials.begin(); i != materials.end(); i++ ) {
		ret.push_back( (*i)->getName() );
	}

	return ret;
}

void material::removeMaterial( material* mat ) {
	list< material* >::iterator i;
	list< list< material* >::iterator > iters;
	for ( i = materials.begin(); i != materials.end(); i++ ) {
		if ( *i == mat ) {
			iters.push_back( i );
		}
	}

	for ( list< list< material* >::iterator >::iterator it = iters.begin(); it != iters.end(); it++ ) {
		materials.erase( *it );
	}
}

void material::setMaterials( vector< string > value ) {
	materials.clear();

	vector< string >::const_iterator i;
	for ( i = value.begin(); i != value.end(); i++ ) {
		material* mat = dynamic_cast< material* >( Model::command( MODEL_GET, "material", *i ) );

		if ( mat != NULL )
			materials.push_back( mat );
		else
			printf( "material::setMaterials(): Error! Could not find material %s\n", (*i).c_str() );
	}
}