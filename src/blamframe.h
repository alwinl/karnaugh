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

#ifndef _BLAMFRAME_H_
#define _BLAMFRAME_H_

#include <wx/wx.h>
#include <wx/spinctrl.h>
#include <wx/treectrl.h>

#include "karnaughdata.h"
//#include "karnaughconfig.h"

#include "truthtablegrid.h"
#include "kmapgrid.h"
#include "solutiontree.h"

/**
 * @short Application Main Frame
 * @author Robert Kovacevic <robert.kovacevic@etfos.hr>
 * @version 0.1
 */
class blamFrame : public wxFrame
{
public:
    blamFrame( const wxString& title, const wxPoint& pos, const wxSize& size );

private:
    enum {
    	Menu_File_Quit = 100,
		Menu_File_About,
		Menu_Language_Croatian,
		Menu_Language_Dutch,
		Menu_Language_Default,
		Menu_Cell_Adresses,
		Menu_Show_Zeros,
		Vars_Count,
		Truth_Table,
		Karnaugh_Map,
		Sol_Tree,
		Solution_Type
	};

    void OnQuit( wxCommandEvent& event );
    void OnAbout( wxCommandEvent& event );
	void OnMenuLanguage( wxCommandEvent& event );
    //void OnLangCroatian( wxCommandEvent& event );
    //void OnLangDefault( wxCommandEvent& event );
    void OnCellAdresses( wxCommandEvent& event );
    void OnShowZeros( wxCommandEvent& event );
    void OnVarsChange( wxSpinEvent& event );
    void OnTruthTChange( wxGridEvent& event );
    void OnKMapChange( wxGridEvent& event );
    void OnSolSelect( wxTreeEvent& event );
    void OnSolutionTypeChange( wxCommandEvent& event );

    wxMenu *menuLanguage;
    wxMenu *menuSettings;
    wxMenuItem * showZeroMenuItem;
    wxMenuItem * showCellAddress;
    wxSpinCtrl* numberOfVariables;
    wxChoice* solutionType;
    KMapGrid* kmap_grid;
    SolutionTree * treeSolution;
    TruthTableGrid* truthTable;

    KarnaughData data;
    KarnaughConfig config;
    wxLocale m_locale;

	void SetInputs( unsigned int no_of_inputs );
	void RunSolver();
	void SetNewValue( unsigned int adress, KarnaughData::eCellValues new_value );
	void SetNewSolutionType( KarnaughData::eSolutionType type );
	void SetNewLanguage( KarnaughConfig::eLANGUAGES language );
	void SetNewShowAddress( bool on );
	void SetNewShowZeroes( bool on );

    DECLARE_EVENT_TABLE()
};

#endif // _BLAMFRAME_H_
