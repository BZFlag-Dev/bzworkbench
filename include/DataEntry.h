#ifndef DATAENTRY_H_
#define DATAENTRY_H_

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

#include "ftoa.h"

// the generic map object class
class DataEntry {
public:
	
	virtual string get(void) = 0;
	virtual void update(string&) = 0;
	
	virtual string toString() {
		return string("# unknown DataEntry");
	}
	
	virtual int render(void) = 0;
	
	// initialize the header and keys
	DataEntry(const char* header, const char* keys) {
		this->header = string(header);
		this->keys = string(keys);
	}
	
	
	virtual ~DataEntry() { }
	
	// is the key supported?
	bool isKey(string& key) {
		string::size_type index = this->keys.find("<" + key + ">", 0);
		if(index != string::npos)
			return true;
		return false;
	}
	bool isKey(const char* key) { string k = string(key); return isKey(k); }
	
	// get the supported keys
	string getKeys(void) { return keys; }
	
	// get the header
	string getHeader(void) { return header; }
	
	// helper method:  ensures that a key occurs only once (useful for calling from update())
	bool hasOnlyOne(vector<string> lines, const char* key) {
		if( lines.size() != 1) {
			printf("%s::update():  Error!  Defined \"%s\" %d times!\n", header.c_str(), key, lines.size());
			return false;	
		}
		return true;
	}
	
	// set the header
	void setHeader(const char* c) { this->header = string(c); }
	
	// set the keys
	void setKeys(const char* c) { this->keys = string(c); }
	
private:
	string header;
	string keys;
};

#endif /*DATAENTRY_H_*/
