/*
 * Copyright 2020 Alwin Leerling <dna.leerling@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 */

#include "karnaughapp.h"

#include "karnaughwindow.h"
#include "karnaughconfig.h"

IMPLEMENT_APP( KarnaughApp )

bool KarnaughApp::OnInit()
{
	config = new KarnaughConfig( *this );
	data = new KarnaughData( config->GetInputs() );

	CreateGUI();

    return true;
}

int KarnaughApp::OnExit()
{
	delete config;
	delete data;

	return 0;
}

void KarnaughApp::CreateGUI()
{
	wxWindow * topwindow = GetTopWindow();

	if( topwindow ) {
		SetTopWindow( NULL );
		topwindow->Destroy();
	}

    frame = new KarnaughWindow( *this );

	frame->SetNewShowAddress( config->GetShowAddress() );
    frame->SetNewShowZeroes( config->GetShowZeroes() );

    unsigned int no_of_inputs = config->GetInputs();

	frame->SetInputs( config->GetSolutionType() == KarnaughData::SOP, no_of_inputs );

    for( int row = 0; row < (1 << (no_of_inputs / 2)); ++row )
		frame->SetGridLabel( row, data->index_to_greycode_string( row, no_of_inputs / 2 ), true );

    for( int col = 0; col < (1 << ((no_of_inputs + 1) / 2)); ++col )
		frame->SetGridLabel( col, data->index_to_greycode_string( col, (no_of_inputs + 1) / 2 ), false );

    frame->SetNewSolutionType( config->GetSolutionType() == KarnaughData::SOP );

    for( int index = 0; index < (1 << no_of_inputs ); ++index )
		frame->SetNewValue( index, data->calc_address(index), data->get_value(index) );

    SetTopWindow( frame );
    frame->Show();
}

void KarnaughApp::SelectLanguage( )
{
	long index = frame->GetLanguageChoice( config->GetLanguages() );

	if( index == -1 )
		return;

	config->SetNewLocale( index );
	CreateGUI();
}

void KarnaughApp::SetNewValue( unsigned int address, KarnaughData::eCellValues new_value )
{
	data->set_value( address, new_value );

	frame->SetNewValue( address, data->calc_address( address ), new_value );

	RunSolver();
}

void KarnaughApp::SetNewValue( int row, int col, KarnaughData::eCellValues new_value )
{
	SetNewValue( data->calc_address( row, col ), new_value );
}

void KarnaughApp::SetInputs( unsigned int no_of_inputs )
{
	config->SetInputs( no_of_inputs );
	data->set_dimension( no_of_inputs );

	frame->SetInputs( data->get_solution_type() == KarnaughData::SOP, no_of_inputs );

    for( int row = 0; row < (1 << (no_of_inputs / 2)); ++row )
		frame->SetGridLabel( row, data->index_to_greycode_string( row, no_of_inputs / 2 ), true );

    for( int col = 0; col < (1 << ((no_of_inputs + 1) / 2)); ++col )
		frame->SetGridLabel( col, data->index_to_greycode_string( col, (no_of_inputs + 1) / 2 ), false );

	RunSolver();
}

void KarnaughApp::SetNewSolutionType( KarnaughData::eSolutionType type )
{
	config->SetSolutionType( type );
	data->set_solution_type( type );

	frame->SetNewSolutionType( type == KarnaughData::SOP );

	RunSolver();
}

void KarnaughApp::SetNewShowAddress( bool on )
{
	config->SetShowAddress( on );

	frame->SetNewShowAddress( on );
}

void KarnaughApp::SetNewShowZeroes( bool on )
{
	config->SetShowZeroes( on );

	frame->SetNewShowZeroes( on );
}

void KarnaughApp::RunSolver( )
{
	frame->PreSolver( );

    SolutionEntries solutions = data->find_best_solution();

	frame->PostSolverStart( data->get_solution_type() == KarnaughData::SOP, solutions.size() );

    unsigned int id = 0;
	for( SolutionEntry& entry : solutions ) {
		frame->PostSolverAdd( entry, data->get_solution_type() == KarnaughData::SOP, data->get_entry_addresses(entry), id );
		++id;
	}

	frame->PostSolverFinish();
}

void KarnaughApp::SetSolutionSelection( unsigned int index )
{
	frame->SetSolutionSelection( data->get_entry_addresses( index ) );
}
