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

#ifndef MATERIAL_H_
#define MATERIAL_H_

#include "DataEntry.h"
#include "render/RGBA.h"
#include "model/BZWParser.h"
#include "model/Model.h"

#include <string.h>
#include <vector>
#include <list>

#include <osg/Material>
#include <osg/Texture2D>
#include <osg/StateSet>
#include <osg/TexEnv>

class dynamicColor;
class texturematrix;

#define IS_VALID_COLOR( c ) (c.x() >= 0.0 && c.y() >= 0.0 && c.z() >= 0.0 && c.w() >= 0.0)

class material : public DataEntry, public osg::StateSet {

	friend class SceneBuilder;		// allow SceneBuilder to access protected/private methods

public:
	// default constructor
	material();

	static DataEntry* init() { return new material(); }
	
	enum {
		MAT_REF = 0,
		MAT_AMBIENT,
		MAT_DIFFUSE,
		MAT_SPECULAR,
		MAT_EMISSION,
		MAT_TEXTURE
	};

	// getter
	string get(void);

	// bzw methods
	bool parse( std::string& line );
	void finalize();

	// tostring
	string toStringClean();
	string toString(void);

	// binary getters and setters
	string getName() { return name; }
	dynamicColor* getDynamicColor() { return dynCol; }
	
	bool getHasAmbient() { return hasAmbient; }
	bool getHasDiffuse() { return hasDiffuse; }
	bool getHasSpecular() { return hasSpecular; }
	bool getHasEmission() { return hasEmission; }
	bool getHasShininess() { return hasShininess; }
	bool getHasAlphaThreshold() { return hasAlphaThreshold; }
	
	const osg::Vec4& getAmbient() { return _ambient; }
	const osg::Vec4& getDiffuse() { return _diffuse; }
	const osg::Vec4& getSpecular() { return _specular; }
	const osg::Vec4& getEmission() { return _emission; }
	float getShininess() { return _shininess; }
	float getAlphaThreshold() { return alphaThreshold; }

	bool getNoTextures() { return noTextures; }
	bool getNoShadows() { return noShadow; }
	bool getNoCulling() { return noCulling; }
	bool getNoSorting() { return noSorting; }
	bool getNoRadar() { return noRadar; }
	bool getNoLighting() { return noLighting; }
	bool getNoShaders() { return noShaders; }
	bool getGroupAlpha() { return groupAlpha; }
	bool getOccluder() { return occluder; }

	std::vector< std::string > getMaterials();
	void removeMaterial( material* mat );
	void setMaterials( std::vector< std::string > value );

	void setName( const string& _name );
	void setDynamicColor( dynamicColor* _dynCol ) { dynCol = _dynCol; }

	void setHasAmbient( bool value ) { hasAmbient = value; }
	void setHasDiffuse( bool value ) { hasDiffuse = value; }
	void setHasSpecular( bool value ) { hasSpecular = value; }
	void setHasEmission( bool value ) { hasEmission = value; }
	void setHasShininess( bool value ) { hasShininess = value; }
	void setHasAlphaThreshold( bool value ) { hasAlphaThreshold = value; }
	
	void setAmbient( const osg::Vec4& value ) { _ambient = value; hasAmbient = true; }
	void setDiffuse( const osg::Vec4& value ) { _diffuse = value; hasDiffuse = true; }
	void setSpecular( const osg::Vec4& value ) { _specular = value; hasSpecular = true; }
	void setEmission( const osg::Vec4& value ) { _emission = value; hasEmission = true; }
	void setShininess( float value ) { _shininess = value; hasShininess = true; }
	void setAlphaThreshold( float value ) { alphaThreshold = value; hasAlphaThreshold = true;}

	void setNoTextures( bool value ) { noTextures = value; }
	void setNoShadows( bool value ) { noShadow = value; }
	void setNoCulling( bool value ) { noCulling = value; }
	void setNoSorting( bool value ) { noSorting = value; }
	void setNoRadar( bool value ) { noRadar = value; }
	void setNoLighting( bool value ) { noLighting = value; }
	void setNoShaders( bool value ) { noShaders = value; }
	void setGroupAlpha( bool value ) { groupAlpha = value; }
	void setOccluder( bool value ) { occluder = value; }

	// the following set()'s operate on the last added texture
	void addTexture(const std::string&);
    void setTexture(const std::string&);
    void setTextureMatrix( texturematrix* texmat );
	void setCombineMode( osg::TexEnv::Mode value );
    void setNoTexAlpha( bool value );
    void setNoTexColor( bool value);
    void setSphereMap( bool value );
	void clearTextures() { textures.clear(); } // remove all textures

	int getTextureCount() { return textures.size(); }
    const std::string& getTexture( int num ) { return textures[num].name; }
    texturematrix* getTextureMatrix( int num ) { return textures[num].matrix; }
	osg::TexEnv::Mode getCombineMode( int num ) { return textures[num].combineMode; }
    bool getNoTexAlpha( int num ) { return textures[num].noAlpha; }
    bool getNoTexColor( int num ) { return textures[num].noColor; }
	bool getUseSphereMap( int num ) { return textures[num].sphereMap; }

	void reset();

	// use this to compute the osg stateset to apply
	// this entails merging parts of other materials
	static material* computeFinalMaterial( vector< material* >& materialList );

	// get the current material
	osg::Material* getCurrentMaterial();

	// get the current texture
	osg::Texture2D* getCurrentTexture();

	material& operator=(material const &rhs);
	
	int getMatType() {return type;}
	void setMatType(int value) { type = value; }

private:
	std::string name;
	int type;
	std::vector< std::string > shaders;
	std::list< material* > materials;
	dynamicColor* dynCol;
	bool noTextures;
	bool noShadow;
	bool noCulling;
	bool noSorting;
	bool noRadar;
	bool noLighting;
	bool noShaders;
	bool groupAlpha;
	bool occluder;
	bool hasAmbient;
	bool hasDiffuse;
	bool hasSpecular;
	bool hasEmission;
	bool hasShininess;
	bool hasAlphaThreshold;
	osg::Vec4 _ambient;
	osg::Vec4 _diffuse;
	osg::Vec4 _specular;
	osg::Vec4 _emission;
	float _shininess;
	float alphaThreshold;

	struct TextureInfo {
      std::string name;
      texturematrix* matrix;
	  osg::TexEnv::Mode combineMode;
      bool noAlpha;
      bool noColor;
      bool sphereMap;
    };
	std::vector< TextureInfo > textures;	// the various textures from "texture" and "addTexture"

	// compute the final OSG texture
	string computeFinalTexture();

	// compute the final OSG material
	void computeFinalMaterial();

	// build the alias map
	void buildAliases();
};

#endif /*MATERIAL_H_*/
