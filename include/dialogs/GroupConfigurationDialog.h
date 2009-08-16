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

#ifndef GROUPCONFIGURATIONDIALOG_H_
#define GROUPCONFIGURATIONDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Choice.H>

#include "ConfigurationDialog.h"
#include "widgets/QuickLabel.h"
#include "defines.h"

#include "objects/group.h"
#include "objects/define.h"

class GroupConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 300;
	static const int DEFAULT_HEIGHT = 100;

	// constructor
	GroupConfigurationDialog( group* theGroup );

	// static constructor
	static ConfigurationDialog* init( DataEntry* d ) {
		group* c = dynamic_cast< group* > (d);
		if( c )
			return new GroupConfigurationDialog( dynamic_cast< group* >( d ) );
		else
			return NULL;
	}

	// destructor
	virtual ~GroupConfigurationDialog() { }

	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		GroupConfigurationDialog* ccd = (GroupConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}

	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		GroupConfigurationDialog* ccd = (GroupConfigurationDialog*)(data);
		ccd->CancelCallback_real( w );
	}

private:

	// the cone
	group* theGroup;

	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );

	QuickLabel* defineLabel;
	Fl_Choice* defineChoice;
};

#endif /*PYRAMIDCONFIGURATIONDIALOG_H_*/
