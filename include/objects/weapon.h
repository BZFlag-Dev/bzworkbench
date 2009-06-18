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

#ifndef WEAPON_H_
#define WEAPON_H_

#include "bz2object.h"
#include "../flags.h"


class weapon : public bz2object {

public:
	
	// default constructor
	weapon();
	
	// constructor with data
	weapon(string& data);
	
	static DataEntry* init() { return new weapon(); }
	static DataEntry* init(string& data) { return new weapon(data); }
	
	void setDefaults();

	// getter
	string get(void);
	
	// setter
	int update(string& data);
	
	// toString
	string toString(void);
	
	// render
	int render(void);
	
	int getColor() { return team; }
	string getType() { return type; }
	string getTrigger() { return trigger; }
	float getInitdelay() { return initdelay; }
	float getTilt() { return tilt; }
	vector<float> getDelay() { return delay; }
	int getEventTeam() { return eventTeam; }

	void setColor( int value ) { team = value; }
	void setType( string value ) { type = value; }
	void setTrigger( string value ) { trigger = value; }
	void setInitdelay( float value ) { initdelay = value; }
	void setTilt( float value ) { tilt = value; }
	void setDelay( vector<float> value ) { delay = value; }
	void setEventTeam( int value ) { eventTeam = value; }


private:
	string type, trigger;
	float initdelay, tilt;
	vector<float> delay;
	int team;
	int eventTeam;

	void updateGeometry();
};

#endif /*WEAPON_H_*/
