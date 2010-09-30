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

#include "widgets/ColorCommandWidget.h"
#include "defines.h"

ColorCommandWidget::ColorCommandWidget( const ColorCommandWidget& mat ) : Fl_Group( 0, 0, 0, 0 ) { end(); }

ColorCommandWidget::SequenceEditor::SequenceEditor( vector<int> seq ) :
	Fl_Dialog( "Sequence Editor", 200, 300, Fl_Dialog::Fl_OK ) {

	end();

	addButton = new Fl_Button( 5, 235, 60, DEFAULT_TEXTSIZE + 6, "Add" );
	addButton->callback( AddCallback, this );
	removeButton = new Fl_Button( 70, 235, 60, DEFAULT_TEXTSIZE + 6, "Remove" );
	removeButton->callback( RemoveCallback, this );
	seqArea = new Fl_Scroll( 5, 5, 190, 225 );
	seqArea->end();

	add(addButton);
	add(removeButton);
	add(seqArea);

	vector<int>::iterator i;
	for ( i = seq.begin(); i != seq.end(); i++ ) {
		addToSequence( (int)(*i) );
	}

	setOKEventHandler( OKCallback, this );
}

vector<int> ColorCommandWidget::SequenceEditor::getSequence() {
	vector<int> params;

	list<Fl_Choice*>::iterator i;
	for ( i = sequence.begin(); i != sequence.end(); i++ ) {
		params.push_back( (*i)->value() - 1 );
	}

	return params;
}

void ColorCommandWidget::SequenceEditor::addToSequence( int type ) {
	Fl_Choice* sequenceItem = new Fl_Choice(
			seqArea->x() + 5,
			seqArea->y() + 5 + (DEFAULT_TEXTSIZE + 6) * sequence.size(),
			150, DEFAULT_TEXTSIZE + 6);

	sequenceItem->add( "active clampDown" );
	sequenceItem->add( "no clamps" );
	sequenceItem->add( "active clampUp" );

	sequenceItem->value( type + 1 );

	sequence.push_back( sequenceItem );
	seqArea->add( sequenceItem );
	seqArea->redraw();
}

void ColorCommandWidget::SequenceEditor::OKCallback_real( Fl_Widget* w ) {
	hide();
}

void ColorCommandWidget::SequenceEditor::RemoveCallback_real( Fl_Widget* w ) {
	if ( sequence.size() > 1 ) {
		list< Fl_Choice* >::reverse_iterator last = sequence.rbegin();
		seqArea->remove( *last );
		sequence.remove( *last );
		seqArea->redraw();
	}
}

void ColorCommandWidget::SequenceEditor::AddCallback_real( Fl_Widget* w ) {
	addToSequence( 0 );
}

