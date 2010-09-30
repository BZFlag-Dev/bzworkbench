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

#ifndef TEXCOORDWIDGET_H_
#define TEXCOORDWIDGET_H_

#include <osg/Vec2>

#include <FL/Fl_Group.H>
#include <FL/Fl_Value_Input.H>

#include "widgets/QuickLabel.h"

#include "render/TexCoord2D.h"

class TexcoordWidget : public Fl_Group {

public:
	TexcoordWidget( int x, int y );

	TexCoord2D getTexcoord();

	void setTexcoord( TexCoord2D value );
private:
	QuickLabel* uLabel;
	QuickLabel* vLabel;

	Fl_Value_Input* uInput;
	Fl_Value_Input* vInput;
};

#endif /* TEXCOORDWIDGET_H_ */
