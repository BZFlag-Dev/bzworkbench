#include "../include/model/BZWParser.h"

/**
 * Helper method:  eliminate the whitespace on the ends of the line
 */
string cutWhiteSpace(string line) {
	const char* text = line.c_str();
	unsigned long len = line.length();
	unsigned int startIndex = 0, endIndex = len - 1;
	
	// cut any comments
	string::size_type commentIndex = line.find("#", 0);
	if(commentIndex != string::npos) {
		line = line.substr(0, commentIndex);	
	}
	
	// move the indexes into the string by skipping outside spacess
	while(startIndex < len && (text[startIndex] == ' ' || text[startIndex] == 9 || text[startIndex] == '\r')) { startIndex++; }
	while(endIndex > startIndex && (text[endIndex] == ' ' || text[endIndex] == 9 || text[endIndex] == '\n' || text[endIndex] == '\r')) { endIndex--; }
	
	// return the line if there was no white space to cut
	if(startIndex == len)
		return line;
	
	// return the inner content
	return line.substr(startIndex, endIndex - startIndex + 1);
}

/**
 * Helper method:  return a substring from the beginning of the string to the next occurence of '\n'
 */
string cutLine(string text) {
	string::size_type index = text.find("\n", 0);
	return text.substr(0, index);
}

/**
 * Helper method:  does a text chunk have another line in it?
 */

bool hasLine(string text) {
	string::size_type index = text.find("\n", 0);
	if(index == string::npos)
		return false;
		
	return true;
}

/**
 * Helper method:  determines whether or not the passed key is the key of the passed line
 */
 bool isKey(string key, string line) {
 	line = cutWhiteSpace(line);
 	string::size_type index = line.find(key + " ", 0);
 	if(index == 0) 
 		return true;
 	return false;
 }

/**
 * Get the value text from a line
 */
string BZWParser::value(const char* _key, const char* _text) {
	string line = cutWhiteSpace(string(_text));
	string key = string(_key);
	
	unsigned int startIndex = line.find(key, 0);
	
	// stop if we didn't find the key
	if(startIndex == string::npos)
		return string(BZW_NOT_FOUND);
		
	// advance to the start of the value
	startIndex += key.length() + 1;
	
	// stop if the first key is the value
	if(startIndex > line.length())
		return key;
	
	// get the value
	string value = line.substr(startIndex, line.length() - startIndex);
	
	// trunicate the value and return it
	return cutWhiteSpace(value);
}

/**
 * Get the key of a line
 */
string BZWParser::key(const char* _text) {
	string text = cutWhiteSpace(_text);
	string::size_type index = text.find(" ", 0);
	if(index == string::npos)
		return text;
	return text.substr(0, index);
}

/**
 * This method returns the terminator token of a key.  Usually, it's "end"
 */
 
string BZWParser::terminatorOf(const char* _text) {
	string key = string(_text);
	string terms = string(BZW_TERMINATORS);
	
	// see of there's an unusual terminator
	string::size_type start = terms.find("<" + key + "|", 0);
	if(start != string::npos) {
		start += 2 + key.length();
		string::size_type end = terms.find(">", start);
		return terms.substr(start, end - (start));
	}
	
	return string("end");
}

/**
 * This method will extract the key-value lines from a section, given the object name of the section and its text
 */
vector<string> BZWParser::getLines(const char* _start, const char* _text) {
	string start = string(_start);
	string text = string(_text);
	
	string header = cutWhiteSpace(start);
	string section = cutWhiteSpace(text);
	string terminator = BZWParser::terminatorOf(header.c_str());
	
	vector<string> lines = vector<string>();
	
	// find the start of the section
	while(true) {
		// make sure we have line enough to cut
		if(section.length() < header.length())
			break;
			
		// get a line
		string currLine = cutLine(section);
		
		// try to find the header in it
		string::size_type index = currLine.find(header, 0);
		
		if(index == 0)
			break;
		
		// advance the line
		section = section.substr(currLine.length() + 1);
	}
	
	// read in lines
	while(true) {
		// get the next line
		string currLine = cutLine(section);
		
		// break if we get to "end"
		if(currLine.compare(terminator) == 0)
			break;
		
		// add the line (remove whitespace)
		lines.push_back(cutWhiteSpace(currLine));
		
		// check to see if we ran out of room (then return if so)
		if(section.length() < currLine.length() + 1)
			return lines;
			
		// advance the section
		section = section.substr(currLine.length() + 1);
	}
	
	return lines;
}

