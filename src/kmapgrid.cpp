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

#include "kmapgrid.h"

#include <wx/font.h>

BEGIN_EVENT_TABLE( KMapGrid, wxGrid )
    EVT_GRID_CELL_RIGHT_CLICK( KMapGrid::DisplayPopup )
    EVT_MENU_RANGE( MENU_SET1, MENU_SETRAND, KMapGrid::OnMenuRange )
END_EVENT_TABLE()


class KMapGridCellRenderer : public wxGridCellStringRenderer
{
public:
	KMapGridCellRenderer( );
	~KMapGridCellRenderer();

	void show_greycode( bool on ) { do_show_greycode = on; };
	void draw_zeros( bool on ) { do_draw_zeros = on; };

protected:
    virtual void Draw( wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected );

private:
	wxFont* font;

	bool do_show_greycode;
	bool do_draw_zeros;

	unsigned int GrayEncode( unsigned int number ) { return number ^ (number >> 1); }
};

KMapGridCellRenderer::KMapGridCellRenderer()
{
    font = new wxFont( 7, wxDEFAULT, wxNORMAL, wxNORMAL );
    font->SetFaceName( "sans" );
    do_show_greycode = true;
    do_draw_zeros = true;
}

KMapGridCellRenderer::~KMapGridCellRenderer()
{
	delete font;
}

void KMapGridCellRenderer::Draw( wxGrid& grid, wxGridCellAttr& attr, wxDC& dc, const wxRect& rect, int row, int col, bool isSelected )
{
	wxRect newRect = rect;
	newRect.Inflate(-1);

	wxGridCellRenderer::Draw( grid, attr, dc, rect, row, col, isSelected );	// call base class

	SetTextColoursAndFont( grid, attr, dc, isSelected );

	if( do_draw_zeros || grid.GetCellValue(row, col) != "0" )
		grid.DrawTextRectangle( dc, grid.GetCellValue(row, col), newRect, wxALIGN_CENTER );

    if( do_show_greycode ) {

		dc.SetFont( *font );
		dc.SetTextForeground( wxColour( 150, 150, 150 ) );

		dc.SetPen( *wxGREY_PEN );
		dc.SetBrush( *wxTRANSPARENT_BRUSH );

		int w, h;

		unsigned i;
		for( i = 0; (1 << i) < grid.GetNumberCols(); ++i )
			;

		unsigned int grey_code = ( GrayEncode(row) << i ) + GrayEncode(col);

		dc.GetTextExtent( wxString::Format( "%d", grey_code ), &w, &h );
		dc.DrawText( wxString::Format( "%d", grey_code ), rect.GetX()+rect.GetWidth()-w-2, rect.GetY()+rect.GetHeight()-h-1 );
    }
}


KMapGrid::KMapGrid( wxWindow* parent, wxWindowID id, const wxSize& size )
									: wxGrid( parent, id, wxDefaultPosition, size, wxSIMPLE_BORDER, wxPanelNameStr )
{
    CreateGrid( 1, 1 );
    SetDefaultCellAlignment( wxALIGN_CENTRE, wxALIGN_CENTRE );
    EnableDragGridSize( 0 );

	renderer = new KMapGridCellRenderer();
    SetDefaultRenderer( renderer );

    mnuPopup = new wxMenu;

    mnuPopup->Append( new wxMenuItem( 0, MENU_SET1, _( "Set to 1" ) ) );
    mnuPopup->Append( new wxMenuItem( 0, MENU_SET0, _( "Set to 0" ) ) );
    mnuPopup->Append( new wxMenuItem( 0, MENU_SETDC, _( "Set to \"don't care\"" ) ) );
    mnuPopup->AppendSeparator();
    mnuPopup->Append( new wxMenuItem( 0, MENU_SETRAND, _( "Set randomly" ) ) );

    srand( static_cast<unsigned>( time( 0 ) ) );
}

void KMapGrid::SetValue( unsigned int row, unsigned int col, KarnaughData::eCellValues value )
{
	switch( value ) {
	case KarnaughData::ZERO : SetCellValue( row, col, "0" ); break;
	case KarnaughData::ONE : SetCellValue( row, col, "1" ); break;
	case KarnaughData::DONTCARE : SetCellValue( row, col, "?" ); break;
	}
}

