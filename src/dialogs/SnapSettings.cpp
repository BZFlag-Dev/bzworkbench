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

#include "dialogs/SnapSettings.h"

#include "defines.h"

SnapSettings::SnapSettings( MainWindow* mw )  :
		Fl_Dialog( "Material Editor", 250, 100, Fl_Dialog::Fl_OK )
{
	parent = mw;

	begin();

	translateLabel = new QuickLabel( "Translate:", 5, 5 );
	translateInput = new Fl_Value_Input( 120, 5, 120, DEFAULT_TEXTSIZE + 6 );
	translateInput->value( mw->getView()->getTranslateSnapSize() );

	rotateLabel = new QuickLabel( "Rotate:", 5, 30 );
	rotateInput = new Fl_Value_Input( 120, 30, 120, DEFAULT_TEXTSIZE + 6 );
	rotateInput->value( mw->getView()->getRotateSnapSize() );

	scaleLabel = new QuickLabel( "Scale:", 5, 55 );
	scaleInput = new Fl_Value_Input( 120, 55, 120, DEFAULT_TEXTSIZE + 6 );
	scaleInput->value( mw->getView()->getScaleSnapSize() );
	
	end();

	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );
}

void SnapSettings::OKCallback_real( Fl_Widget* w ) {
	parent->getView()->setTranslateSnapSize( translateInput->value() );
	parent->getView()->setScaleSnapSize( scaleInput->value() );
	parent->getView()->setRotateSnapSize( rotateInput->value() );
	hide();
}

void SnapSettings::CancelCallback_real( Fl_Widget* w ) {
	hide();
}