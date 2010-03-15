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

#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>

#ifdef _WIN32
	#include <direct.h>
#else
	#include <sys/stat.h>
#endif
#include <iostream>;

#include "model/SceneBuilder.h"
#include "windows/View.h"
#include "model/Primitives.h"


int SceneBuilder::nameCount;

std::map< std::string, osg::ref_ptr< osg::StateSet > > SceneBuilder::stateCache;

// constructor
bool SceneBuilder::init() {
	nameCount = 0;
	return true;
}

// destructor
bool SceneBuilder::shutdown() {

	return true;
}

/**
 * Object builder.
 * This method builds and returns a node loaded from nodeFile
 */
osg::Node* SceneBuilder::buildNode( const char* nodeFile ) {
	if(!nodeFile)
		return NULL;

	// string-ify the nodeFile and add the corresponding tail
	string nodeName = SceneBuilder::nameNode( nodeFile );

	// load it in
	osg::Node* node = osgDB::readNodeFile( nodeFile );

	if(node == NULL)
		return NULL;

	// each node stores nodeName (i.e. the string it's mapped to) as its name
	node->setName( nodeName );

	// make a TextureVisitor to enable GL_REPEAT in the node's textures
	TextureRepeaterVisitor tv = TextureRepeaterVisitor();

	// do a traversal to apply the modification
	node->accept( tv );

	// make a TextureScalerVisitor
	//Renderable* r = new Renderable( node );
	//TextureScalerVisitor tsv = TextureScalerVisitor( r, osg::Vec3( 7.0, 3.5, 1.0 ) );

	//node->accept( tsv );

	// turn on blending
	osg::StateSet* stateSet = node->getOrCreateStateSet();
	stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);


	return node;
}

/**
 * Load a node and mark it selected
 */

osg::Node* SceneBuilder::buildSelectedNode( const char* fileName ) {
	string selectedNodeName = SceneBuilder::nameSelected( SceneBuilder::nameNode( fileName ).c_str() );

	osg::Node* node = osgDB::readNodeFile( fileName );
	if( node == NULL )
		return NULL;

	SceneBuilder::markSelected( node );

	node->setName( selectedNodeName );

	return node;
}

/**
 * Geometry builder
 * This method builds and returns a geometry node (a geode) from arrays of vertexes, indexes, texture coordinates, and
 * a texture filename
 */
osg::Geode* SceneBuilder::buildGeode( const char* _nodeName, osg::Geometry* geometry, const char* textureFile, unsigned int textureMode ) {
	// make the node name
	string nodeName = string(_nodeName) + SCENEBUILDER_TAIL_NODE;

	// make the geometry name
	string geometryName = string(_nodeName) + SCENEBUILDER_TAIL_GEOMETRY;

	// make a geode
	osg::Geode* geode = new osg::Geode();

	// load up the texture
    SceneBuilder::assignTexture( textureFile, geode, textureMode );

	geode->addDrawable( geometry );

	return geode;
}

// used by curl function in SceneBuilder::buildTexture2D
size_t write_curl_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written;
    written = fwrite(ptr, size, nmemb, stream);
    return written;
}



/**
 * Build a Texture2D from a file; return NULL if not found
 */	

// FIXME - buildTexture2D function is called too much and waste memory
// should store a list of textures an reference those 
// load each textures only once

