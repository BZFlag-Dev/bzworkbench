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

#ifndef RENAMEDIALOG_H_
#define RENAMEDIALOG_H_

#include "Fl_Dialog.h"

#include "FL/Fl_Input.H"

#include "model/Model.h"
#include "widgets/QuickLabel.h"
#include "windows/MainWindow.h"

class RenameDialog : public Fl_Dialog {

public:

	// constructor
	RenameDialog();

	// destructor
	virtual ~RenameDialog() { }

	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		RenameDialog* ccd = (RenameDialog*)(data);
		ccd->OKCallback_real( w );
	}

	// Cancel callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		RenameDialog* ccd = (RenameDialog*)(data);
		ccd->CancelCallback_real( w );
	}

	void setName( std::string value ) { nameInput->value( value.c_str() ); }
	std::string getName() { return std::string( nameInput->value() ); }

	bool getCancelled() { return cancelled; }

protected:

	// name size
	QuickLabel* nameLabel;
	Fl_Input* nameInput;

	bool cancelled;

private:
	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );

};

#endif