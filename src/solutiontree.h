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

#ifndef SOLUTIONTREE_H
#define SOLUTIONTREE_H

#include <vector>
#include <string>

#include <wx/wx.h>
#include <wx/treectrl.h>

#include "karnaughdata.h"

/** \brief custom tree to display the solutions
 */
class SolutionTree : public wxTreeCtrl
{
public:
    SolutionTree( wxWindow *parent, wxWindowID id );
    virtual ~SolutionTree() {};

	void RemoveAllItems( );
	void AddItem( KarnaughData::eSolutionType type, SolutionEntry& entry, unsigned int max_address );

	bool IsRootItem( const wxTreeItemId & item ) { return GetRootItem() == item; }

	std::vector<unsigned int> GetItemAdresses( const wxTreeItemId & item );

private:
	wxString RootLabel;
};

#endif // SOLUTIONTREE_H
