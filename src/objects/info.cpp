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

// setter
/*int info::update(string& data) {
	const char* header = getHeader().c_str();
	// get the chunk we need
	vector<string> chunks = BZWParser::getSectionsByHeader(header, data.c_str(), "end");

	// check and see if the proper data segment was found
	if(chunks[0] == BZW_NOT_FOUND) {
		printf("info: data not found\n");
		return 0;
	}

	if(!hasOnlyOne(chunks, "info")) {
		printf("info: improper data\n");
		return 0;
	}

	vector<string> lines = BZWParser::getLines( header, data.c_str() );

	// copy lines in skipping header (first line)
	infoLines.clear();
	for ( vector<string>::iterator i = ++lines.begin(); i != lines.end(); i++ ) {
		infoLines.push_back( *i );
	}

	return 1;
}*/

// bzw methods
bool info::parse( std::string& line ) {
	return false;
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
