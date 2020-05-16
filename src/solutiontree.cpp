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
 *
 *
 */

#include "solutiontree.h"

/**
 * @short ADT
 * @author Robert Kovacevic <robert.kovacevic@etfos.hr>
 * @version 0.1
 */
class SolveTreeItemData : public wxTreeItemData
{
public:
    SolveTreeItemData() : wxTreeItemData() {};
    SolveTreeItemData(std::vector<unsigned int> adresses ) : wxTreeItemData() {this->adresses = adresses; };

    std::vector<unsigned int> adresses;
};



SolutionTree::SolutionTree( wxWindow *parent, wxWindowID id )
		: wxTreeCtrl( parent, id, wxDefaultPosition, wxDefaultSize, wxSIMPLE_BORDER | wxTR_HAS_BUTTONS )
{
	//RemoveAllItems();
}

std::vector<unsigned int> SolutionTree::GetItemAdresses( const wxTreeItemId & item )
{
	return ((SolveTreeItemData*)GetItemData( item ))->adresses;
}

void SolutionTree::RemoveAllItems()
{
	DeleteAllItems();

	RootLabel =  wxT("X = ");

	AddRoot( RootLabel );

    Expand( GetRootItem() );
}

void SolutionTree::AddItem( KarnaughData::eSolutionType type , SolutionEntry& entry, unsigned int max_address )
{
	std::string result;

	// run through the mask bit shifting rit
	char current_variable_name  = 'a';
	unsigned int temp_mask = entry.GetMask();
	unsigned int temp_number = entry.GetNumber();

	for( ;temp_mask; ) {
		if( temp_mask & 0x01 ) {		// this variable is unique
			if( !result.empty() && (type == KarnaughData::SOP))
				result.push_back( '+' );

			result.push_back( current_variable_name );

			if( (type == KarnaughData::SOP) && !(temp_number & 0x01) )
				result.push_back( '\'' );

			if( (type == KarnaughData::POS) && (temp_number & 0x01) )
				result.push_back( '\'' );
		}

		++current_variable_name;
		temp_mask >>= 1;
		temp_number >>= 1;
	}

	if( result.empty() ) {			// there are no unique variables for this solution, this means that the solve is either X = 0 or X = 1, depending on the solution type
		RootLabel.Append( (type == KarnaughData::POS) ? wxT("0") : wxT("1") );
	} else {
		if( type == KarnaughData::SOP ) {
			if( RootLabel != wxT("X = ") )
				RootLabel.Append( wxT(" + ") );

			RootLabel.Append( result );
		} else {
			RootLabel.Append( wxT("(") );
			RootLabel.Append( result );
			RootLabel.Append( wxT(")") );
		}

		AppendItem( GetRootItem(), result, -1, -1, new SolveTreeItemData(entry.GetAddresses( max_address )) );
	}

	SetItemText( GetRootItem(), RootLabel );
	Expand( GetRootItem() );
}


