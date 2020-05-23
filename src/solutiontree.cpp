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

#include "solutiontree.h"

class SolveTreeItemData : public wxTreeItemData
{
public:
    SolveTreeItemData( unsigned int id ) : wxTreeItemData() { this->id = id; };

    unsigned int id;
};


SolutionTree::SolutionTree( wxWindow *parent, wxWindowID id )
		: wxTreeCtrl( parent, id, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER | wxTR_HAS_BUTTONS )
{
}

unsigned long SolutionTree::GetEntryID( const wxTreeItemId & item )
{
	if( GetRootItem() == item )
		return -1;

	return ((SolveTreeItemData*)GetItemData( item ))->id;
}

void SolutionTree::RemoveAllItems( bool isSOP, unsigned int solution_size )
{
	DeleteAllItems();

	RootLabel =  wxT("X = ");

	if( solution_size == 0 )
		RootLabel.Append( isSOP ? wxT("0") : wxT("1") );

	AddRoot( RootLabel );

    Expand( GetRootItem() );
}

void SolutionTree::AddItem( bool isSOP, unsigned int mask, unsigned int number, unsigned long entry_id )
{
	std::string result;

	for( char current_variable_name  = 'a'; mask; ++current_variable_name ) {

		if( mask & 0x01 ) {		// this variable is unique

			if( !isSOP && !result.empty() )
				result.push_back( '+' );

			result.push_back( current_variable_name );

			if( isSOP && !(number & 0x01) )
				result.push_back( '\'' );

			if( !isSOP && (number & 0x01) )
				result.push_back( '\'' );
		}

		mask >>= 1;
		number >>= 1;
	}

	if( result.empty() ) {			// there are no unique variables for this solution, this means that the solve is either X = 0 or X = 1, depending on the solution type
		RootLabel.Append( isSOP ? wxT("1") : wxT("0") );
	} else {
		if( isSOP ) {
			if( RootLabel != wxT("X = ") )
				RootLabel.Append( wxT(" + ") );

			RootLabel.Append( result );
		} else {
			RootLabel.Append( wxT("(") );
			RootLabel.Append( result );
			RootLabel.Append( wxT(")") );
		}

		AppendItem( GetRootItem(), result, -1, -1, new SolveTreeItemData( entry_id ) );
	}

	SetItemText( GetRootItem(), RootLabel );
	Expand( GetRootItem() );
}
