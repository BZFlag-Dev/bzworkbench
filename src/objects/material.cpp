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
	dynCol = NULL;
	color = string("");
	textures = vector< TextureInfo >();
	noTextures = true;
	noRadar = noShadow = noCulling = noLighting = noSorting = groupAlpha = occluder = false;
	alphaThreshold = 1.0f;
	
	setMode(GL_BLEND, osg::StateAttribute::ON);
	setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

	// allocate a material
	osg::Material* finalMaterial = new osg::Material();
	setAttribute( finalMaterial, osg::StateAttribute::ON  | osg::StateAttribute::OVERRIDE );

	// deactivate texturing
	setTextureMode( 0, GL_TEXTURE_2D, osg::StateAttribute::OFF  | osg::StateAttribute::OVERRIDE );

	setAmbient( osg::Vec4( 1, 1, 1, 1) );
	setDiffuse( osg::Vec4( 1, 1, 1, 1) );
	setSpecular( osg::Vec4( 0, 0, 0, 1) );
	setEmissive( osg::Vec4( 1, 1, 1, 1) );
}

// getter
string material::get(void) { return toString(); }

// bzw methods
bool material::parse( std::string& line ) {
	// check if we reached the end of the section
	if ( line == "end" )
		return false;

	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );

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
	else if ( key == "emission" ) { // get the emissive colors
		setEmissive( RGBA( value.c_str() ) );
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

// tostring
string material::toString(void) {
	string texString = string("");
	if( textures.size() > 0) {
		for(vector< TextureInfo >::iterator i = textures.begin(); i != textures.end(); i++) {
			TextureInfo info = *i;
			if( info.name.compare("") != 0 ){
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
	string ambientString, diffuseString, specularString, emissiveString;

	RGBA ambientColor = RGBA( getAmbient().x(), getAmbient().y(), getAmbient().z(), getAmbient().w() );
	RGBA diffuseColor = RGBA( getDiffuse().x(), getDiffuse().y(), getDiffuse().z(), getDiffuse().w() );
	RGBA specularColor = RGBA( getSpecular().x(), getSpecular().y(), getSpecular().z(), getSpecular().w() );
	RGBA emissiveColor = RGBA( getEmissive().x(), getEmissive().y(), getEmissive().z(), getEmissive().w() );

	if( IS_VALID_COLOR( ambientColor ) )
		ambientString = "  ambient " + ambientColor.toString();
	else
		ambientString = "";

	if( IS_VALID_COLOR( diffuseColor ) )
		diffuseString = "  diffuse " + diffuseColor.toString();
	else
		diffuseString = "";

	if( IS_VALID_COLOR( specularColor ) )
		specularString = "  specular " + specularColor.toString();
	else
		specularString = "";

	if( IS_VALID_COLOR( emissiveColor ) )
		emissiveString = "  emission " + emissiveColor.toString();
	else
		emissiveString = "";

	return string("material\n") +
				  (name.length() == 0 ? string("# name") : "  name " + name) + "\n" +
				  (dynCol != NULL ? string("  dyncol ") + dynCol->getName() : string("")) + "\n" +
				  (noTextures == true ? "  notextures\n" : "") +
				  (noShadow == true ? "  noshadow\n" : "") +
				  (noCulling == true ? "  noculling\n" : "") +
				  (noSorting == true ? "  nosorting\n" : "") +
				  (noLighting == true ? "  nolighting\n" : "") +
				  (noRadar == true ? "  noradar\n" : "") +
				  (groupAlpha == true ? "  groupalpha\n" : "") +
				  (occluder == true ? "  occluder\n" : "") +
				  ambientString +
				  (color.length() == 0 ? diffuseString : string("  color ") + color + "\n" ) +
				  specularString +
				  emissiveString +
				  "  shininess " + string(ftoa(getShininess())) + "\n" +
				  "  alphathresh " + string(ftoa(alphaThreshold)) + "\n" +
				  texString +
				  matString +
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
	if ( Model::renameMaterial( getName(), _name ) ) {
		name = _name;
	}
}

void material::reset() {
	dynCol = NULL;

	setAmbient( osg::Vec4( 0.2f, 0.2f, 0.2f, 1.0f ) );
	setDiffuse( osg::Vec4( 1.0f, 1.0f, 1.0f, 1.0f ) );
	setSpecular( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
	setEmissive( osg::Vec4( 0.0f, 0.0f, 0.0f, 1.0f ) );
	setShininess( 0.0f );

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
	color = rhs.color;

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
			  emissive = osg::Vec4( 0, 0, 0, 0);
	float shiny = 0.0;
	float alphaThreshold = 1.0;
	string tex;
	bool foundTexture = false;

	if( materialList.size() > 0 ) {
		for( vector< material* >::iterator i = materialList.begin(); i != materialList.end(); i++ ) {

			(*i)->computeFinalMaterial();
			(*i)->computeFinalTexture();

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
					emissive = mat->getSpecular( osg::Material::FRONT );
				}

				shiny = mat->getShininess( osg::Material::FRONT );
			}
			
			alphaThreshold = (*i)->getAlphaThreshold();

			// get the texture
			if (!foundTexture && (*i)->getTextureCount() > 0) {
				tex = (*i)->getTexture(0);
				foundTexture = true;
			}
		}
	}

	// build the material
	material* mat = new material();

	osg::Material* finalMaterial = new osg::Material();

	finalMaterial->setAmbient( osg::Material::FRONT, ambient );
	finalMaterial->setDiffuse( osg::Material::FRONT, diffuse );
	finalMaterial->setSpecular( osg::Material::FRONT, specular );
	finalMaterial->setEmission( osg::Material::FRONT, emissive );
	finalMaterial->setShininess( osg::Material::FRONT, shiny );
	
	if( ambient.w() > alphaThreshold 
	   || diffuse.w() > alphaThreshold 
	   || specular.w() > alphaThreshold
	   || emissive.w() > alphaThreshold )
		finalMaterial->setAlpha( osg::Material::FRONT, alphaThreshold );
	
	mat->setAttribute( finalMaterial, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
	
	if( foundTexture ) {
		mat->setTexture( tex );
	}
	return mat;
}

// compute the final osg material
void material::computeFinalMaterial() {

	osg::Vec4 ambient = osg::Vec4( 0, 0, 0, 0),
		diffuse = osg::Vec4( 0, 0, 0, 0),
		specular = osg::Vec4( 0, 0, 0, 0),
		emissive = osg::Vec4( 0, 0, 0, 0);

	float shiny = 0.0;
	float alphaThreshold = 1.0;

	if( materials.size() > 0 ) {

		for( list< material* >::iterator i = materials.begin(); i != materials.end(); i++ ) {

			material* mat = *i;

			// NOTE: BZFlag pays attention only to the LAST occurence of a color
			if( mat ) {
				if(IS_VALID_COLOR( mat->getAmbient() ) ) {
					ambient = mat->getAmbient();
				}

				if(IS_VALID_COLOR( mat->getDiffuse() ) ) {
					diffuse = mat->getDiffuse();
				}

				if(IS_VALID_COLOR( mat->getSpecular() ) ) {
					specular = mat->getSpecular();
				}

				if(IS_VALID_COLOR( mat->getEmissive() ) ) {
					emissive = mat->getSpecular();
				}

				shiny = mat->getShininess();
			}
			
			alphaThreshold = (*i)->getAlphaThreshold();
			
		}

		osg::Material* finalMaterial = new osg::Material();

		finalMaterial->setAmbient( osg::Material::FRONT, ambient );
		finalMaterial->setDiffuse( osg::Material::FRONT, diffuse );
		finalMaterial->setSpecular( osg::Material::FRONT, specular );
		finalMaterial->setEmission( osg::Material::FRONT, emissive );
		finalMaterial->setShininess( osg::Material::FRONT, shiny );
		
		if( ambient.w() > alphaThreshold 
			|| diffuse.w() > alphaThreshold 
			|| specular.w() > alphaThreshold
			|| emissive.w() > alphaThreshold )
				finalMaterial->setAlpha( osg::Material::FRONT, alphaThreshold );

		setAttribute( finalMaterial, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
	}
}

// compute the final texture
// simple: BZFlag only pays attention to the first texture declared
void material::computeFinalTexture() {
	if( textures.size() > 0 && textures[ 0 ].name.compare("") != 0) {
		osg::Texture2D* finalTexture = SceneBuilder::buildTexture2D( textures[ 0 ].name.c_str() );

		if ( finalTexture ) {
			setTextureMode( 0, GL_TEXTURE_2D, osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE );
			setTextureAttribute( 0, finalTexture );
			return;
		}
	}

	setTextureMode( 0, GL_TEXTURE_2D, osg::StateAttribute::OFF | osg::StateAttribute::OVERRIDE );
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