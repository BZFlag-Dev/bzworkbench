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

#include "widgets/Point3DWidget.h"

#include "defines.h"

Point3DWidget::Point3DWidget( int x, int y ) : Fl_Group( x, y, 280, DEFAULT_TEXTSIZE + 6 ) {
	xLabel = new QuickLabel( "X", x + 3, y );
	xInput = new Fl_Value_Input( x + 15, y, 60, DEFAULT_TEXTSIZE + 6 );

	yLabel = new QuickLabel( "Y", x + 78, y );
	yInput = new Fl_Value_Input( x + 90, y, 60, DEFAULT_TEXTSIZE + 6 );

	zLabel = new QuickLabel( "Z", x + 153, y );
	zInput = new Fl_Value_Input( x + 165, y, 60, DEFAULT_TEXTSIZE + 6 );

	end();
}

Point3D Point3DWidget::getPoint3D() {
	return Point3D( xInput->value(), yInput->value(), zInput->value() );
}

void Point3DWidget::setPoint3D( Point3D value ) {
	xInput->value( value.x() );
	yInput->value( value.y() );
	zInput->value( value.z() );
}