ColorCommandWidget::ColorCommandWidget( int x, int y, std::string color, ColorCommand cmd ) :
	Fl_Group( x, y, 505, DEFAULT_TEXTSIZE*2 + 6 ) {
	colorChoice = new Fl_Choice( x, y + DEFAULT_TEXTSIZE, 60, DEFAULT_TEXTSIZE + 6 );
	colorChoice->add( "red" );
	colorChoice->add( "green" );
	colorChoice->add( "blue" );
	colorChoice->add( "alpha" );
	if ( color == "red" ) colorChoice->value( 0 );
	else if ( color == "green" ) colorChoice->value( 1 );
	else if ( color == "blue" ) colorChoice->value( 2 );
	else if ( color == "alpha" ) colorChoice->value( 3 );
	else colorChoice->value( 1 );
	commandChoice = new Fl_Choice( x + 65, y + DEFAULT_TEXTSIZE, 60, DEFAULT_TEXTSIZE + 6 );
	commandChoice->add( "limits", 0, TypeChangedCallback, this );
	commandChoice->add( "sinusoid", 0, TypeChangedCallback, this );
	commandChoice->add( "clampUp", 0, TypeChangedCallback, this );
	commandChoice->add( "clampDown", 0, TypeChangedCallback, this );
	commandChoice->add( "sequence", 0, TypeChangedCallback, this );
	string type = cmd.getName();

	firstParamLabel = new QuickLabel( "", x + 135, y);
	firstParamInput = new Fl_Value_Input( x + 135, y + DEFAULT_TEXTSIZE, 120, DEFAULT_TEXTSIZE + 6 );
	secondParamLabel = new QuickLabel( "", x + 260, y);
	secondParamInput = new Fl_Value_Input( x + 260, y + DEFAULT_TEXTSIZE, 120, DEFAULT_TEXTSIZE + 6 );
	thirdParamLabel = new QuickLabel( "", x + 385, y);
	thirdParamInput = new Fl_Value_Input( x + 385, y + DEFAULT_TEXTSIZE, 120, DEFAULT_TEXTSIZE + 6 );

	editSequence = new Fl_Button( x + 385, y + DEFAULT_TEXTSIZE, 70, DEFAULT_TEXTSIZE + 6, "Edit" );
	editSequence->callback( EditCallback, this );

	end();

	if ( type == "limits" ) commandChoice->value( 0 );
	else if ( type == "sinusoid" ) commandChoice->value( 1 );
	else if ( type == "clampUp" ) commandChoice->value( 2 );
	else if ( type == "clampDown" ) commandChoice->value( 3 );
	else if ( type == "sequence" ) commandChoice->value( 4 );
	else commandChoice->value( 1 );

	vector<float> values = cmd.getCommands();
	if (type == "limits") {
		firstParamInput->value( values[0] );
		secondParamInput->value( values[1] );
	}
	else if (type == "sinusoid" || type == "clampUp" || type == "clampDown") {
		firstParamInput->value( values[0] );
		secondParamInput->value( values[1] );
		thirdParamInput->value( values[2] );
	}
	else if (type == "sequence") {
		firstParamInput->value( values[0] );
		secondParamInput->value( values[1] );
	}
	else {
		firstParamInput->value( values[0] );
		secondParamInput->value( values[1] );
	}

	switchCommandType( type );

	// setup sequence editor
	vector<int> seq;
	if (type == "sequence") {
		for (vector<float>::iterator i = values.begin() + 2; i != values.end(); i++ ) {
			seq.push_back( (int)(*i) );
		}
	}
	else {
		seq.push_back( 0 );
	}

	seqEditor = new SequenceEditor( seq );
}

ColorCommand ColorCommandWidget::getEditedCommand() {
	ColorCommand cmd;

	vector<float> params;
	string type( commandChoice->text() );
	if (type == "limits") {
		params.push_back( firstParamInput->value() );
		params.push_back( secondParamInput->value() );
	}
	else if (type == "sinusoid" || type == "clampUp" || type == "clampDown") {
		params.push_back( firstParamInput->value() );
		params.push_back( secondParamInput->value() );
		params.push_back( thirdParamInput->value() );
	}
	else if (type == "sequence") {
		params.push_back( firstParamInput->value() );
		params.push_back( secondParamInput->value() );
		vector<int> seq = seqEditor->getSequence();
		for ( vector<int>::iterator i = seq.begin(); i != seq.end(); i++ ) {
			params.push_back( *i );
		}
	}

	cmd.setCommands( params );
	cmd.setName( type );

	return cmd;
}

string ColorCommandWidget::getColor() {
	return string( colorChoice->text() );

}

void ColorCommandWidget::TypeChangedCallback_real( Fl_Widget* w ) {
	switchCommandType( string( commandChoice->text() ) );
}

void ColorCommandWidget::EditCallback_real( Fl_Widget* w ) {
	seqEditor->show();

	while( seqEditor->shown() ) { Fl::wait(); }
}

void ColorCommandWidget::switchCommandType( std::string type ) {
	if (type == "limits") {
		firstParamLabel->label( "min limit" );
		firstParamInput->show();
		secondParamLabel->label( "max limit" );
		secondParamInput->show();
		thirdParamLabel->label( "" );
		thirdParamInput->hide();
		editSequence->hide();
	}
	else if (type == "sinusoid") {
		firstParamLabel->label( "period" );
		firstParamInput->show();
		secondParamLabel->label( "offset" );
		secondParamInput->show();
		thirdParamLabel->label( "weight" );
		thirdParamInput->show();
		editSequence->hide();
	}
	else if (type == "clampUp" || type == "clampDown") {
		firstParamLabel->label( "period" );
		firstParamInput->show();
		secondParamLabel->label( "offset" );
		secondParamInput->show();
		thirdParamLabel->label( "width" );
		thirdParamInput->show();
		editSequence->hide();
	}
	else if (type == "sequence") {
		firstParamLabel->label( "period" );
		firstParamInput->show();
		secondParamLabel->label( "offset" );
		secondParamInput->show();
		thirdParamLabel->label( "" );
		thirdParamInput->hide();
		editSequence->show();
	}
	redraw();
}
