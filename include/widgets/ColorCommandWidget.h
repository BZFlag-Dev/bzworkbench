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

#ifndef COLORCOMMANDWIDGET_H_
#define COLORCOMMANDWIDGET_H_

#include <list>

#include "ColorCommand.h"
#include "dialogs/Fl_Dialog.h"
#include "widgets/QuickLabel.h"

#include "FL/Fl_Choice.H"
#include "FL/Fl_Scroll.H"
#include "FL/Fl_Value_Input.H"

class ColorCommandWidget : public Fl_Group {

public:
	class SequenceEditor : public Fl_Dialog {

	public:
		SequenceEditor( vector<int> seq );

		// OK callback
		static void OKCallback( Fl_Widget* w, void* data ) {
			SequenceEditor* ccd = (SequenceEditor*)(data);
			ccd->OKCallback_real( w );
		}

		static void AddCallback( Fl_Widget* w, void* data ) {
			SequenceEditor* ccd = (SequenceEditor*)(data);
			ccd->AddCallback_real( w );
		}

		static void RemoveCallback( Fl_Widget* w, void* data ) {
			SequenceEditor* ccd = (SequenceEditor*)(data);
			ccd->RemoveCallback_real( w );
		}

		vector<int> getSequence();

	private:

		// real callbacks
		void OKCallback_real( Fl_Widget* w );
		void RemoveCallback_real( Fl_Widget* w );
		void AddCallback_real( Fl_Widget* w );


		void addToSequence( int type );

		std::list< Fl_Choice* > sequence;
		Fl_Button* addButton;
		Fl_Button* removeButton;
		Fl_Scroll* seqArea;
	};

	ColorCommandWidget( const ColorCommandWidget& mat );
	ColorCommandWidget( int x, int y, std::string color = "red", ColorCommand cmd = ColorCommand() );

	static void TypeChangedCallback( Fl_Widget* w, void* data ) {
		ColorCommandWidget* ccd = (ColorCommandWidget*)(data);
		ccd->TypeChangedCallback_real( w );
	}

	static void EditCallback( Fl_Widget* w, void* data ) {
		ColorCommandWidget* ccd = (ColorCommandWidget*)(data);
		ccd->EditCallback_real( w );
	}

	ColorCommand getEditedCommand();
	std::string getColor();

private:
	void TypeChangedCallback_real( Fl_Widget* w );
	void EditCallback_real( Fl_Widget* w );

	void switchCommandType( std::string type );

	Fl_Choice* colorChoice;
	Fl_Choice* commandChoice;

	QuickLabel* firstParamLabel;
	Fl_Value_Input* firstParamInput;
	QuickLabel* secondParamLabel;
	Fl_Value_Input* secondParamInput;
	QuickLabel* thirdParamLabel;
	Fl_Value_Input* thirdParamInput;

	Fl_Button* editSequence;

	SequenceEditor* seqEditor;
};


#endif /* COLORCOMMANDWIDGET_H_ */