void KMapGrid::ResetBackgroundColour( bool isSOP, unsigned int solution_size )
{
	wxColour the_colour = GetDefaultCellBackgroundColour();
	if( !isSOP )
		the_colour = wxColour( the_colour.Red()-(solution_size * 40), the_colour.Green()-(solution_size * 30), the_colour.Blue() );

	for( int row = 0; row < GetNumberRows(); ++row )
		for( int col = 0; col < GetNumberCols(); ++col )
			SetCellBackgroundColour( row, col, the_colour );

    ForceRefresh();
}

void KMapGrid::SetBackgroundColour( bool isSOP, GridAddresses addresses )
{
	for( GridAddress address : addresses ) {
		wxColour current = GetCellBackgroundColour( address.first, address.second );
		if( isSOP )
			SetCellBackgroundColour( address.first, address.second, wxColour( current.Red()-40, current.Green()-30, current.Blue() ) );
		else
			SetCellBackgroundColour( address.first, address.second, wxColour( current.Red()+40, current.Green()+30, current.Blue() ) );
	}

    ForceRefresh();
}

void KMapGrid::AddCellToSelection( unsigned int row, unsigned int col )
{
	SelectBlock( row, col, row, col, true );
    ForceRefresh();
}

void KMapGrid::ResetSelection()
{
	ClearSelection();
}

void KMapGrid::SetVars( unsigned int vars )
{
    ClearGrid();

    int height = 1 << (vars / 2);
    int width = 1 << ((vars + 1) / 2);

    if( GetNumberCols() < width  ) AppendCols( width - GetNumberCols() );
    if( GetNumberRows() < height ) AppendRows( height - GetNumberRows() );

    if( GetNumberCols() > width  ) DeleteCols( 0, GetNumberCols() - width );
    if( GetNumberRows() > height ) DeleteRows( 0, GetNumberRows() - height );

	ResetBackgroundColour( true, 0 );

	for( int row = 0; row < GetNumberRows(); ++row )
		for( int col = 0; col < GetNumberCols(); ++col )
			SetValue( row, col, KarnaughData::ZERO );

    ForceRefresh();
    AdjustScrollbars();
}

void KMapGrid::SetLabel( int index, wxString label, bool isRow )
{
	if( isRow )
		SetRowLabelValue( index, label );
	else
		SetColLabelValue( index, label );
}

KarnaughData::eCellValues KMapGrid::GetUserInput( wxGridEvent& event )
{
	wxString value = GetCellValue( event.GetRow(), event.GetCol() );

	if( value == "0" )
		return KarnaughData::ZERO;

	if( value == "1" )
		return KarnaughData::ONE;

	return KarnaughData::DONTCARE;
}

void KMapGrid::OnMenuRange( wxCommandEvent& event )
{
	wxGridCellCoordsArray selection = GetSelectionBlockTopLeft();

	int row = selection[0].GetRow();
	int col = selection[0].GetCol();

	switch( event.GetId() ) {
	case MENU_SET1 : SetValue( row, col, KarnaughData::ONE ); break;
	case MENU_SET0 : SetValue( row, col, KarnaughData::ZERO ); break;
	case MENU_SETDC : SetValue( row, col, KarnaughData::DONTCARE ); break;
	case MENU_SETRAND : SetValue( row, col, (rand()%2 ) ? KarnaughData::ONE : KarnaughData::ZERO ); break;
	}

	SendEvent( wxEVT_GRID_CELL_CHANGE, row, col );	// inform main frame of a change
}

void KMapGrid::DisplayPopup( wxGridEvent& event )
{
    SelectBlock( event.GetRow(), event.GetCol(), event.GetRow(), event.GetCol() );
    SetGridCursor( event.GetRow(), event.GetCol() );

    PopupMenu( mnuPopup, event.GetPosition() );
}

void KMapGrid::SetShowZeros( bool on )
{
	renderer->draw_zeros( on );
    ForceRefresh();
}

void KMapGrid::SetCellAdresses( bool on )
{
	renderer->show_greycode( on );
	ForceRefresh();
}