osg::Texture2D* SceneBuilder::buildTexture2D( const char* filename ) {
	string searchPath("share/textures/");
	string temp_name( filename );	

	//---- download and cache remote texture
	// see if filename starts with "http://"
	string protocol("http://");
	if( temp_name.compare(0, protocol.length(), TextUtils::tolower(protocol), 0, protocol.length()) == 0 ){
		// swap "http://" for "bzwb_cache/" in filename to fix the path
		temp_name = "bzwb_cache/" + temp_name.substr(protocol.length(),temp_name.length()-protocol.length());
		//strip off .png from end if it exists since it is added below
		string last4 = TextUtils::tolower(temp_name.substr(temp_name.length()-4,4));
		if( last4.compare(".png") == 0 ){
			temp_name = temp_name.substr(0,temp_name.length()-4);
		}
		// check for texture in bzwb_cache
		// FIXME: should compare modification dates of the file in cache and the one online, not just if it exists locally 
		FILE * cFile;
		cFile = fopen ( ( searchPath + temp_name + ".png" ).c_str(), "r");
		if( cFile!=NULL ) {
			// file exists in http_cache, then close it cause we don't need it.
			printf ( "Using Cached Texture: %s\n", ( searchPath + temp_name + ".png" ).c_str() );
			fclose ( cFile );
		}else{
			// file does not exist in http_cache:
			// get texture's parent directory path and create directories
			string pDirPath = string("");
			std::vector<std::string> tks = TextUtils::tokenize( string( searchPath + temp_name ), "/", 20, false);
			for ( std::vector<std::string>::iterator i = tks.begin() ; i < tks.end()-1 ; i++ ){
				if(i < tks.end()-2)
					pDirPath += (*i) + "/";
				else
					pDirPath += (*i) ;
				// create directories as we go
				#ifdef _WIN32
					_mkdir( pDirPath.c_str() );
				#else
					mkdir( pDirPath.c_str(), 0777 );
				#endif
			}
			//printf(  "parent directory path: %s\n", pDirPath.c_str() );
			
			// download the texture into http_cache
			printf ( "Downloading Texture: %s\n", ( searchPath + temp_name + ".png" ).c_str() );
			CURL *curl;
			FILE *fp;
			CURLcode res;
			long len = 0;
			curl = curl_easy_init();
			if( curl ) {
				fp = fopen( ( searchPath + temp_name + ".png" ).c_str(), "wb" );
				if( fp != NULL ){
					curl_easy_setopt( curl, CURLOPT_URL, filename );
					curl_easy_setopt( curl, CURLOPT_WRITEFUNCTION, write_curl_data );
					curl_easy_setopt( curl, CURLOPT_WRITEDATA, fp );
					res = curl_easy_perform( curl );
					curl_easy_cleanup( curl );
					len = ftell(fp);
					fclose( fp );
					if(res != 0){
						printf ( "Error Downloading: %s, %l\n", curl_easy_strerror(res), len );
						//remove cached file
						std::remove( ( searchPath + temp_name + ".png" ).c_str() );
						//use error downloading texture
						temp_name = "../UI/error_downloading";
					}
				}else{
					printf( "Unable to open/create texture file: %s\n", ( searchPath + temp_name + ".png" ).c_str() );
				}
			}
		}		
	}
	//---- end download and cache remote texture
	
	//strip .png extension - this is done again here for local textures entered with a .png extension
	string last4 = temp_name.substr(temp_name.length()-4,4);
	if( last4.compare(".png") == 0 ){
		temp_name = temp_name.substr(0,temp_name.length()-4);
	}

	if( temp_name.c_str() != NULL ) {
		osg::Texture2D* texture = NULL;
		
		//see if texture file exists and use a default "missing texture" if it doesn't
		FILE * cFile;
		cFile = fopen ( (searchPath + temp_name + ".png").c_str() ,"r");
		if (cFile!=NULL) {
			// file exists in http_cache, then close it cause we don't need it.
			fclose (cFile);
		}else{
			printf("Can not find texture: %s\n", (searchPath + temp_name + ".png").c_str() );
			temp_name = "../UI/not_found";
		}

		osg::Image* image = osgDB::readImageFile( (searchPath + temp_name + ".png").c_str() );

		if( image != NULL ) {	// only build the texture if the image exists!

			texture = new osg::Texture2D();

			// don't allow OSG to optimize the texture (otherwise it may disappear)
			texture->setDataVariance( osg::Object::DYNAMIC );

			// set the texture's image
			texture->setImage( image );

			// turn on GL_REPEAT texture wrapping
			texture->setWrap( osg::Texture::WRAP_R, osg::Texture::REPEAT );
			texture->setWrap( osg::Texture::WRAP_S, osg::Texture::REPEAT );
			texture->setWrap( osg::Texture::WRAP_T, osg::Texture::REPEAT );
		}

		return texture;
	}
	return NULL;
}

// assign a texture to a Node
void SceneBuilder::assignTexture( const char* _textureName, osg::Node* node, unsigned int mode ) {
	if(_textureName != NULL) {
		// if the stateset is already in the cache, assign it
		if ( stateCache.count( string( _textureName ) ) > 0 ) {
			node->setStateSet( stateCache[ _textureName ].get() );
		}
		// otherwise load the texture and make a stateset
		else {
			osg::Texture2D* texture = buildTexture2D( _textureName );
			
			if (texture != NULL) {
				// make a new state set for the texture (so we can manipulate the texture attributes)
				osg::StateSet* texStateSet = new osg::StateSet();

				// assign the texture to the state set and activate it
				texStateSet->setTextureAttributeAndModes( 0, texture, mode );

				// finally, attach the texture to the geode
				node->setStateSet( texStateSet );

				// save it to the cache
				stateCache[ _textureName ] = texStateSet;
			}
		}
	}
}

