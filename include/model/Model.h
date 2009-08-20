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

#ifndef MODEL_H_
#define MODEL_H_

#include <string>
#include <vector>
#include <map>

// forward declarations of some objects
class bz2object;
class world;
class waterLevel;
class info;
class options;
class texturematrix;
class dynamicColor;
class physics;
class Tlink;
class define;
class DataEntry;
class material;
class teleporter;
class group;

// supported query commands.
#define MODEL_GET "get"

#include "Observable.h"
#include "ObserverMessage.h"

#include "dialogs/ConfigurationDialog.h"

#include <osg/ref_ptr>
#include <osg/Vec3>


// thrown when there is an error reading a bzw file
struct BZWReadError {
	BZWReadError( DataEntry* obj, std::string message ) :
		bzobject( obj ),
		message( message )
	{ }

	DataEntry* bzobject;
	std::string message;
};

class Model : public Observable
{
public:

	typedef std::vector< osg::ref_ptr< bz2object > > objRefList;

	Model();
	Model(const char* supportedObjects, const char* objectHierarchy, const char* terminators);

	virtual ~Model();

	// string-based control mechanism
	// command syntax: "<command> <object> <name>[ data]", where
	// command is the query command, object is the type of object, the name is the object's name, and data is any additional data the command would need
	static DataEntry* command(const std::string& command, const std::string& object, const std::string& name, const std::string& data = "" );

	// the real query method
	DataEntry* _command(const std::string& command, const std::string& object, const std::string& name, const std::string& data = "" );

	// build the model from a stream of bzw data
	static bool build( std::istream& data );

	// the real build method
	// returns false if it fails
	bool _build( std::istream& data );

	// universal getter
	static std::string& toString(void);

	// the "real" universal getter
	std::string& _toString(void);

	// BZWB-specific API for built-in objects
	static world* getWorldData();
	static waterLevel* getWaterLevelData();
	static options* getOptionsData();
	static info* getInfoData();
	static objRefList& getObjects();
	static std::map< std::string, osg::ref_ptr< material > >& getMaterials();
	static std::map< std::string, texturematrix* >& getTextureMatrices();
	static std::map< std::string, dynamicColor* >& getDynamicColors();
	static std::map< std::string, osg::ref_ptr< physics > >& getPhysicsDrivers();
	static std::map< std::string, osg::ref_ptr< Tlink > >& getTeleporterLinks();
	static std::map< std::string, define* >& getGroups();

	static void addObject( bz2object* obj );
	static DataEntry* buildObject( const char* header );
	static void removeObject( bz2object* obj );
	static void setSelected( bz2object* obj );
	static void setUnselected( bz2object* obj );
	static void selectAll();
	static void unselectAll();
	static bool isSelected( bz2object* obj );
	static objRefList& getSelection();
	static void assignMaterial( const std::string& matref, bz2object* obj );
	static void assignMaterial( material* matref, bz2object* obj );
	static ConfigurationDialog* configureObject( DataEntry* obj );

	static bool renameMaterial( std::string oldName, std::string newName );
	static bool renameDynamicColor( std::string oldName, std::string newName );
	static bool renameTextureMatrix( std::string oldName, std::string newName );
	static bool renamePhysicsDriver( std::string oldName, std::string newName );
	static bool renameTeleporterLink( std::string oldName, std::string newName );
	static bool renameGroup( std::string oldName, std::string newName );

	// editor-like methods (BZWB-specific)
	static bool cutSelection();
	static bool copySelection();
	static bool pasteSelection();
	static bool deleteSelection();
	static bool newWorld();
	static bool linkTeleporters( teleporter* t1, teleporter* t2 );
	static void groupObjects( Model::objRefList& objects );
	static void ungroupObjects( group* g );

	// instantiated BZWB-specific API
	world* _getWorldData() { return worldData; }
	options* _getOptionsData() { return optionsData; }
	waterLevel* _getWaterLevelData() { return waterLevelData; }
	info* _getInfoData() { return infoData; }
	objRefList& _getObjects() { return this->objects; }
	std::map< std::string, osg::ref_ptr< material > >& _getMaterials() { return this->materials; }
	std::map< std::string, texturematrix* >& _getTextureMatrices() { return this->textureMatrices; }
	std::map< std::string, dynamicColor* >& _getDynamicColors() { return this->dynamicColors; }
	std::map< std::string, osg::ref_ptr< physics > >& _getPhysicsDrivers() { return this->phys; }
	std::map< std::string, osg::ref_ptr< Tlink > >& _getTeleporterLinks() { return this->links; }
	std::map< std::string, define* >& _getGroups() { return this->groups; }

	void _addObject( bz2object* obj );
	DataEntry* _buildObject( const char* header );
	void _removeObject( bz2object* obj );
	void _removeMaterial( material* mat );
	void _removePhysicsDriver( physics* phydrv );
	void _removeTextureMatrix( texturematrix* texmat );
	void _removeDynamicColor( dynamicColor* dyncol );
	void _removeGroup( define* def );
	void _setSelected( bz2object* obj );
	void _setUnselected( bz2object* obj );
	void _selectAll();
	void _unselectAll();
	bool _isSelected( bz2object* obj );
	objRefList& _getSelection() { return this->selectedObjects; }
	void _assignMaterial( const std::string& matref, bz2object* obj );
	void _assignMaterial( material* matref, bz2object* obj );
	ConfigurationDialog* _configureObject( DataEntry* obj );

