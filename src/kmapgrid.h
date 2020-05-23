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

#ifndef KMAPGRID_H
#define KMAPGRID_H

#include <wx/wx.h>
#include <wx/grid.h>

#include "karnaughdata.h"

class KMapGridCellRenderer;

class KMapGrid : public wxGrid
{
public:
    KMapGrid( wxWindow* parent, wxWindowID id, const wxSize& size );

    void SetVars( unsigned int vars );
	void SetLabel( int index, wxString label, bool isRow );
    void SetShowZeros( bool on );
    void SetCellAdresses( bool on );

    void SetValue( unsigned int row, unsigned int col, KarnaughData::eCellValues value );
	KarnaughData::eCellValues GetUserInput( wxGridEvent& event );

    void ResetBackgroundColour( bool isSOP, unsigned int solution_size );
	void SetBackgroundColour( bool isSOP, GridAddresses addresses );

	void ResetSelection();
	void AddCellToSelection( unsigned int row, unsigned int col );

private:
	enum eMenuIds { MENU_SET1 = 100, MENU_SET0, MENU_SETDC, MENU_SETRAND };

    void DisplayPopup( wxGridEvent& event );
    void OnMenuRange( wxCommandEvent& event );

    wxMenu* mnuPopup;
    KMapGridCellRenderer * renderer;

    DECLARE_EVENT_TABLE()
};

#endif // KMAPGRID_H
