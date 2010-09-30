/*
 * info.cpp
 *
 *  Created on: Jun 18, 2009
 *      Author: william
 */

#include "objects/info.h"

using namespace std;

// default constructor
info::info() : bz2object("info", "") {
}

// getter
string info::get(void) {
	return toString();
}

// bzw methods
bool info::parse( std::string& line ) {
	string key = BZWParser::key( line.c_str() );
	string value = BZWParser::value( key.c_str(), line.c_str() );
	
	// check if we reached the end of the section
	if ( key == "end" )
		return false;

	// if we aren't at the end add the line
	infoLines.push_back( line );
	return true;
}

void info::finalize() {

}

// toString
string info::toString(void) {
	string ret("info\n");
	for ( vector<string>::iterator i = infoLines.begin(); i != infoLines.end(); i++ ) {
		ret += *i + "\n";
	}
	ret += "end\n";

	return ret;
}