	bool _renameMaterial( std::string oldName, std::string newName );
	bool _renameDynamicColor( std::string oldName, std::string newName );
	bool _renameTextureMatrix( std::string oldName, std::string newName );
	bool _renamePhysicsDriver( std::string oldName, std::string newName );
	bool _renameTeleporterLink( std::string oldName, std::string newName );
	bool _renameGroup( std::string oldName, std::string newName );

	// editor-like methods (BZWB-specific)--instantiated
	bool _cutSelection();
	bool _copySelection();
	bool _pasteSelection();
	bool _deleteSelection();
	bool _newWorld();
	bool _linkTeleporters( teleporter* t1, teleporter* t2 );
	void _groupObjects( Model::objRefList& objects );
	void _ungroupObjects( group* g );

	// plugin-specific API
	static bool registerObject(std::string& name, DataEntry* (*init)());
	static bool registerObject(const char* name, const char* hierarchy, const char* terminator, DataEntry* (*init)(), ConfigurationDialog* (*config)(DataEntry*) = NULL);
	static bool isSupportedObject(const char* name);
	static bool isSupportedTerminator(const char* name, const char* end);
	static bool isSupportedHierarchy(const char* name);

	// model-specific API
	static const std::string getSupportedObjects();
	static const std::string getSupportedHierarchies();
	static const std::string getSupportedTerminators();
	static bool hasInitializer( DataEntry* d );
	static bool hasConfigurationDialog( DataEntry* d );

	// methods for getting the last error encountered while reading a bzw file
	BZWReadError getLastError();
	int getLastErrorLineNumber();

	// methods to manipulate the aforementioned strings
	static bool addObjectSupport(const char* name);
	bool _addObjectSupport(const char* name);

	static bool removeObjectSupport(const char* name);
	bool _removeObjectSupport(const char* name);

	static bool addTerminatorSupport(const char* name, const char* end);
	bool _addTerminatorSupport(const char* name, const char* end);

	static bool removeTerminatorSupport(const char* name, const char* end);
	bool _removeTerminatorSupport(const char* name, const char* end);

	static bool addSupportedHierarchy(const char* name);
	bool _addSupportedHierarchy(const char* name);

	static bool removeSupportedHierarchy(const char* name);
	bool _removeSupportedHierarchy(const char* name);

	// instantiated plug-in API
	bool _registerObject(std::string& name, DataEntry* (*init)());
	bool _registerObject(const char* name, const char* hierarchy, const char* terminator, DataEntry* (*init)(), ConfigurationDialog* (*config)(DataEntry*));
	bool _isSupportedObject(const char* name);
	bool _isSupportedTerminator(const char* name, const char* end);
	bool _isSupportedHierarchy(const char* name);

	// model-specific API
	const std::string _getSupportedObjects();
	const std::string _getSupportedHierarchies();
	const std::string _getSupportedTerminators();
	bool _hasInitializer( DataEntry* d ) { return ( cmap.count( d->getHeader() ) == 0 || cmap[ std::string(d->getHeader()) ] == NULL ) ? false : true; }
	bool _hasConfigurationDialog( DataEntry* d ) { return ( configMap.count( d->getHeader() ) == 0 || configMap[ std::string(d->getHeader()) ] == NULL ) ? false : true; }

	// methods for getting the last error encountered while reading a bzw file
	BZWReadError _getLastError();
	int _getLastErrorLineNumber();


private:

// error exception and line
	BZWReadError error;
	int errorLine;

// world options
	world* worldData;
	options* optionsData;
	waterLevel* waterLevelData;
	info* infoData;

// physics (map refname to object )
	std::map< std::string, osg::ref_ptr< physics > > phys;

// materials (map refname to object )
	std::map< std::string, osg::ref_ptr< material > > materials;
	osg::ref_ptr< material > defaultMaterial; 		// default material

// group definitions (map refname to object)
	std::map< std::string, define* > groups;

// dynamic colors (map refname to the object )
	std::map< std::string, dynamicColor* > dynamicColors;

// links (map refname to the object )
	std::map< std::string, osg::ref_ptr< Tlink > > links;

// texture matrices (map refname to the object itself)
	std::map< std::string, texturematrix* > textureMatrices;

// objects
	objRefList objects;

// world data (array of all objects in BZW format)
	std::vector<std::string> data;

// map from strings to initializers
	std::map<std::string, DataEntry* (*)()> cmap;

// map from strings to configuration dialog initializers
	std::map<std::string, ConfigurationDialog* (*)(DataEntry*)> configMap;

// build the default bzw objects in
	void buildDatabase();

	// clear all objects
	void clear();

// list of registered object keys
	std::string supportedObjects;

// list of object/terminator pairs (exceptions to object..end)
	std::string objectTerminators;

// hierarchy of objects and subobjects
	std::string objectHierarchy;

// save unused data chunks
	std::vector<std::string> unusedData;

// vector of selected objects
	objRefList selectedObjects;

// cut/copy buffer
	objRefList objectBuffer;

	static Model* modRef;
};

#endif /*MODEL_H_*/
