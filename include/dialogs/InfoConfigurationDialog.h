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

#ifndef INFOCONFIGURATIONDIALOG_H_
#define INFOCONFIGURATIONDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Multiline_Input.H>

#include "ConfigurationDialog.h"
#include "widgets/QuickLabel.h"
#include "defines.h"

#include "objects/info.h"

class InfoConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 300;
	static const int DEFAULT_HEIGHT = 300;

	// constructor
	InfoConfigurationDialog( info* theInfo );

	// static constructor
	static ConfigurationDialog* init( DataEntry* d ) {
		info* c = dynamic_cast< info* > (d);
		if( c )
			return new InfoConfigurationDialog( dynamic_cast< info* >( d ) );
		else
			return NULL;
	}

	// destructor
	virtual ~InfoConfigurationDialog() { }

	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		InfoConfigurationDialog* ccd = (InfoConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}

	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		InfoConfigurationDialog* ccd = (InfoConfigurationDialog*)(data);
		ccd->CancelCallback_real( w );
	}

private:

	// the cone
	info* theInfo;

	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );

	// info text label
	QuickLabel* infoTextLabel;

	// info text field
	Fl_Multiline_Input* infoTextInput;

};

#endif /* INFOCONFIGURATIONDIALOG_H_ */
