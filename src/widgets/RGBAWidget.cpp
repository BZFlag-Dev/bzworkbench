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

#include "widgets/RGBAWidget.h"

#include "defines.h"

RGBAWidget::RGBAWidget( int x, int y ) : Fl_Group( x, y, 280, DEFAULT_TEXTSIZE + 6 ) {
	rLabel = new QuickLabel( "R", x + 3, y );
	rInput = new Fl_Value_Input( x + 15, y, 60, DEFAULT_TEXTSIZE + 6 );

	gLabel = new QuickLabel( "G", x + 78, y );
	gInput = new Fl_Value_Input( x + 90, y, 60, DEFAULT_TEXTSIZE + 6 );

	bLabel = new QuickLabel( "B", x + 153, y );
	bInput = new Fl_Value_Input( x + 165, y, 60, DEFAULT_TEXTSIZE + 6 );

	aLabel = new QuickLabel( "A", x + 228, y );
	aInput = new Fl_Value_Input( x + 240, y, 60, DEFAULT_TEXTSIZE + 6 );

	end();
}

osg::Vec4 RGBAWidget::getRGBA() {
	return osg::Vec4( rInput->value(), gInput->value(), bInput->value(), aInput->value() );
}

void RGBAWidget::setRGBA( osg::Vec4 value ) {
	rInput->value( value.r() );
	gInput->value( value.g() );
	bInput->value( value.b() );
	aInput->value( value.a() );
}