/**
 * This method gets all the lines in a section starting with a given key
 */
vector<string> BZWParser::getLinesByKey(const char* _key, const char* _header, const char* _text) {
	
	string key = cutWhiteSpace(string(_key));
	string header = cutWhiteSpace(string(_header));
	string text = cutWhiteSpace(string(_text));
	
	// get all lines from the section
	vector<string> lines = BZWParser::getLines(_header, _text);
	
	// the lines starting with key
	vector<string> ret = vector<string>();
	
	// find the lines with the matching key
	for(vector<string>::iterator i = lines.begin(); i != lines.end(); i++) {
		if(isKey(key, *i)) {
			ret.push_back(*i);
		}
	}
	
	return ret;
}

/*
 * This method reads a section that starts with header from a chunk of text 
 */
string BZWParser::getSection(const char* _header, const char* _text) {
	string header = cutWhiteSpace(string(_header));
	string text = cutWhiteSpace(string(_text));
	string terminator = BZWParser::terminatorOf(header.c_str());
	
	// find header occurence
	string::size_type index = text.find(header, 0);
	
	// if we didn't find anything then return an empty string
	if(index == string::npos)
		return string("");
	
	// find the end of the section
	string::size_type end = text.find(terminator + "\n", index);
	
	// if we didn't find anything then return the rest of the section
	if(end == string::npos)
		return text.substr(index);
	
	return text.substr(index, end - index);
}

/**
 * This method finds all occurences of a section (by looking for its header) in a chunk of text
 */
const vector<string> BZWParser::getSectionsByHeader(const char* _header, const char* _text, const char* _footer) {
	string header = cutWhiteSpace(string(_header));
	string text = cutWhiteSpace(string(_text)) + " ";
	string footer = cutWhiteSpace(string(_footer));
	
	vector<string> sections = vector<string>();
	int cnt = 0;		// how many sections
	
	while(true) {
		string section = string("");
		
		string currLine, line;
		
		bool found = false;
		
		// find the header 
		while(true) {
			
			// ensure we have space to cut
			if(!hasLine(text)) {
				break;
			}
				
			// cut a line
			currLine = cutLine(text);
			line = cutWhiteSpace(currLine);
			
			// chunk up the line
			vector<string> lineElements = getLineElements(line.c_str());
			
			if(lineElements.size() == 0)
				lineElements.push_back(" ");
				
			// see if it has the header
			if(lineElements[0].compare(header) == 0) {
				section += line + "\n";
				text = text.substr(currLine.length() + 1);
				found = true;
				break;
			}
			
			text = text.substr(currLine.length() + 1);
		}
		
		found = false;
		// find the footer
		while(true) {
			// ensure we have space to cut
			if(!hasLine(text)) {
				break;
			}
				
			// cut a line
			currLine = cutLine(text);
			line = cutWhiteSpace(currLine);
			
			// chunk up the line
			vector<string> lineElements = getLineElements(line.c_str());
			
			if(lineElements.size() == 0)
				lineElements.push_back(" ");
			
			// add it to the section
			section += line + "\n";
			
			// see if it has the end
			if(lineElements[0].compare(footer) == 0) {
				found = true;
				break;
			}
				
			
			text = text.substr(currLine.length() + 1);
		}
		
		if(!found)
			break;	
		else {
			sections.push_back(section);
			cnt++;
		}
	}
	// add SOMETHING if we found nothing
	if(cnt == 0)
		sections.push_back(BZW_NOT_FOUND);
		
	return sections;
}

const vector<string> BZWParser::getSectionsByHeader(const char* _header, const char* _text) {
	return BZWParser::getSectionsByHeader(_header, _text, BZWParser::terminatorOf(_header).c_str());	
}

/**
 * General case of getSectionsByHeader.
 * This method, unlike the others, anticipates subobjects listed in internalSectionKeys, and won't be 
 * fooled by internal "end" statements.
 * Format of internalSectionKeys is "<key1><key2>...<keyN>" for N keys
 */
 
