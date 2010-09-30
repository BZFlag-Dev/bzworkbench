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

#include "widgets/TexcoordWidget.h"

#include "defines.h"

TexcoordWidget::TexcoordWidget( int x, int y ) : Fl_Group( x, y, 280, DEFAULT_TEXTSIZE + 6 ) {
	uLabel = new QuickLabel( "U", x + 3, y );
	uInput = new Fl_Value_Input( x + 15, y, 60, DEFAULT_TEXTSIZE + 6 );

	vLabel = new QuickLabel( "V", x + 78, y );
	vInput = new Fl_Value_Input( x + 90, y, 60, DEFAULT_TEXTSIZE + 6 );

	end();
}

TexCoord2D TexcoordWidget::getTexcoord() {
	return TexCoord2D( uInput->value(), vInput->value() );
}

void TexcoordWidget::setTexcoord( TexCoord2D value ) {
	uInput->value( value.u );
	vInput->value( value.v );
}
