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
#include <wx/grid.h>

#include "truthtablegrid.h"

BEGIN_EVENT_TABLE( TruthTableGrid, wxGrid )
    EVT_GRID_CELL_RIGHT_CLICK( TruthTableGrid::DisplayPopup )
    EVT_MENU_RANGE(MENU_SET1, MENU_SETRAND, TruthTableGrid::OnMenuRange )
END_EVENT_TABLE()


class TruthTableGridCellRenderer : public wxGridCellStringRenderer
{
public:
	TruthTableGridCellRenderer( ) : wxGridCellStringRenderer(), do_draw_zeros( true ) {};

	void draw_zeros( bool on ) { do_draw_zeros = on; };

protected:
    virtual void Draw( wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected );

private:
	bool do_draw_zeros;
};

void TruthTableGridCellRenderer::Draw( wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected )
{
	wxRect newRect = rect;
	newRect.Inflate(-1);

	wxGridCellRenderer::Draw( grid, attr, dc, rect, row, col, isSelected );	// call base class

	SetTextColoursAndFont( grid, attr, dc, isSelected );

	if( do_draw_zeros || grid.GetCellValue(row, col) != wxT("0") )
		grid.DrawTextRectangle( dc, grid.GetCellValue(row, col), newRect, wxALIGN_CENTER );
}




TruthTableGrid::TruthTableGrid( wxWindow* parent, wxWindowID id, const wxSize& size ) : wxGrid( parent, id, wxDefaultPosition, size, wxSIMPLE_BORDER, wxPanelNameStr )
{
    CreateGrid( 1, 1, wxGrid::wxGridSelectRows  );
    SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
    EnableDragGridSize( 0 );

	renderer = new TruthTableGridCellRenderer();
    SetDefaultRenderer( renderer );

    stateMenu = new wxMenu;

    stateMenu->Append( new wxMenuItem( 0, MENU_SET1, _( "Set to 1" ) ) );
    stateMenu->Append( new wxMenuItem( 0, MENU_SET0, _( "Set to 0" ) ) );
    stateMenu->Append( new wxMenuItem( 0, MENU_SETDC, _( "Set to \"don't care\"" ) ) );
    stateMenu->AppendSeparator();
    stateMenu->Append( new wxMenuItem( 0, MENU_SETRAND, _( "Set randomly" ) ) );

    srand( static_cast<unsigned>( time( 0 ) ) );
}

void TruthTableGrid::SetValue( unsigned int row, unsigned int col, KarnaughData::eCellValues value )
{
	switch( value ) {
	case KarnaughData::ZERO : SetCellValue( row, col, wxT("0") ); break;
	case KarnaughData::ONE : SetCellValue( row, col, wxT("1") ); break;
	case KarnaughData::DONTCARE : SetCellValue( row, col, wxT("?") ); break;
	}
}

void TruthTableGrid::SetValue( unsigned int index, KarnaughData::eCellValues new_value )
{
	SetValue( index, GetNumberCols() - 1, new_value );
}

void TruthTableGrid::SetVars( int vars )
{
    ClearGrid();

    int height = 1 << vars;
    int width = vars + 1;

    if( GetNumberCols() < width  ) AppendCols( width - GetNumberCols() );
    if( GetNumberRows() < height ) AppendRows( height - GetNumberRows() );

    if( GetNumberCols() > width  ) DeleteCols( 0, GetNumberCols() - width );
    if( GetNumberRows() > height ) DeleteRows( 0, GetNumberRows() - height );

    for( int col = 0; col < (GetNumberCols() - 1); ++col )
        SetColLabelValue( col, wxString::Format( wxT( "%c" ), 'A' + vars - (col + 1) ) ) ;

    SetColLabelValue( vars, wxT( "X" ) );

    for( int row = 0; row < GetNumberRows(); ++row ) {

        SetRowLabelValue( row, wxString::Format( wxT( "%d" ), row ) );

        for( int col = 0; col < (GetNumberCols() - 1); ++col ) {

            SetReadOnly( row, col, 1 );
            SetCellBackgroundColour( row, col, (row%2) ? wxColour(235, 235, 235) : wxColour(245, 245, 245) );

            if( ( row % (1 << (vars-col) ) ) <  (1 << (vars - (col + 1) ) ) )
				SetValue( row, col, KarnaughData::ZERO );
            else
				SetValue( row, col, KarnaughData::ONE );
        }

        SetCellBackgroundColour( row, GetNumberCols() - 1, wxColour(215, 225, 255) );
		SetValue( row, GetNumberCols() - 1, KarnaughData::ZERO );
    }

    AutoSizeColumns( true );
    SetColSize( vars, 1.5 * GetColSize(0) );

    ForceRefresh();
    AdjustScrollbars();
}

KarnaughData::eCellValues TruthTableGrid::GetUserInput( wxGridEvent& event )
{
    wxString value = GetCellValue( event.GetRow(), GetNumberCols() - 1 );

    if( ( value == wxT( "0" ) ) )
        return KarnaughData::ZERO;

    if( value == wxT( "1" ) )
        return KarnaughData::ONE;

    return KarnaughData::DONTCARE;
}

void TruthTableGrid::OnMenuRange( wxCommandEvent& event )
{
	wxArrayInt selection = GetSelectedRows();

	switch( event.GetId() ) {
	case MENU_SET1 : SetValue( selection[0], GetNumberCols() - 1, KarnaughData::ONE ); break;
	case MENU_SET0 : SetValue( selection[0], GetNumberCols() - 1, KarnaughData::ZERO ); break;
	case MENU_SETDC : SetValue( selection[0], GetNumberCols() - 1, KarnaughData::DONTCARE ); break;
	case MENU_SETRAND : SetValue( selection[0], GetNumberCols() - 1, (rand()%2 ) ? KarnaughData::ONE : KarnaughData::ZERO ); break;
	}

	SendEvent( wxEVT_GRID_CELL_CHANGE, selection[0], GetNumberCols() - 1 ); // inform main frame of a change
}

void TruthTableGrid::DisplayPopup( wxGridEvent& event )
{
    SetGridCursor( event.GetRow(), event.GetCol() );
    ClearSelection();
    SelectRow( event.GetRow() );

    PopupMenu( stateMenu, event.GetPosition() );
}

void TruthTableGrid::SetShowZeros( bool s )
{
    renderer->draw_zeros( s );

    ForceRefresh();
}