const vector<string> BZWParser::getSectionsByHeader(const char* _header, const char* _text, const char* _footer, const char* internalSectionKeys) {
	string header = cutWhiteSpace(string(_header));
	string text = cutWhiteSpace(string(_text)) + " ";
	string footer = cutWhiteSpace(string(_footer));
	string keyList = cutWhiteSpace(string(internalSectionKeys));
	int sectionDepth = 0;	// determines the section depth (will be positive if inside a subobject)
	
	vector<string> sections = vector<string>();
	int cnt = 0;		// how many sections
	
	while(true) {
		string section = string("");
		
		string currLine, line;
		
		bool found = false;
		
		// find the header 
		while(true) {
			
			// ensure we have space to cut
			if(!hasLine(text)) {
				break;
			}
				
			// cut a line
			currLine = cutLine(text);
			line = cutWhiteSpace(currLine);
			
			// chunk up the line
			vector<string> lineElements = getLineElements(line.c_str());
			
			if(lineElements.size() == 0)
				lineElements.push_back(" ");
			
			// see if it has the header
			if(lineElements[0].compare(header) == 0) {
				section += line + "\n";
				text = text.substr(currLine.length() + 1);
				found = true;
				break;
			}
			
			text = text.substr(currLine.length() + 1);
		}
		
		found = false;
		// find the footer
		while(true) {
			// ensure we have space to cut
			if(!hasLine(text)) {
				break;
			}
				
			// cut a line
			currLine = cutLine(text);
			line = cutWhiteSpace(currLine);
			
			// chunk up the line
			vector<string> lineElements = getLineElements(line.c_str());
			
			if(lineElements.size() == 0)
				lineElements.push_back(" ");
			
			// see if this is a key
			if(keyList.find("<" + lineElements[0] + ">") != string::npos)	
				sectionDepth++;
			
			// add it to the section if its relevant
			if(sectionDepth == 0)
				section += line + "\n";
			
			// see if it has the end
			if(lineElements[0].compare(footer) == 0) {
				sectionDepth--;
				if(sectionDepth == 0) {
					found = true;
					break;	
				}
			}
				
			
			text = text.substr(currLine.length() + 1);
		}
		
		if(!found)
			break;	
		else {
			sections.push_back(section);
			cnt++;
		}
	}
	// add SOMETHING if we found nothing
	if(cnt == 0)
		sections.push_back(BZW_NOT_FOUND);
		
	return sections;
}


/**
 * This method gets the list of all values referenced by a key in a segment of text (usually a section)
 */
vector<string> BZWParser::getValuesByKey(const char* _key, const char* _header, const char* _text) {
	string key = cutWhiteSpace(string(_key));
	string header = cutWhiteSpace(string(_header));
	string text = cutWhiteSpace(string(_text));
	
	// get all lines with the key
	vector<string> lines = BZWParser::getLinesByKey(_key, _header, _text);
	
	// return value
	vector<string> ret = vector<string>();
	
	// get the values and load up ret
	for(vector<string>::iterator i = lines.begin(); i != lines.end(); i++) {
		string value = BZWParser::value(_key, i->c_str());
		ret.push_back( value );
	}
	
	return ret;
}

/**
 * This method gets all values to a set of keys and preserves the order in which the occur
 */
 
vector<string> BZWParser::getLinesByKeys(vector<string> keys, const char* _header, const char* _section) {
	string header = cutWhiteSpace(_header);
	string section = cutWhiteSpace(_section);
	
	vector<string> lines = BZWParser::getLines(_header, _section);
	
	vector<string> ret = vector<string>();
	
	for(vector<string>::iterator i = lines.begin(); i != lines.end(); i++) {
		for(vector<string>::iterator j = keys.begin(); j != keys.end(); j++) {
			if(isKey(*j, *i)) {
				ret.push_back(*i);
				break;	
			}	
		}	
	}
	
	return ret;
}

/**
 * This method gets all the elements in a line separated by one or more spaces
 */
