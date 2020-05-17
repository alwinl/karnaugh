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

#include "translationhelper.h"

#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/config.h>
#include <wx/intl.h>

TranslationHelper::TranslationHelper( wxApp& app ) : m_app( app ), m_locale( nullptr )
{
	GetInstalledLanguages();
    Load();
}

TranslationHelper::~TranslationHelper()
{
    Save();
    delete m_locale;
}

void TranslationHelper::GetInstalledLanguages( )
{
    wxString name = wxLocale::GetLanguageName( wxLANGUAGE_DEFAULT );
    if( !name.IsEmpty() )
		languages.push_back( LanguageEntry( { .id = wxLANGUAGE_DEFAULT, .name = "Default" } ) );

    wxDir dir( wxPathOnly( m_app.argv[0] ) );
    wxString filename;

    for( bool cont = dir.GetFirst( &filename, wxEmptyString, wxDIR_DIRS ); cont; cont = dir.GetNext( &filename ) ) {

        wxLogTrace( wxTraceMask(), _( "TranslationHelper: Directory found = \"%s\"" ), filename.GetData() );

		const wxLanguageInfo * langinfo = wxLocale::FindLanguageInfo( filename );
        if( langinfo != NULL ) {
            if( wxFileExists( dir.GetName() + wxFileName::GetPathSeparator() + filename
												+ wxFileName::GetPathSeparator() + m_app.GetAppName() + wxT( ".mo" ) ) ) {

				LanguageEntry entry;
				entry.id = langinfo->Language;
				entry.name = langinfo->Description;
				languages.push_back( entry );
            }
        }
    }
}

void TranslationHelper::set_new_locale( LanguageEntry entry )
{
	wxDELETE( m_locale );

	m_locale = new wxLocale;

	m_locale->Init( entry.id );
	m_locale->AddCatalogLookupPathPrefix( wxPathOnly( m_app.argv[0] ) );
	m_locale->AddCatalog( m_app.GetAppName() );

	wxLogTrace( wxTraceMask(), _( "TranslationHelper: Path Prefix = \"%s\"" ), wxPathOnly( m_app.argv[0] ).GetData() );
	wxLogTrace( wxTraceMask(), _( "TranslationHelper: Catalog Name = \"%s\"" ), m_app.GetAppName().GetData() );
	wxLogTrace( wxTraceMask(), _( "TranslationHelper: Setting language to = \"%s\"" ), entry.name );

	Save();
}

bool TranslationHelper::AskUserForLanguage( wxWindow * parent )
{
    wxArrayString names;
    for( LanguageEntry& entry : languages )
		names.Add( entry.name );

    long index = wxGetSingleChoiceIndex( _( "Select the language" ), _( "Language" ), names, parent,
				wxDefaultCoord, wxDefaultCoord, true, 400, 1300 );

    if( index == -1 )
		return false;

	set_new_locale( languages[index] );
	return true;
}

bool TranslationHelper::Load()
{
    wxConfig config( m_app.GetAppName() );
    long language;

    if( !config.Read( wxT( "Language" ), &language, wxLANGUAGE_UNKNOWN ) )
        return false;

    if( language == wxLANGUAGE_UNKNOWN )
        return false;

    for( LanguageEntry& entry : languages )
		if( entry.id == language ) {
			set_new_locale( entry );
			return true;
		}

	return false;
}

void TranslationHelper::Save( bool bReset )
{
    wxConfig config( m_app.GetAppName() );

    long language = wxLANGUAGE_UNKNOWN;

    if( !bReset && m_locale ) {
        language = m_locale->GetLanguage();

        config.Write( wxT( "Language" ), language );
        config.Flush();
    }
}
