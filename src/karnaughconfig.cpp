/*
 * Copyright 2019 Alwin Leerling <dna.leerling@gmail.com>
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
 *
 *
 */

#include "karnaughconfig.h"

#include "blamapp.h"

KarnaughConfig::KarnaughConfig() : config( wxGetApp().GetAppName() )
{
}

/*
KarnaughConfig::eLANGUAGES KarnaughConfig::GetLanguage( )
{
    wxString lang;

	if( !config.Read( wxT( "Language" ), &lang ) ) {
		switch( wxLocale::GetSystemLanguage() ) {
		case wxLANGUAGE_CROATIAN : SetLanguage( CROATIAN ); return CROATIAN;
		case wxLANGUAGE_DUTCH : SetLanguage( DUTCH ); return DUTCH;
		default: SetLanguage( DEFAULT ); return DEFAULT;
		}
	}

	if( lang == wxT( "hr" ) ) return CROATIAN;
	if( lang == wxT( "nl" ) ) return DUTCH;

	return DEFAULT;
}


void KarnaughConfig::SetLanguage( KarnaughConfig::eLANGUAGES lang )
{
	switch( lang ) {
	case CROATIAN : config.Write( wxT( "Language" ), wxT("hr") ); break;
	case DUTCH : config.Write( wxT( "Language" ), wxT("nl") ); break;
	default:		 config.Write( wxT( "Language" ), wxT("") ); break;
	}

	config.Flush();
}
*/

bool KarnaughConfig::GetShowZeroes()
{
	wxString ShowZeros;

    if( ! config.Read( wxT( "Show_Zeros" ), &ShowZeros ) )
		return false;

	return (ShowZeros == wxT("yes")) ? true : false;
}

void KarnaughConfig::SetShowZeroes( bool on )
{
	config.Write( wxT( "Show_Zeros" ), on ? wxT( "yes" ) : wxT( "no" ) );
	config.Flush();
}

bool KarnaughConfig::GetShowAddress()
{
	wxString ShowAddress;

    if( ! config.Read( wxT( "Cell_Adresses" ), &ShowAddress ) )
		return false;

	return (ShowAddress == wxT("yes")) ? true : false;
}

void KarnaughConfig::SetShowAddress( bool on )
{
	config.Write( wxT( "Cell_Adresses" ), on ? wxT( "yes" ) : wxT( "no" ) );
	config.Flush();
}

int KarnaughConfig::GetInputs()
{
	int inputs;

    if( ! config.Read( wxT( "Inputs" ), &inputs ) )
		return 4;

	return inputs;
}

void KarnaughConfig::SetInputs( int inputs )
{
	config.Write( wxT( "Inputs" ), inputs );
	config.Flush();
}

KarnaughData::eSolutionType KarnaughConfig::GetSolutionType()
{
	wxString type;

	if( !config.Read( wxT( "SolutionType" ), &type ) ) {
		SetSolutionType( KarnaughData::SOP );
		return KarnaughData::SOP;
	}

	return (type == wxT("SOP") ? KarnaughData::SOP : KarnaughData::POS );
}

void KarnaughConfig::SetSolutionType( KarnaughData::eSolutionType type )
{
	config.Write( wxT( "SolutionType" ), (type == KarnaughData::SOP) ? wxT("SOP") : wxT("POS") );
	config.Flush();
}


