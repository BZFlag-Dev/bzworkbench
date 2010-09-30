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

#ifndef LOD_H_
#define LOD_H_

#include "DataEntry.h"
#include "model/Model.h"
#include "model/BZWParser.h"
#include "LODCommand.h"

#include <string>
#include <vector>

class LOD : public DataEntry {

public:
	
	struct MaterialSet {
		string matref;
		vector<LODCommand> commands;
	};

	// default constructor
	LOD() : DataEntry("lod", "<matref><lengthperpixel>") {
		lengthPerPixel = 0;
		currentMatSet = NULL;
		currentCommand = NULL;
		matSets.empty();
	}
	
	// constructor with data
	LOD(string& data) : DataEntry("lod", "<matref><lengthperpixel>", data.c_str()) {
		lengthPerPixel = 0;
		currentMatSet = NULL;
		currentCommand = NULL;
		matSets.empty();		
		this->update(data);
	}
	
	// getter
	string get(void) { return this->toString(); }
	
	// setter
	int update(string& data) {
		const char* _header = this->getHeader().c_str();
		
		// get the data
		vector<string> lines = BZWParser::getSectionsByHeader(_header, data.c_str());
		
		// make sure we only have one
		if(lines[0] == BZW_NOT_FOUND)
			return 0;
		if(!hasOnlyOne(lines, _header))
			return 0;
			
		// get the data
		const char* lodData = lines[0].c_str();
		
		// get the lengthPerPixel values
		vector<string> lppVals = BZWParser::getValuesByKey("lengthPerPixel", _header, lodData);
		//use last lengthPerPixel
		this->lengthPerPixel = (lppVals.size() != 0 ? atoi( lppVals[lppVals.size()-1].c_str() ) : 0);
			
		// get the sections beginning with matref and create LODCommand lists from them
		vector<string> matrefSections = BZWParser::getSectionsByHeader("matref", lodData);
		for(vector<string>::iterator m = matrefSections.begin() + 1; m != matrefSections.end(); m++){
			currentMatSet = new MaterialSet();
			currentMatSet->matref = BZWParser::value( "matref", m[0].c_str() );
			// parse each LOD command
			vector<string> lodLines = BZWParser::getLines("matref", matrefSections[0].c_str());
			vector<LODCommand> cmds = vector<LODCommand>();
			if(lodLines.size() > 1) {
				for(vector<string>::iterator i = lodLines.begin() + 1; i != lodLines.end(); i++) {
					if(LODCommand::isValidCommand( *i )) {
						currentMatSet->commands.push_back( LODCommand(*i) );
					}
					else {
						printf("mesh::LOD::update(): Error! Unsupported LOD entry \"%s\"\n", i->c_str());
						//return 0;
					}
				}
			}
			matSets.push_back(currentMatSet);
		}
		// do base class update
		if(!DataEntry::update(data))
			return 0;
		
		return 1;
	}
	
	// toString
	string toString(void) {
		string matsets = string("");
		if(matSets.size() > 0) {
			for( vector<MaterialSet*>::iterator i = matSets.begin(); i != matSets.end(); i++) {
				if((*i)->commands.size() > 0 && (*i)->matref.length() > 0) {
					matsets += "      matref " + (*i)->matref + "\n";
					for( vector<LODCommand>::iterator j = (*i)->commands.begin(); j != (*i)->commands.end(); j++) {
						matsets += "        " + j->toString() + "\n";	
					}
					matsets += "      end\n";
				}
			}
		}
		return string("lod\n") + 
				"      lengthPerPixel " + string(ftoa(lengthPerPixel)) + "\n" +
				matsets + "    end\n";
	}
	
	bool parse( string& line ) { 
		string key = BZWParser::key( line.c_str() );
		string value = BZWParser::value( key.c_str(), line.c_str() );
		
		if ( currentCommand ) {
			if ( !currentCommand->parse( line ) ) {
				matSets.push_back(currentMatSet);
				currentCommand = NULL;
				currentMatSet = NULL;
			}else{
				currentMatSet->commands.push_back(*currentCommand);
				currentCommand = new LODCommand();
			}
			return true;
		}
		else if ( key == "lengthperpixel"){
			// lengthPerPixel <value>
			value = BZWParser::value( key.c_str(), TextUtils::tolower(line.c_str()).c_str() );
			lengthPerPixel = atof(value.c_str());
			return true; 
		}
		else if ( key == "matref"){
			// matref <name> (repeatable)
			currentMatSet = new MaterialSet();
			currentMatSet->matref = value;
			currentMatSet->commands = vector<LODCommand>();
			currentCommand = new LODCommand();
			return true; 
		}
		else if( key == "end" ){
			// end matref, lod or drawinfo
			return false;
		}
		throw BZWReadError( this, string( "Unknown LOD Command: " ) + line );
		return true;
	} 
	
	// render
	int render(void) {
		return 0;	
	}
	
	vector<MaterialSet*>& getMaterialSets() {return matSets;}
	float getLengthPerPixel() {return lengthPerPixel;}
	
private:
	
	
	
	vector<MaterialSet*> matSets;
	MaterialSet* currentMatSet;
	LODCommand* currentCommand;
	float lengthPerPixel;
	
};

#endif /*LOD_H_*/
