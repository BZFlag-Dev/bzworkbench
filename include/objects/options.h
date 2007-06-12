#ifndef OPTIONS_H_
#define OPTIONS_H_

#include "../DataEntry.h"
#include <string>

using namespace std;

class options : public DataEntry {
public:
	
	// default constructor
	options();
	
	// constructor with data
	options(string& data);
	
	// get method
	string get(void);
	
	// update method
	int update(string& data);
	
	// toString method
	string toString(void);
	
	// render method
	int render(void);
private:
	string optionsString;
};

#endif /*OPTIONS_H_*/
