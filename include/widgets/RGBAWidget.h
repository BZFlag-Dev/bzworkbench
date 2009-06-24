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

#ifndef RGBAWIDGET_H_
#define RGBAWIDGET_H_

#include <osg/Vec4>

#include <FL/Fl_Group.H>
#include <FL/Fl_Value_Input.H>

#include "widgets/QuickLabel.h"

class RGBAWidget : public Fl_Group {

public:
	RGBAWidget( int x, int y );

	osg::Vec4 getRGBA();

	void setRGBA( osg::Vec4 value );
private:
	QuickLabel* rLabel;
	QuickLabel* gLabel;
	QuickLabel* bLabel;
	QuickLabel* aLabel;

	Fl_Value_Input* rInput;
	Fl_Value_Input* gInput;
	Fl_Value_Input* bInput;
	Fl_Value_Input* aInput;
};

#endif