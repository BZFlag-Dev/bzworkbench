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

#ifndef FL_ERROR_H_
#define FL_ERROR_H_

#include <FL/Fl.H>
#include <FL/Fl_Multiline_Output.H>
#include <FL/Fl_XPM_Image.H>

#include "Fl_Dialog.h"
#include "../widgets/QuickLabel.h"

class Fl_Error : public Fl_Dialog {

public:
	// build the error box
	Fl_Error(const char* message);
	
	virtual ~Fl_Error();
	
	// set error text
	void setText(const char* message);
	
	// close callback
	static void Fl_ErrorCallback(Fl_Widget* w, void* data) {
		Fl_Error* dialog = (Fl_Error*)(data);
		dialog->Fl_ErrorCallback_real(w);	
	}
	
private:

	// default callback
	void Fl_ErrorCallback_real(Fl_Widget* w);
	
	// error message
	Fl_Multiline_Output* errorMessage;
	
	// error icon
	Fl_XPM_Image* errorImage;
	
	// error label
	QuickLabel* errorLabel;
	
};

#endif /*FL_ERROR_H_*/
