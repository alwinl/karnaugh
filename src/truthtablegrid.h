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

#ifndef TRUTHTABLEGRID_H
#define TRUTHTABLEGRID_H

#include <wx/wx.h>
#include <wx/grid.h>

#include "karnaughdata.h"

class TruthTableGridCellRenderer;

class TruthTableGrid : public wxGrid
{
public:
    TruthTableGrid( wxWindow* parent, wxWindowID id, const wxSize& size );

    void SetVars( int vars );
    void SetShowZeros( bool s );

    void SetValue( unsigned int index, KarnaughData::eCellValues );
    KarnaughData::eCellValues GetUserInput( wxGridEvent& event );

private:
	enum eMenuIds { MENU_SET1 = 100, MENU_SET0, MENU_SETDC, MENU_SETRAND };

    void DisplayPopup( wxGridEvent& event );
	void OnMenuRange( wxCommandEvent& event );

    wxMenu* stateMenu;
    TruthTableGridCellRenderer * renderer;

	void SetValue( unsigned int row, unsigned int col, KarnaughData::eCellValues value );

    DECLARE_EVENT_TABLE()
};

#endif // TRUTHTABLEGRID_H
