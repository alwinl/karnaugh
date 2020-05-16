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

#ifndef _KMAP_H_
#define _KMAP_H_

#include <vector>
#include <string>

#include <wx/wx.h>
#include <wx/grid.h>

#include "karnaughdata.h"

class KMapGridCellRenderer;

/**
 * @short Karnaugh Map grid control
 * @author Robert Kovacevic <robert.kovacevic@etfos.hr>
 * @version 0.1
 */
class KMapGrid : public wxGrid
{
public:
    KMapGrid( wxWindow* parent, wxWindowID id, const wxSize& size = wxDefaultSize );

    void SetVars( KarnaughData& data, unsigned int vars );
    void SetShowZeros( bool on );
    void SetCellAdresses( bool on );

    void SetValue( unsigned int row, unsigned int col, KarnaughData::eCellValues value );
	KarnaughData::eCellValues GetUserInput( wxGridEvent& event );

    void ResetBackgroundColour( KarnaughData::eSolutionType type, unsigned int solution_size );
	void SetBackgroundColour( KarnaughData& data, SolutionEntry& entry );

	void ResetSelection();
	void AddCellToSelection( unsigned int row, unsigned int col );

	void RowLabels( std::vector<std::string> labels );
	void ColLabels( std::vector<std::string> labels );

private:
	enum eMenuIds { MENU_SET1 = 100, MENU_SET0, MENU_SETDC, MENU_SETRAND };

    void DisplayPopup( wxGridEvent& event );
    void OnMenuRange( wxCommandEvent& event );

    wxMenu* stateMenu;
    KMapGridCellRenderer * renderer;

    DECLARE_EVENT_TABLE()
};


#endif // _KMAP_H_
