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

#ifndef MATERIALEDITOR_H_
#define MATERIALEDITOR_H_

#include "Fl_Dialog.h"
#include "FL/Fl_Multi_Browser.H"
#include "FL/Fl_Button.H"

#include "model/Model.h"
#include "widgets/QuickLabel.h"

class MaterialEditor : public Fl_Dialog {

public:

	// constructor
	MaterialEditor( Model* model );

	// destructor
	virtual ~MaterialEditor() { }

	// OK callback
	static void OKCallback( Fl_Widget* w, void* data ) {
		MaterialEditor* ccd = (MaterialEditor*)(data);
		ccd->OKCallback_real( w );
	}

	static void MaterialAddCallback( Fl_Widget* w, void* data ) {
		MaterialEditor* ccd = (MaterialEditor*)(data);
		ccd->MaterialAddCallback_real( w );
	}

	static void MaterialRemoveCallback( Fl_Widget* w, void* data ) {
		MaterialEditor* ccd = (MaterialEditor*)(data);
		ccd->MaterialRemoveCallback_real( w );
	}

	static void MaterialEditCallback( Fl_Widget* w, void* data ) {
		MaterialEditor* ccd = (MaterialEditor*)(data);
		ccd->MaterialEditCallback_real( w );
	}

	static void TexmatAddCallback( Fl_Widget* w, void* data ) {
		MaterialEditor* ccd = (MaterialEditor*)(data);
		ccd->TexmatAddCallback_real( w );
	}

	static void TexmatRemoveCallback( Fl_Widget* w, void* data ) {
		MaterialEditor* ccd = (MaterialEditor*)(data);
		ccd->TexmatRemoveCallback_real( w );
	}

	static void TexmatEditCallback( Fl_Widget* w, void* data ) {
		MaterialEditor* ccd = (MaterialEditor*)(data);
		ccd->TexmatEditCallback_real( w );
	}

	static void DyncolAddCallback( Fl_Widget* w, void* data ) {
		MaterialEditor* ccd = (MaterialEditor*)(data);
		ccd->DyncolAddCallback_real( w );
	}

	static void DyncolRemoveCallback( Fl_Widget* w, void* data ) {
		MaterialEditor* ccd = (MaterialEditor*)(data);
		ccd->DyncolRemoveCallback_real( w );
	}

	static void DyncolEditCallback( Fl_Widget* w, void* data ) {
		MaterialEditor* ccd = (MaterialEditor*)(data);
		ccd->DyncolEditCallback_real( w );
	}

protected:
	Model* model;

	// material label
	QuickLabel* materialLabel;

	// material browser
	Fl_Multi_Browser* materialBrowser;

	// material buttons
	Fl_Button* materialAddButton;
	Fl_Button* materialRemoveButton;
	Fl_Button* materialEditButton;

	// texture matrix label
	QuickLabel* textureMatrixLabel;

	// texture matrix browser
	Fl_Multi_Browser* textureMatrixBrowser;

	// texture matrix buttons
	Fl_Button* textureMatrixAddButton;
	Fl_Button* textureMatrixRemoveButton;
	Fl_Button* textureMatrixEditButton;

	// dynamic color label
	QuickLabel* dyncolLabel;

	// dynamic color browser
	Fl_Multi_Browser* dyncolBrowser;

	// dynamic color buttons
	Fl_Button* dyncolAddButton;
	Fl_Button* dyncolRemoveButton;
	Fl_Button* dyncolEditButton;

private:
	void refreshMaterialList();
	void refreshTexmatList();
	void refreshDyncolList();

	// real callbacks
	void OKCallback_real( Fl_Widget* w );

	void MaterialAddCallback_real( Fl_Widget* w );
	void MaterialRemoveCallback_real( Fl_Widget* w );
	void MaterialEditCallback_real( Fl_Widget* w );

	void TexmatAddCallback_real( Fl_Widget* w );
	void TexmatRemoveCallback_real( Fl_Widget* w );
	void TexmatEditCallback_real( Fl_Widget* w );

	void DyncolAddCallback_real( Fl_Widget* w );
	void DyncolRemoveCallback_real( Fl_Widget* w );
	void DyncolEditCallback_real( Fl_Widget* w );
};

#endif /* MATERIALEDITOR_H_ */
