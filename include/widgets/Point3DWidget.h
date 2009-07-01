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

#ifndef POINT3DWIDGET_H_
#define POINT3DWIDGET_H_

#include <FL/Fl_Group.H>
#include <FL/Fl_Value_Input.H>

#include "widgets/QuickLabel.h"

#include "render/Point3D.h"

class Point3DWidget : public Fl_Group {

public:
	Point3DWidget( int x, int y );

	Point3D getPoint3D();

	void setPoint3D( Point3D value );
private:
	QuickLabel* xLabel;
	QuickLabel* yLabel;
	QuickLabel* zLabel;

	Fl_Value_Input* xInput;
	Fl_Value_Input* yInput;
	Fl_Value_Input* zInput;
};

#endif /* POINT3DWIDGET_H_ */
