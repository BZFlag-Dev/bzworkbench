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

#ifndef MATERIALCONFIGURATIONDIALOG_H_
#define MATERIALCONFIGURATIONDIALOG_H_

#include <FL/Fl.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Value_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Check_Browser.H>
#include <FL/Fl_Multi_Browser.H>
#include <FL/Fl_Choice.H>

#include "ConfigurationDialog.h"
#include "widgets/QuickLabel.h"
#include "defines.h"

#include "objects/material.h"

class MaterialConfigurationDialog : public ConfigurationDialog {

public:

	// dimensions
	static const int DEFAULT_WIDTH = 300;
	static const int DEFAULT_HEIGHT = 550;

	// constructor
	MaterialConfigurationDialog( material* theMat );

	// static constructor
	static ConfigurationDialog* init( DataEntry* d ) {
		material* c = dynamic_cast< material* > (d);
		if( c )
			return new MaterialConfigurationDialog( dynamic_cast< material* >( d ) );
		else
			return NULL;
	}

	// destructor
	virtual ~MaterialConfigurationDialog() { }

	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		MaterialConfigurationDialog* ccd = (MaterialConfigurationDialog*)(data);
		ccd->OKCallback_real( w );
	}

	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		MaterialConfigurationDialog* ccd = (MaterialConfigurationDialog*)(data);
		ccd->CancelCallback_real( w );
	}

private:

	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );

	// the cone
	material* theMat;

	// name label
	QuickLabel* nameLabel;

	// name input
	Fl_Input* nameInput;

	// matref label
	QuickLabel* matrefLabel;

	// matref browser
	Fl_Multi_Browser* matrefBrowser;

	// matref choice
	Fl_Choice* matrefChoice;

	// matref buttons
	Fl_Button* matrefAddButton;
	Fl_Button* matrefRemoveButton;

	// dynamic color label
	QuickLabel* dyncolLabel;

	// dynamic color choice
	Fl_Choice* dyncolChoice;

	// option buttons
	Fl_Check_Button* noTexturesButton;
	Fl_Check_Button* noShadowButton;
	Fl_Check_Button* noCullingButton;
	Fl_Check_Button* noSortingButton;
	Fl_Check_Button* noRadarButton;
	Fl_Check_Button* noLightingButton;
	Fl_Check_Button* groupAlphaButton;
	Fl_Check_Button* occluderButton;

	// alpha threshold label
	QuickLabel* alphaThresholdLabel;

	// alpha threshold input
	Fl_Value_Input* alphaThresholdInput;

	// texture label
	QuickLabel* textureLabel;

	// texture input
	Fl_Input* textureInput;

	// texture matrix label
	QuickLabel* textureMatrixLabel;

	// texture matrix
	Fl_Choice* textureMatrixChoice;

	// texture options
	Fl_Check_Button* noAlphaButton;
	Fl_Check_Button* noColorButton;
	Fl_Check_Button* sphereMapButton;

};

#endif /* MATERIALCONFIGURATIONDIALOG_H_ */
