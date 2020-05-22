/***************************************************************************
 *   Copyright (C) 2005 by Robert Kovacevic                                *
 *   robert.kovacevic@etfos.hr                                             *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <wx/wx.h>
#include <wx/config.h>

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

    frame = new KarnaughWindow( *this, *data );

	frame->SetNewShowAddress( config->GetShowAddress() );
    frame->SetNewShowZeroes( config->GetShowZeroes() );
    frame->SetInputs( config->GetInputs() );
    frame->SetNewSolutionType( config->GetSolutionType() );

    for( int index = 0; index < (1 << config->GetInputs() ); ++index )
		frame->SetNewValue( index, data->get_value(index) );

	frame->RunSolver( config->GetSolutionType() == KarnaughData::SOP );

    SetTopWindow( frame );
    frame->Show();
}

void KarnaughApp::SelectLanguage( )
{
    if( config->AskUserForLanguage( GetTopWindow() ) )
		CreateGUI();
}

void KarnaughApp::SetNewValue( unsigned int address, KarnaughData::eCellValues new_value )
{
	data->set_value( address, new_value );

	frame->SetNewValue( address, new_value );
	frame->RunSolver( data->get_solution_type() == KarnaughData::SOP );
}

void KarnaughApp::SetInputs( unsigned int no_of_inputs )
{
	config->SetInputs( no_of_inputs );
	data->set_dimension( no_of_inputs );

	frame->SetInputs( no_of_inputs );
	frame->RunSolver( data->get_solution_type() == KarnaughData::SOP );
}

void KarnaughApp::SetNewSolutionType( KarnaughData::eSolutionType type )
{
	config->SetSolutionType( type );
	data->set_solution_type( type );

	frame->SetNewSolutionType( type == KarnaughData::SOP );
	frame->RunSolver( type == KarnaughData::SOP );
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

void KarnaughApp::SetSolutionSelection( unsigned int index )
{
	frame->SetSolutionSelection( data->GetEntryAddresses( index ) );
}

