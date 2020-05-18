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
#include <wx/notebook.h>
#include <wx/grid.h>
#include <wx/treectrl.h>
#include <wx/spinctrl.h>
#include <wx/config.h>

#include "blamframe.h"

#include "blamapp.h"

#include "solutionentry.h"

BEGIN_EVENT_TABLE( blamFrame, wxFrame )
    EVT_MENU( Menu_File_Quit, blamFrame::OnQuit )
    EVT_MENU( Menu_File_About, blamFrame::OnAbout )
    EVT_MENU( Menu_Set_Language, blamFrame::OnSetLanguage )
    EVT_MENU( Menu_Cell_Adresses, blamFrame::OnCellAdresses )
    EVT_MENU( Menu_Show_Zeros, blamFrame::OnShowZeros )
    EVT_SPINCTRL( Vars_Count, blamFrame::OnVarsChange )
    EVT_GRID_CMD_CELL_CHANGE( Truth_Table, blamFrame::OnTruthTChange )
    EVT_GRID_CMD_CELL_CHANGE( Karnaugh_Map, blamFrame::OnKMapChange )
    EVT_TREE_SEL_CHANGED( Sol_Tree, blamFrame::OnSolSelect )
    EVT_CHOICE( Solution_Type, blamFrame::OnSolutionTypeChange )
END_EVENT_TABLE()


/*----------------------------------------
 * Main window object implementation
 */
blamFrame::blamFrame( ) : wxFrame( (wxFrame *)NULL, -1, _( "Karnaugh Map Minimizer" ), wxDefaultPosition, wxSize( 450,700 ) ), data(4)
{
    /**** Icon *****/
    SetIcon( wxIcon( "wxwin.ico", wxBITMAP_TYPE_ICO ) );

    /**** Menu *****/
    wxMenuBar *menuBar = new wxMenuBar;

    wxMenu *menuFile = new wxMenu;
    menuSettings = new wxMenu;

    showZeroMenuItem = new wxMenuItem( 0, Menu_Show_Zeros, _( "Show zeros" ), _( "Show / hide zero values" ), wxITEM_CHECK );
    showCellAddress = new wxMenuItem( 0, Menu_Cell_Adresses, _( "Show cell adresses" ), _( "Show / hide cell adresses in the K-map" ), wxITEM_CHECK );

    menuFile->Append( new wxMenuItem( 0, Menu_File_About, _( "&About" ), _( "About the program" ) ) );
    menuFile->AppendSeparator();
    menuFile->Append( new wxMenuItem( 0, Menu_File_Quit, _( "E&xit" ), _( "Exit the program" ) ) );

    menuSettings->Append( new wxMenuItem( 0, Menu_Set_Language, _( "Set language" ), _( "Set language" ) ) );
    menuSettings->Append( showZeroMenuItem );
    menuSettings->Append( showCellAddress );

    menuBar->Append( menuFile, _( "&Program" ) );
    menuBar->Append( menuSettings, _( "&Settings" ) );

    SetMenuBar( menuBar );

    /**** Status Bar *****/
    CreateStatusBar();
    SetStatusText( _( "Welcome to Karnaugh Map Minimizer!" ) );

    /**** GUI initialization *****/
    wxPanel* mainPanel = new wxPanel( this, -1, wxDefaultPosition, wxDefaultSize );

    wxNotebook* methodBook = new wxNotebook( mainPanel, -1 );

    wxPanel* kmapPanel = new wxPanel( methodBook, -1 );

	methodBook->AddPage( kmapPanel, _( "Karnaugh map" ) );

    truthTable = new TruthTableGrid( mainPanel, Truth_Table, wxSize( 170,450 ) );
    kmap_grid = new KMapGrid( kmapPanel, Karnaugh_Map, wxSize( 100,150 ) );
    treeSolution = new SolutionTree( mainPanel, Sol_Tree );

    wxBoxSizer* kmapSizer = new wxBoxSizer( wxVERTICAL );

    kmapSizer->Add( kmap_grid, 1, wxEXPAND | wxALL, 5 );

    kmapPanel->SetSizer( kmapSizer );

    numberOfVariables = new wxSpinCtrl( mainPanel, Vars_Count );
    numberOfVariables->SetRange( 1, 8 );

    solutionType = new wxChoice( mainPanel, Solution_Type );
    solutionType->Append( _( "Sum of products" ) );
    solutionType->Append( _( "Product of sums" ) );

    wxStaticBoxSizer* leftSizer = new wxStaticBoxSizer( new wxStaticBox( mainPanel, -1, _( "Truth table" ) ), wxVERTICAL );

    leftSizer->Add( truthTable, 1, wxEXPAND | wxALL, 5 );

    wxBoxSizer* rightSizerTop = new wxBoxSizer( wxHORIZONTAL );

    rightSizerTop->Add( new wxStaticText( mainPanel, -1, _( "Number of variables: " ) ), 0, wxCENTER | wxRIGHT, 10 );
    rightSizerTop->Add( numberOfVariables, 0, wxCENTER );
    rightSizerTop->Add( new wxStaticText( mainPanel, -1, _( "Type of solution: " ) ), 0, wxCENTER | wxLEFT | wxRIGHT, 10 );
    rightSizerTop->Add( solutionType, 1,  wxCENTER );

    wxBoxSizer* rightSizerBottom = new wxBoxSizer( wxHORIZONTAL );

    rightSizerBottom->Add( methodBook, 1, wxEXPAND  | wxBOTTOM, 10 );

    wxBoxSizer* rightSizer = new wxBoxSizer( wxVERTICAL );

    rightSizer->Add( rightSizerTop, 0, wxEXPAND | wxBOTTOM, 10 );
    rightSizer->Add( rightSizerBottom, 2, wxEXPAND );
    rightSizer->Add( new wxStaticText( mainPanel, -1, _( "Solution:" ) ), 0, wxEXPAND | wxTOP, 0 );
    rightSizer->Add( treeSolution, 1, wxEXPAND | wxTOP, 5 );


    wxBoxSizer* mainSizer = new wxBoxSizer( wxHORIZONTAL );

    mainSizer->Add( leftSizer, 1, wxEXPAND | wxALL, 10 );
    mainSizer->Add( rightSizer, 2, wxEXPAND | wxTOP | wxRIGHT | wxBOTTOM, 10 );
    mainSizer->SetSizeHints( mainPanel );

    mainPanel->SetSizer( mainSizer );

    wxBoxSizer* panelSizer = new wxBoxSizer( wxHORIZONTAL );

    panelSizer->Add( mainPanel, 1, wxEXPAND );
    panelSizer->SetSizeHints( this );

    SetSizerAndFit( panelSizer );
    SetAutoLayout( true );

    /**** Load configuration ****/

    SetNewShowAddress( config.GetShowAddress() );
    SetNewShowZeroes( config.GetShowZeroes() );
    SetInputs( config.GetInputs() );
    SetNewSolutionType( config.GetSolutionType() );
}

