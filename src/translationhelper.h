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

#ifndef TRANSLATIONHELPER_H
#define TRANSLATIONHELPER_H

#include <wx/wx.h>

#include <vector>

class TranslationHelper
{
public:
    TranslationHelper( wxApp& app );
    ~TranslationHelper();

    wxLocale * GetLocale() { return m_locale; }

    bool AskUserForLanguage( wxWindow * parent );
    bool Load();
    void Save( bool bReset = false );

private:
    struct LanguageEntry {
    	long id;
    	std::string name;
    };

    wxApp& m_app;
    wxLocale * m_locale;
    std::vector<LanguageEntry> languages;

    void GetInstalledLanguages( );

	void set_new_locale( LanguageEntry entry );
};

#endif // TRANSLATIONHELPER_H
