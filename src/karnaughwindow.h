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

#ifndef KARNAUGHWINDOW_H
#define KARNAUGHWINDOW_H

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/treectrl.h>
#include <wx/spinctrl.h>

#include "karnaughdata.h"

class KarnaughApp;
class TruthTableGrid;
class KMapGrid;
class SolutionTree;

class KarnaughWindow : public wxFrame
{
public:
    explicit KarnaughWindow( KarnaughApp& app_init );

	void SetInputs( bool isSOP, unsigned int no_of_inputs );
	void SetGridLabel( int index, wxString label, bool isRow );
	void SetNewValue( unsigned int adress, GridAddress grid_adress, KarnaughData::eCellValues new_value );
	void SetNewSolutionType( bool isSOP );
	void SetSolutionSelection( GridAddresses addresses );
	void SetNewShowAddress( bool on );
	void SetNewShowZeroes( bool on );

	void PreSolver( );
	void PostSolverStart( bool isSOP, unsigned int solution_size );
	void PostSolverAdd( SolutionEntry& entry, bool isSOP, GridAddresses addresses, unsigned int id );
	void PostSolverFinish();

	long GetLanguageChoice( wxArrayString languages );

private:
    enum { QUIT_MENU = 100, ABOUT_MENU, SET_LANGUAGE_MENU, SHOW_CELL_ADDRESS_MENU, SHOW_ZERO_MENU,
							INPUT_VAR_SPINNER, TRUTHTABLE_GRID, KMAP_GRID, SOLUTION_TREE, SOLUTIONTYPE_COMBO };

    void OnQuit( wxCommandEvent& event );
    void OnAbout( wxCommandEvent& event );
	void OnSetLanguage( wxCommandEvent& event );
    void OnShowCellAddress( wxCommandEvent& event );
    void OnShowZero( wxCommandEvent& event );
    void OnInputVarChange( wxSpinEvent& event );
    void OnTruthTableChange( wxGridEvent& event );
    void OnKMapChange( wxGridEvent& event );
    void OnSolutionSelect( wxTreeEvent& event );
    void OnSolutionTypeChange( wxCommandEvent& event );

    wxMenu *mnuSettings;
    wxSpinCtrl* spnInputVariables;
    wxChoice* cbxSolutionType;
    KMapGrid* gridKMap;
    TruthTableGrid* gridTruthTable;
    SolutionTree * treeSolution;

    KarnaughApp& app;

    DECLARE_EVENT_TABLE()
};

#endif // KARNAUGHWINDOW_H
