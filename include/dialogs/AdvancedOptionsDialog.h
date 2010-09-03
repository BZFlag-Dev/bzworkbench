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

#ifndef ADVANCEDOPTIONSDIALOG_H_
#define ADVANCEDOPTIONSDIALOG_H_

#include "Fl_Dialog.h"
#include "ConfigurationDialog.h"

#include "widgets/QuickLabel.h"
#include "widgets/MaterialWidget.h"
#include "defines.h"

#include "model/Model.h"

#include "objects/material.h"

#include <FL/Fl.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Choice.H>
#include <FL/Fl_Check_Button.H>
#include <FL/Fl_Scroll.H>
#include <FL/Fl_Pack.H>
#include <FL/Fl_Tabs.H>

#include <vector>
#include <string>
#include <map>

using namespace std;

/**
 * This is the dialog that opens when the user clicks "Advanced..." on the Master Configuration Dialog
 */
 
class AdvancedOptionsDialog : public ConfigurationDialog {

public:
	class AdvancedOptionsPage : public Fl_Group {
	
	public:
		AdvancedOptionsPage( int x, int y, std::string name, std::vector<material*> mats, physics* phys, bool usephydrv );

		// add material callback
		static void addMaterialCallback( Fl_Widget* w, void* data ) {
			AdvancedOptionsPage* ccd = (AdvancedOptionsPage*)data;
			ccd->addMaterialCallback_real( w );
		}

		static void removeMaterialCallback( Fl_Widget* w, void* data ) {
			AdvancedOptionsPage* obj = (AdvancedOptionsPage*)data;
			obj->removeMaterialCallback_real( w );
		}

		void commitChanges( bz2object* obj );

	private:
		void addMaterialCallback_real( Fl_Widget* w );
		void removeMaterialCallback_real( Fl_Widget* w );

		void addMaterial( MaterialWidget* mw );

		// material widgets
		QuickLabel* materialLabel;
		Fl_Button* materialAdd;
		Fl_Button* materialRemove;
		Fl_Scroll* materialList;
		Fl_Pack* listPack;
		vector< MaterialWidget* > materialWidgets;
	
		// physics driver widgets
		QuickLabel* phydrvLabel;
		Fl_Choice* phydrvMenu;

		vector<string> materialRefs;
		std::string name;
		bool usephydrv;
	};


	static const int DEFAULT_WIDTH = 400;
	static const int DEFAULT_HEIGHT = 350;
	
	// constructor
	AdvancedOptionsDialog( bz2object* obj );
	
	// destructor
	virtual ~AdvancedOptionsDialog() { }
	
	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		AdvancedOptionsDialog* ccd = (AdvancedOptionsDialog*)(data);
		ccd->OKCallback_real( w );
	}
	
	// CANCEL callback
	static void CancelCallback( Fl_Widget* w, void* data ) {
		AdvancedOptionsDialog* ccd = (AdvancedOptionsDialog*)(data);
		ccd->CancelCallback_real( w );
	}
	
	
	
private:

	// real callbacks
	void OKCallback_real( Fl_Widget* w );
	void CancelCallback_real( Fl_Widget* w );
	
	// bz2object reference
	// (duplicated from ConfigurationDialog::object for type safety)
	bz2object* obj;

	Fl_Tabs* tabs;

	vector< AdvancedOptionsPage* > tabPages;
	
};

#endif /*ADVANCEDOPTIONSDIALOG_H_*/
