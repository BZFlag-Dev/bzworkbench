#include "../include/objects/box.h"

// constructors
box::box() : bz2object("box", "<position><rotation><size>") {
	this->addChild( SceneBuilder::buildNode("share/box/box.obj") );
	this->setName( SceneBuilder::nameNode("share/box/box.obj") );
	
	this->setPosition( osg::Vec3(0.0, 0.0, 0.0) );
	this->setScale( osg::Vec3(10.0, 10.0, 10.0) );
	SceneBuilder::markUnselected( this );
}

box::box(string& data) : bz2object("box", "<position><rotation><size>", data.c_str()) {
	this->addChild( SceneBuilder::buildNode("share/box/box.obj") );
	this->setName( SceneBuilder::nameNode("share/box/box.obj") );
	
	this->update(data);	
}

// nothing to destroy...
box::~box() { }

// getter
string box::get(void) {
	return this->toString();
}

// setter
int box::update(string& data) {
	return bz2object::update(data);
}

// toString
string box::toString(void) {
	return string("box\n") +
				  this->BZWLines() +
				  "end\n";
}