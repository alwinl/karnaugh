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

#include "karnaughconfig.h"

#include <algorithm>

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/config.h>
#include <wx/intl.h>

#include "karnaughapp.h"

KarnaughConfig::KarnaughConfig( wxApp& app ) : m_app(app), m_locale(nullptr), config( app.GetAppName() )
{
	GetInstalledLanguages();
	GetLanguage();
}

bool KarnaughConfig::GetShowZeroes()
{
	wxString ShowZeros;

    if( ! config.Read( "Show_Zeros", &ShowZeros ) )
		return false;

	return (ShowZeros == "yes") ? true : false;
}

void KarnaughConfig::SetShowZeroes( bool on )
{
	config.Write( "Show_Zeros", on ? "yes" : "no" );
	config.Flush();
}

bool KarnaughConfig::GetShowAddress()
{
	wxString ShowAddress;

    if( ! config.Read( "Cell_Adresses", &ShowAddress ) )
		return false;

	return (ShowAddress == "yes") ? true : false;
}

void KarnaughConfig::SetShowAddress( bool on )
{
	config.Write( "Cell_Adresses", on ? "yes" : "no" );
	config.Flush();
}

int KarnaughConfig::GetInputs()
{
	int inputs;

    if( ! config.Read( "Inputs", &inputs ) )
		return 4;

	return inputs;
}

void KarnaughConfig::SetInputs( int inputs )
{
	config.Write( "Inputs", inputs );
	config.Flush();
}

KarnaughData::eSolutionType KarnaughConfig::GetSolutionType()
{
	wxString type;

	if( !config.Read( "SolutionType", &type ) ) {
		SetSolutionType( KarnaughData::SOP );
		return KarnaughData::SOP;
	}

	return (type == "SOP") ? KarnaughData::SOP : KarnaughData::POS;
}

void KarnaughConfig::SetSolutionType( KarnaughData::eSolutionType type )
{
	config.Write( "SolutionType", (type == KarnaughData::SOP) ? "SOP" : "POS" );
	config.Flush();
}

void KarnaughConfig::GetInstalledLanguages( )
{
    wxString name = wxLocale::GetLanguageName( wxLANGUAGE_DEFAULT );
    if( !name.IsEmpty() )
		languages.push_back( LanguageEntry( { .id = wxLANGUAGE_DEFAULT, .name = "Default" } ) );

    wxDir dir( wxPathOnly( m_app.argv[0] ) );
    wxString filename;

    for( bool cont = dir.GetFirst( &filename, wxEmptyString, wxDIR_DIRS ); cont; cont = dir.GetNext( &filename ) ) {

		const wxLanguageInfo * langinfo = wxLocale::FindLanguageInfo( filename );
        if( langinfo != NULL ) {
            if( wxFileExists( dir.GetName() + wxFileName::GetPathSeparator() + filename
												+ wxFileName::GetPathSeparator() + m_app.GetAppName() + ".mo" ) ) {

				LanguageEntry entry;
				entry.id = langinfo->Language;
				entry.name = langinfo->Description;
				languages.push_back( entry );
            }
        }
    }
}

void KarnaughConfig::SetNewLocale( LanguageEntry entry )
{
	wxDELETE( m_locale );

	m_locale = new wxLocale;

	m_locale->Init( entry.id );
	m_locale->AddCatalogLookupPathPrefix( wxPathOnly( m_app.argv[0] ) );
	m_locale->AddCatalog( m_app.GetAppName() );

	SetLanguage( false );
}

void KarnaughConfig::SetNewLocale( long index )
{
	SetNewLocale( languages[index] );
}


wxArrayString KarnaughConfig::GetLanguages( )
{
    wxArrayString names;
    for( LanguageEntry& entry : languages )
		names.Add( entry.name );

	return names;
}

bool KarnaughConfig::GetLanguage()
{
	long language;

	if( !config.Read( "Language", &language, wxLANGUAGE_UNKNOWN ) )
		return false;

	if( language == wxLANGUAGE_UNKNOWN )
		return false;

	std::vector<LanguageEntry>::iterator lang_entry = std::find_if( languages.begin(), languages.end(), [language](const LanguageEntry& entry){ return (entry.id == language); } );

	if( lang_entry == languages.end() )
		return false;

	SetNewLocale( *lang_entry );
	return true;
}

void KarnaughConfig::SetLanguage( bool bReset )
{
    long language = wxLANGUAGE_UNKNOWN;

	if( !bReset && m_locale )
		language = m_locale->GetLanguage();

	config.Write( "Language", language );
	config.Flush();
}