// assign a material to a node
void SceneBuilder::assignMaterial( osg::Vec4 ambient, osg::Vec4 diffuse, osg::Vec4 specular, osg::Vec4 emissive, float shininess, float alphaThreshold, osg::Node* node, osg::StateAttribute::Values val ) {
	osg::Material* mat = new osg::Material();

	// set ambient lighting
	mat->setAmbient( osg::Material::FRONT, ambient );

	// set diffuse color
	mat->setDiffuse( osg::Material::FRONT, diffuse );

	// set specular
	mat->setSpecular( osg::Material::FRONT, specular );

	// set emissive
	mat->setEmission( osg::Material::FRONT, emissive );

	// set shininess
	mat->setShininess( osg::Material::FRONT, shininess );

	// using setAlpha or setTransparency 
	// overrides the alpha values for ambient, diffuse, specular, emissive
	// only apply if alphas are higher then threshold
	// set transparency
	if( ambient.w() > alphaThreshold 
		|| diffuse.w() > alphaThreshold 
		|| specular.w() > alphaThreshold
		|| emissive.w() > alphaThreshold )
			mat->setAlpha( osg::Material::FRONT, alphaThreshold );

	// get the state set from the node
	osg::StateSet* stateSet = node->getOrCreateStateSet();
	
	stateSet->setMode(GL_BLEND, osg::StateAttribute::ON );
	stateSet->setRenderingHint( osg::StateSet::TRANSPARENT_BIN );

	// assign the material
	stateSet->setAttribute( mat, val);

	// assign the new state set
	node->setStateSet( stateSet );
}

// assign a BZWB material to a bz2object
void SceneBuilder::assignBZMaterial( osg::StateSet* bzmat, osg::Node* obj ) {
	if(bzmat != NULL && obj != NULL){
		obj->setStateSet( bzmat );
	}
}

// get the material from a node
osg::Material* extractMaterial( Renderable* r ) {
	osg::StateSet* stateSet = r->getOrCreateStateSet();

	return (osg::Material*)(stateSet->getAttribute( osg::StateAttribute::MATERIAL ) );
}

// build a geometry and call the other buildGeode method
osg::Geode* SceneBuilder::buildGeode( const char* nodeName, osg::Vec3Array* vertexes, osg::DrawElementsUInt* indexes, osg::Vec2Array* texCoords, const char* textureName, unsigned int textureMode ) {
	// don't bother if the data is NULL
	if(vertexes == NULL || indexes == NULL)
		return NULL;

	// make a new geometry
	osg::Geometry* geometry = new osg::Geometry();

	// assign the vertexes
	geometry->setVertexArray( vertexes );

	// assign the indexes
	geometry->addPrimitiveSet( indexes );

	if( texCoords != NULL)
		// assign the texture coordinates
		geometry->setTexCoordArray( 0, texCoords );

	return SceneBuilder::buildGeode( nodeName, geometry, textureName, textureMode );
}

/**
 * Gets the geometry data from a node (returns NULL if it has no geometry).
 * It does NOT look at children nodes.
 */
const vector< osg::ref_ptr< osg::Drawable > >* getNodeGeometry( osg::PositionAttitudeTransform* node ) {
	const osg::Group* group = dynamic_cast< osg::Group* > (node);
	if(!group) {		// return NULL if the dynamic_cast failed
		return NULL;
	}

	const osg::Geode* geode = dynamic_cast< osg::Geode* > (node);
	if(!geode) {
		return NULL;
	}

	return &geode->getDrawableList();
}

/**
 * Mark an OSG node as selected (i.e. color it green)
 */

void SceneBuilder::markSelected( osg::Node* theNode ) {
	SceneBuilder::assignMaterial(  osg::Vec4( 0.0, 1.0, 0.0, 1.0 ),
								   osg::Vec4( 1.0, 1.0, 1.0, 1.0 ),
								   osg::Vec4( 0.0, 0.0, 0.0, 1.0 ),
								   osg::Vec4( 0.0, 1.0, 0.0, 1.0 ),
								   0.0,
								   1.0,
								   theNode,
								   osg::StateAttribute::OVERRIDE );

}

// mark a node as unselected
void SceneBuilder::markUnselected( osg::Node* theNode ) {
	SceneBuilder::assignMaterial(  osg::Vec4( 1.0, 1.0, 1.0, 1.0 ),
								   osg::Vec4( 1.0, 1.0, 1.0, 1.0 ),
								   osg::Vec4( 0.0, 0.0, 0.0, 1.0 ),
								   osg::Vec4( 1.0, 1.0, 1.0, 1.0 ),
								   0.0,
								   1.0,
								   theNode,
								   osg::StateAttribute::OVERRIDE );
}

