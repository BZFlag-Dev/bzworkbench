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

#include "dialogs/RenameDialog.h"

#include "defines.h"

RenameDialog::RenameDialog()  :
Fl_Dialog( "Rename", 250, 50, Fl_Dialog::Fl_OK | Fl_Dialog::Fl_CANCEL )
{
	begin();

	nameLabel = new QuickLabel( "Name:", 5, 5 );
	nameInput = new Fl_Input( 120, 5, 120, DEFAULT_TEXTSIZE + 6 );
	
	end();

	cancelled = false;

	// add the callbacks
	setOKEventHandler( OKCallback, this );
	setCancelEventHandler( CancelCallback, this );
}

void RenameDialog::OKCallback_real( Fl_Widget* w ) {
	cancelled = false;
	hide();
}

void RenameDialog::CancelCallback_real( Fl_Widget* w ) {
	cancelled = true;
	hide();
}