vector<string> BZWParser::getLineElements(const char* data) {
	vector<string> ret = vector<string>();
	string line = cutWhiteSpace(string(data)) + " ";
	
	// separate all elements by finding the " "s
	while(true) {
		if(line.length() < 1)
			break;
			
		string::size_type spaceIndex = line.find(" ", 0);
		if(spaceIndex == string::npos)
			break;
			
		string element = line.substr(0, spaceIndex);
		
		ret.push_back(element);
		
		line = line.substr(spaceIndex + 1);
	}
	
	return ret;
}

/**
 * The big tamale: the top-level file loader.
 * Loads a .bzw file and returns a vector of strings, where each string contains an object's chunk of BZW text
 */

vector<string> BZWParser::loadFile(const char* filename) {
	ifstream fileInput(filename);
	
	vector<string> ret = vector<string>();
	
	// if its not open, its not there
	if(!fileInput.is_open()) {
		printf("BZWParser::loadFile(): Error! Could not open input stream\n");
		return ret;
	}
	
	// start reading the stuff in
	string buff, key, objstr, line, supportedKeys = string(BZW_SUPPORTED_KEYS), hierarchy = string(BZW_SUBOBJECTS);
	vector<string> lineElements = vector<string>();
	
	while(!fileInput.eof()) {
		// read in lines until we find a key
		getline( fileInput, buff );
		
		buff = cutWhiteSpace(buff);
		
		// get the line elements
		lineElements = BZWParser::getLineElements(buff.c_str());
		
		// if there are no line elements, continue
		if(lineElements.size() == 0)
			continue;
		
		// get the key	
		key = BZWParser::key( lineElements[0].c_str() );
		
		// is it a valid key?
		if( supportedKeys.find( "<" + key + ">", 0 ) != string::npos) {
			// if so, read in the object
			objstr = buff + "\n";
			
			// depth count--determines how deep the parser is in an object hierarchy
			// initialized to 1 now that we are in an object
			int depthCount = 1;
			
			// get any sub-object keys
			vector<string> subobjects = vector<string>();
			string::size_type index = hierarchy.find("<" + key + ":", 0);
			
			// load them in if we found any
			if( index != string::npos ) {
				subobjects.clear();
				
				// get the subobject string
				string::size_type subObjectStart = index + key.length() + 2;
				string::size_type subObjectEnd = hierarchy.find(">>", subObjectStart);
				string subobjectString = hierarchy.substr(subObjectStart, subObjectEnd);
				
				// read in the sub-object keys
				while(true) {
					string::size_type start = subobjectString.find("<", 0);
					string::size_type end = subobjectString.find(">", 0);
					
					// read in the key (and terminate it with a space)
					string subKey = subobjectString.substr(start+1, end - (start+1)) + " ";
					subobjects.push_back(subKey);
					
					// see if there are any more keys left
					if(subobjectString.find("<", end) == string::npos)
						break;
					
					// advance the line
					subobjectString = subobjectString.substr(end + 1);
				}
			}
			
			// get number of subobjects
			int numSubobjects = subobjects.size();
			
			// terminator stack
			vector<string> terminatorStack = vector<string>();
			
			// add the terminator token of the base object
			terminatorStack.push_back( BZWParser::terminatorOf(key.c_str()) );
			
			// loop through the text until we have depth 0 (i.e. we exit the object) or we run out of file
			while(!fileInput.eof() && depthCount > 0) {
				// get a line
				getline( fileInput, buff );
				
				// trim the whitespace
				line = cutWhiteSpace(buff);
				
				// don't continue of there was nothing to begin with
				if(line.length() == 0)
					continue;
				
				// add a space to line (so we can tell the difference between, say, "foo" and "foob"
				line += " ";
				
				// see if it's a subobject (if so, increase depthcount)
				for(int i = 0; i < numSubobjects; i++) {
					if( line.find( subobjects[i], 0 ) == 0 ) {
						depthCount++;
						terminatorStack.push_back( BZWParser::terminatorOf(subobjects[i].c_str()) );
						break;	
					}
				}
				
				// see if we're at an "end" (if so, decrease depthcount)
				if(line.find(terminatorStack[ terminatorStack.size() - 1 ] + " ", 0) != string::npos) {
					depthCount--;
					terminatorStack.pop_back();
				}
				
				objstr += line + "\n";
			}
			
			// add the object to ret
			ret.push_back(objstr);
		}
	}
	
	fileInput.close();
	
	return ret;
}
