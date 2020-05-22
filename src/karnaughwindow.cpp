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

#include "karnaughwindow.h"

#include "karnaughapp.h"
#include "solutionentry.h"

#include "truthtablegrid.h"
#include "kmapgrid.h"
#include "solutiontree.h"

BEGIN_EVENT_TABLE( KarnaughWindow, wxFrame )
    EVT_MENU( Menu_File_Quit, KarnaughWindow::OnQuit )
    EVT_MENU( Menu_File_About, KarnaughWindow::OnAbout )
    EVT_MENU( Menu_Set_Language, KarnaughWindow::OnSetLanguage )
    EVT_MENU( Menu_Cell_Adresses, KarnaughWindow::OnCellAdresses )
    EVT_MENU( Menu_Show_Zeros, KarnaughWindow::OnShowZeros )
    EVT_SPINCTRL( Vars_Count, KarnaughWindow::OnVarsChange )
    EVT_GRID_CMD_CELL_CHANGE( Truth_Table, KarnaughWindow::OnTruthTChange )
    EVT_GRID_CMD_CELL_CHANGE( Karnaugh_Map, KarnaughWindow::OnKMapChange )
    EVT_TREE_SEL_CHANGED( Sol_Tree, KarnaughWindow::OnSolSelect )
    EVT_CHOICE( Solution_Type, KarnaughWindow::OnSolutionTypeChange )
END_EVENT_TABLE()


KarnaughWindow::KarnaughWindow( KarnaughApp& app_init )
	: wxFrame( (wxFrame *)NULL, -1, _( "Karnaugh Map Minimizer" ), wxDefaultPosition, wxSize( 450,700 ) ), app(app_init)
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
}

void KarnaughWindow::PreSolver( )
{
    SetStatusText( _( "Solving, please wait..." ) );
}

void KarnaughWindow::PostSolverStart( bool isSOP, unsigned int solution_size )
{
    treeSolution->RemoveAllItems();
    kmap_grid->ResetBackgroundColour( isSOP, solution_size );
}

void KarnaughWindow::PostSolverAdd( SolutionEntry& entry, bool isSOP, SolutionAddresses addresses, unsigned int id )
{
	kmap_grid->SetBackgroundColour( isSOP, addresses );
	treeSolution->AddItem( isSOP, entry.GetMask(), entry.GetNumber(), id );
}

void KarnaughWindow::PostSolverFinish()
{
    SetStatusText( _( "Karnaugh map solved!" ) );
}

void KarnaughWindow::SetNewValue( unsigned int adress, GridAddress grid_adress, KarnaughData::eCellValues new_value )
{
    truthTable->SetValue( adress, new_value );
    kmap_grid->SetValue( grid_adress.first, grid_adress.second, new_value );
}

void KarnaughWindow::SetInputs( KarnaughData& data, unsigned int no_of_inputs )
{
    numberOfVariables->SetValue( no_of_inputs );
    truthTable->SetVars( no_of_inputs );
    kmap_grid->SetVars( no_of_inputs );

    for( int row = 0; row < (1 << (no_of_inputs / 2)); ++row )
		kmap_grid->SetLabel( row, data.index_to_greycode_string( row, no_of_inputs / 2 ), true );

    for( int col = 0; col < (1 << ((no_of_inputs + 1) / 2)); ++col )
		kmap_grid->SetLabel( col, data.index_to_greycode_string( col, (no_of_inputs + 1) / 2 ), false );

    treeSolution->RemoveAllItems();
}

void KarnaughWindow::SetNewSolutionType( bool isSOP )
{
    solutionType->SetSelection( isSOP ? 0 : 1 );
}

void KarnaughWindow::SetNewShowAddress( bool on )
{
    menuSettings->Check( Menu_Cell_Adresses, on );
	kmap_grid->SetCellAdresses( on );
}

void KarnaughWindow::SetNewShowZeroes( bool on )
{
    menuSettings->Check( Menu_Show_Zeros, on );
	kmap_grid->SetShowZeros( on );
	truthTable->SetShowZeros( on );
}

void KarnaughWindow::SetSolutionSelection( SolutionAddresses addresses )
{
	kmap_grid->ResetSelection();

	for( auto address : addresses )
		kmap_grid->AddCellToSelection( address.first, address.second );
}



void KarnaughWindow::OnVarsChange( wxSpinEvent& event )
{
	app.SetInputs( numberOfVariables->GetValue() );
}

void KarnaughWindow::OnTruthTChange( wxGridEvent& event )
{
	app.SetNewValue( event.GetRow(), truthTable->GetUserInput( event ) );
}

void KarnaughWindow::OnKMapChange( wxGridEvent& event )
{
	app.SetNewValue( event.GetRow(), event.GetCol(), kmap_grid->GetUserInput( event ) );
}

void KarnaughWindow::OnSolutionTypeChange( wxCommandEvent& event )
{
	app.SetNewSolutionType( event.GetSelection() ? KarnaughData::POS : KarnaughData::SOP );
}

void KarnaughWindow::OnSolSelect( wxTreeEvent& event )
{
	app.SetSolutionSelection( treeSolution->GetEntryID( event.GetItem() ) );
}

void KarnaughWindow::OnQuit( wxCommandEvent& WXUNUSED( event ) )
{
    Close( TRUE );
}

void KarnaughWindow::OnSetLanguage( wxCommandEvent& WXUNUSED( event ) )
{
	app.SelectLanguage();
}

void KarnaughWindow::OnCellAdresses( wxCommandEvent& WXUNUSED( event ) )
{
	app.SetNewShowAddress( showCellAddress->IsChecked() );
}

void KarnaughWindow::OnShowZeros( wxCommandEvent& WXUNUSED( event ) )
{
	app.SetNewShowZeroes( showZeroMenuItem->IsChecked() );
}

void KarnaughWindow::OnAbout( wxCommandEvent& WXUNUSED( event ) )
{
    wxMessageBox( _( "This is a program for minimizing boolean functions using Karnaugh maps method."
                     "\n\nCopyright (C) 2005. Robert Kovacevic" ),
                  _( "About Karnaugh Map Minimizer" ), wxOK | wxICON_INFORMATION, this );
}

