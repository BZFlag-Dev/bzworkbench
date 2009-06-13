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

#ifndef ZONE_H_
#define ZONE_H_

#include "bz2object.h"
#include "flags.h"

/**
 * Zone object
 */

class zone : public bz2object {
	
public:

	/*
	 * Sub-class that contains a flag/quantity pair
	 */
	 
	class FlagElement {
	public:
	
		// simple construtor
		FlagElement(string _flag, int _qty) {
			flag = _flag;
			qty = _qty;
		}
		
		// self-parser toString() method
		string toString(void) {
			return flag + " " + string(itoa(qty));
		}
		
		// self-reader update() method
		void update(string data) {
			string fl = BZWParser::key(data.c_str());
			string n = BZWParser::value(fl.c_str(), data.c_str());
			
			// quit if there is no flag
			if(fl == BZW_NOT_FOUND)	
				return;
				
			// quit if there was no quantity
			if(n == BZW_NOT_FOUND)
				return;
				
			// load the data
			flag = fl;
			qty = atoi( n.c_str() );
		}

		string getFlag() { return flag; }
		int getQty() { return qty; }
		
		string flag;
		int qty;
	};
	
	// constructor
	zone();
	
	// constructor with data
	zone(string& data);
	
	static DataEntry* init() { return new zone(); }
	static DataEntry* init(string& data) { return new zone(data); }

	void setDefaults();
	
	// getter
	string get(void);
	
	// setter
	int update( string& data );
	int update( UpdateMessage& message );

	
	// toString
	string toString(void);
	
	// render
	int render(void);

	vector<int> getTeams() { return teams; }
	vector<int> getSafety() { return safety; }
	vector<FlagElement> getZoneflags() { return zoneflags; }
	vector<string> getFlags() { return flags; }

	void setTeams( vector<int> value ) { teams = value; }
	void setSafety( vector<int> value ) { safety = value; }
	void setZoneflags( vector<FlagElement> value ) { zoneflags = value; }
	void setFlags( vector<string> value ) { flags = value; }


private:
	vector<int> teams;
	vector<int> safety;
	vector<FlagElement> zoneflags;
	vector<string> flags;

	void updateGeometry();
};

#endif /*ZONE_H_*/