void blamFrame::RunSolver()
{
    SetStatusText( _( "Solving, please wait..." ) );

    std::vector<SolutionEntry> solutions = data.FindBestSolution();

    treeSolution->RemoveAllItems();
    kmap_grid->ResetBackgroundColour( data.get_solution_type(), solutions.size() );

    unsigned int id = 0;
	for( SolutionEntry& entry : solutions ) {
		kmap_grid->SetBackgroundColour( data, entry );
		treeSolution->AddItem( data.get_solution_type(), entry, id++ );
	}

    SetStatusText( _( "Karnaugh map solved!" ) );
}

void blamFrame::SetNewValue( unsigned int adress, KarnaughData::eCellValues new_value )
{
	data.set_value( adress, new_value );

    truthTable->SetValue( adress, new_value );	// By querying and resetting we validate the user data
    kmap_grid->SetValue( data.calc_row(adress), data.calc_col(adress), new_value );

    RunSolver();
}

void blamFrame::SetInputs( unsigned int no_of_inputs )
{
	config.SetInputs( no_of_inputs );
	data.set_dimension( no_of_inputs );

    numberOfVariables->SetValue( no_of_inputs );
    truthTable->SetVars( no_of_inputs );
    kmap_grid->SetVars( data, no_of_inputs );
    treeSolution->RemoveAllItems();

    RunSolver();
}

void blamFrame::SetNewSolutionType( KarnaughData::eSolutionType type )
{
	config.SetSolutionType( type );
	data.set_solution_type( type );

    solutionType->SetSelection( type == KarnaughData::SOP ? 0 : 1 );

	RunSolver();
}

void blamFrame::SetNewShowAddress( bool on )
{
	config.SetShowAddress( on );

    menuSettings->Check( Menu_Cell_Adresses, on );
	kmap_grid->SetCellAdresses( on );
}

void blamFrame::SetNewShowZeroes( bool on )
{
	config.SetShowZeroes( on );

    menuSettings->Check( Menu_Show_Zeros, on );
	kmap_grid->SetShowZeros( on );
	truthTable->SetShowZeros( on );
}

void blamFrame::SetSolutionSelection( unsigned int index )
{
	kmap_grid->ResetSelection();

	SolutionEntry entry = data.get_solution( index );
	if( entry == InvalidEntry )
		return;

	for( unsigned int adress : entry.GetAddresses( 1 << data.get_dimension() ) )
		kmap_grid->AddCellToSelection( data.calc_row(adress), data.calc_col(adress) );
}



void blamFrame::OnVarsChange( wxSpinEvent& event )
{
	SetInputs( numberOfVariables->GetValue() );
}

void blamFrame::OnTruthTChange( wxGridEvent& event )
{
	SetNewValue( event.GetRow(), truthTable->GetUserInput( event ) );
}

void blamFrame::OnKMapChange( wxGridEvent& event )
{
	SetNewValue( data.calc_address( event.GetRow(), event.GetCol() ), kmap_grid->GetUserInput( event ) );
}

void blamFrame::OnSolutionTypeChange( wxCommandEvent& event )
{
	SetNewSolutionType( event.GetSelection() ? KarnaughData::POS : KarnaughData::SOP );
}

void blamFrame::OnSolSelect( wxTreeEvent& event )
{
	SetSolutionSelection( treeSolution->GetEntryID( event.GetItem() ) );
}

void blamFrame::OnQuit( wxCommandEvent& WXUNUSED( event ) )
{
    Close( TRUE );
}

void blamFrame::OnSetLanguage( wxCommandEvent& WXUNUSED( event ) )
{
	wxGetApp().SelectLanguage();
}

void blamFrame::OnCellAdresses( wxCommandEvent& WXUNUSED( event ) )
{
	SetNewShowAddress( showCellAddress->IsChecked() );
}

void blamFrame::OnShowZeros( wxCommandEvent& WXUNUSED( event ) )
{
	SetNewShowZeroes( showZeroMenuItem->IsChecked() );
}

void blamFrame::OnAbout( wxCommandEvent& WXUNUSED( event ) )
{
    wxMessageBox( _( "This is a program for minimizing boolean functions using Karnaugh maps method."
                     "\n\nCopyright (C) 2005. Robert Kovacevic" ),
                  _( "About Karnaugh Map Minimizer" ), wxOK | wxICON_INFORMATION, this );
}