// mark the object selected without destroying its state set
void SceneBuilder::markSelectedAndPreserveStateSet( bz2object* theNode ) {

	osg::StateSet* currStateSet = theNode->getOrCreateStateSet();
	osg::Material* currMaterial = (osg::Material*)currStateSet->getAttribute( osg::StateAttribute::MATERIAL );
	if( currStateSet != NULL ) {
		theNode->savedStateSet = new osg::StateSet( *currStateSet );
		printf(" saving states\n");

		if( currMaterial != NULL ) {
			printf("    ambient:  %f %f %f %f\n", currMaterial->getAmbient( osg::Material::FRONT ).x(), currMaterial->getAmbient( osg::Material::FRONT ).y(), currMaterial->getAmbient( osg::Material::FRONT ).z(), currMaterial->getAmbient( osg::Material::FRONT ).w() );
			printf("    diffuse:  %f %f %f %f\n", currMaterial->getDiffuse( osg::Material::FRONT ).x(), currMaterial->getDiffuse( osg::Material::FRONT ).y(), currMaterial->getDiffuse( osg::Material::FRONT ).z(), currMaterial->getDiffuse( osg::Material::FRONT ).w() );
			printf("    specular: %f %f %f %f\n", currMaterial->getSpecular( osg::Material::FRONT ).x(), currMaterial->getSpecular( osg::Material::FRONT ).y(), currMaterial->getSpecular( osg::Material::FRONT ).z(), currMaterial->getSpecular( osg::Material::FRONT ).w() );
			printf("    emissive: %f %f %f %f\n", currMaterial->getEmission( osg::Material::FRONT ).x(), currMaterial->getEmission( osg::Material::FRONT ).y(), currMaterial->getEmission( osg::Material::FRONT ).z(), currMaterial->getEmission( osg::Material::FRONT ).w() );
		}
	}

	SceneBuilder::assignMaterial(  osg::Vec4( 0.0, 1.0, 0.0, 1.0 ),
								   osg::Vec4( 1.0, 1.0, 1.0, 1.0 ),
								   osg::Vec4( 0.0, 0.0, 0.0, 1.0 ),
								   osg::Vec4( 0.0, 1.0, 0.0, 1.0 ),
								   0.0,
								   0.5,
								   theNode,
								   osg::StateAttribute::OVERRIDE );

}

// mark a node as unselected by restoring it's state set if applicable
void SceneBuilder::markUnselectedAndRestoreStateSet( bz2object* theNode ) {

	osg::StateSet* stateSet = theNode->savedStateSet.get();
	osg::Material* mat = (osg::Material*)stateSet->getAttribute( osg::StateAttribute::MATERIAL );
	if( stateSet != NULL ) {
		printf(" restoring states\n");

		if( mat != NULL ) {
			printf("    ambient:  %f %f %f %f\n", mat->getAmbient( osg::Material::FRONT ).x(), mat->getAmbient( osg::Material::FRONT ).y(), mat->getAmbient( osg::Material::FRONT ).z(), mat->getAmbient( osg::Material::FRONT ).w() );
			printf("    diffuse:  %f %f %f %f\n", mat->getDiffuse( osg::Material::FRONT ).x(), mat->getDiffuse( osg::Material::FRONT ).y(), mat->getDiffuse( osg::Material::FRONT ).z(), mat->getDiffuse( osg::Material::FRONT ).w() );
			printf("    specular: %f %f %f %f\n", mat->getSpecular( osg::Material::FRONT ).x(), mat->getSpecular( osg::Material::FRONT ).y(), mat->getSpecular( osg::Material::FRONT ).z(), mat->getSpecular( osg::Material::FRONT ).w() );
			printf("    emissive: %f %f %f %f\n", mat->getEmission( osg::Material::FRONT ).x(), mat->getEmission( osg::Material::FRONT ).y(), mat->getEmission( osg::Material::FRONT ).z(), mat->getEmission( osg::Material::FRONT ).w() );
		}

		theNode->setStateSet( stateSet );
	}

	else
		SceneBuilder::markUnselected( theNode );
}

bz2object* SceneBuilder::cloneBZObject( bz2object* obj ) {
	std::string str = obj->toString();	

	vector< string > lines = BZWParser::getLines( "", str.c_str() );

	bz2object* newObj = dynamic_cast< bz2object* >( Model::buildObject( obj->getHeader().c_str() ) );

	if ( newObj != NULL ) {
		for ( vector< string >::iterator i = lines.begin(); i != lines.end(); i++ ) {
			std::string line = BZWParser::cutWhiteSpace( *i );
			
			if ( line == "" )
				continue;

			newObj->parse( line );
		}

		newObj->finalize();
	}

	return newObj;
}
