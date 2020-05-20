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

#ifndef SOLUTIONTREE_H
#define SOLUTIONTREE_H

#include <wx/wx.h>
#include <wx/treectrl.h>

/** \brief custom tree to display the solutions
 */
class SolutionTree : public wxTreeCtrl
{
public:
    SolutionTree( wxWindow *parent, wxWindowID id );

	void RemoveAllItems( );
	void AddItem( bool isSOP, unsigned int mask, unsigned int number, unsigned long entry_id );
	unsigned long GetEntryID( const wxTreeItemId & item );

private:
	wxString RootLabel;
};

#endif // SOLUTIONTREE_H
