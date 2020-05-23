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

#include "karnaughwindow.h"

#include <wx/notebook.h>

#include "karnaughapp.h"

#include "truthtablegrid.h"
#include "kmapgrid.h"
#include "solutiontree.h"

BEGIN_EVENT_TABLE( KarnaughWindow, wxFrame )
    EVT_MENU( QUIT_MENU, KarnaughWindow::OnQuit )
    EVT_MENU( ABOUT_MENU, KarnaughWindow::OnAbout )
    EVT_MENU( SET_LANGUAGE_MENU, KarnaughWindow::OnSetLanguage )
    EVT_MENU( SHOW_CELL_ADDRESS_MENU, KarnaughWindow::OnShowCellAddress )
    EVT_MENU( SHOW_ZERO_MENU, KarnaughWindow::OnShowZero )
    EVT_SPINCTRL( INPUT_VAR_SPINNER, KarnaughWindow::OnInputVarChange )
    EVT_GRID_CMD_CELL_CHANGE( TRUTHTABLE_GRID, KarnaughWindow::OnTruthTableChange )
    EVT_GRID_CMD_CELL_CHANGE( KMAP_GRID, KarnaughWindow::OnKMapChange )
    EVT_TREE_SEL_CHANGED( SOLUTION_TREE, KarnaughWindow::OnSolutionSelect )
    EVT_CHOICE( SOLUTIONTYPE_COMBO, KarnaughWindow::OnSolutionTypeChange )
END_EVENT_TABLE()


KarnaughWindow::KarnaughWindow( KarnaughApp& app_init )
	: wxFrame( (wxFrame *)NULL, -1, _( "Karnaugh Map Minimizer" ), wxDefaultPosition, wxSize( 450,700 ) ), app(app_init)
{
    /**** Icon *****/
    SetIcon( wxIcon( "wxwin.ico", wxBITMAP_TYPE_ICO ) );

    /**** Menu *****/
    wxMenuBar *menuBar = new wxMenuBar;

    wxMenu *menuFile = new wxMenu;
    menuFile->Append( new wxMenuItem( 0, ABOUT_MENU, _( "&About" ), _( "About the program" ) ) );
    menuFile->AppendSeparator();
    menuFile->Append( new wxMenuItem( 0, QUIT_MENU, _( "E&xit" ), _( "Exit the program" ) ) );
    menuBar->Append( menuFile, _( "&Program" ) );

    mnuSettings = new wxMenu;
    mnuSettings->Append( new wxMenuItem( 0, SET_LANGUAGE_MENU, _( "Set language" ), _( "Set language" ) ) );
    mnuSettings->Append( new wxMenuItem( 0, SHOW_ZERO_MENU, _( "Show zeros" ), _( "Show / hide zero values" ), wxITEM_CHECK ) );
    mnuSettings->Append( new wxMenuItem( 0, SHOW_CELL_ADDRESS_MENU, _( "Show cell adresses" ), _( "Show / hide cell adresses in the K-map" ), wxITEM_CHECK ) );
    menuBar->Append( mnuSettings, _( "&Settings" ) );

    SetMenuBar( menuBar );

    /**** Status Bar *****/
    CreateStatusBar();
    SetStatusText( _( "Welcome to Karnaugh Map Minimizer!" ) );

    /**** GUI initialization *****/
    wxPanel* mainPanel = new wxPanel( this, -1, wxDefaultPosition, wxDefaultSize );

    wxNotebook* methodBook = new wxNotebook( mainPanel, -1 );

    wxPanel* kmapPanel = new wxPanel( methodBook, -1 );

	methodBook->AddPage( kmapPanel, _( "Karnaugh map" ) );

    gridTruthTable = new TruthTableGrid( mainPanel, TRUTHTABLE_GRID, wxSize( 170,450 ) );
    gridKMap = new KMapGrid( kmapPanel, KMAP_GRID, wxSize( 100,150 ) );
    treeSolution = new SolutionTree( mainPanel, SOLUTION_TREE );

    wxBoxSizer* kmapSizer = new wxBoxSizer( wxVERTICAL );

    kmapSizer->Add( gridKMap, 1, wxEXPAND | wxALL, 5 );

    kmapPanel->SetSizer( kmapSizer );

    spnInputVariables = new wxSpinCtrl( mainPanel, INPUT_VAR_SPINNER );
    spnInputVariables->SetRange( 1, 8 );

    cbxSolutionType = new wxChoice( mainPanel, SOLUTIONTYPE_COMBO );
    cbxSolutionType->Append( _( "Sum of products" ) );
    cbxSolutionType->Append( _( "Product of sums" ) );

    wxStaticBoxSizer* leftSizer = new wxStaticBoxSizer( new wxStaticBox( mainPanel, -1, _( "Truth table" ) ), wxVERTICAL );

    leftSizer->Add( gridTruthTable, 1, wxEXPAND | wxALL, 5 );

    wxBoxSizer* rightSizerTop = new wxBoxSizer( wxHORIZONTAL );

    rightSizerTop->Add( new wxStaticText( mainPanel, -1, _( "Number of variables: " ) ), 0, wxCENTER | wxRIGHT, 10 );
    rightSizerTop->Add( spnInputVariables, 0, wxCENTER );
    rightSizerTop->Add( new wxStaticText( mainPanel, -1, _( "Type of solution: " ) ), 0, wxCENTER | wxLEFT | wxRIGHT, 10 );
    rightSizerTop->Add( cbxSolutionType, 1,  wxCENTER );

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
    treeSolution->RemoveAllItems( isSOP, solution_size );
    gridKMap->ResetBackgroundColour( isSOP, solution_size );
}

void KarnaughWindow::PostSolverAdd( SolutionEntry& entry, bool isSOP, GridAddresses addresses, unsigned int id )
{
	gridKMap->SetBackgroundColour( isSOP, addresses );
	treeSolution->AddItem( isSOP, entry.GetMask(), entry.GetNumber(), id );
}

void KarnaughWindow::PostSolverFinish()
{
    SetStatusText( _( "Karnaugh map solved!" ) );
}

void KarnaughWindow::SetNewValue( unsigned int adress, GridAddress grid_adress, KarnaughData::eCellValues new_value )
{
    gridTruthTable->SetValue( adress, new_value );
    gridKMap->SetValue( grid_adress.first, grid_adress.second, new_value );
}

void KarnaughWindow::SetInputs( bool isSOP, unsigned int no_of_inputs )
{
    spnInputVariables->SetValue( no_of_inputs );
    gridTruthTable->SetVars( no_of_inputs );
    gridKMap->SetVars( no_of_inputs );

    treeSolution->RemoveAllItems( isSOP, 0 );
}

void KarnaughWindow::SetGridLabel( int index, wxString label, bool isRow )
{
	gridKMap->SetLabel( index, label, isRow );
}

void KarnaughWindow::SetNewSolutionType( bool isSOP )
{
    cbxSolutionType->SetSelection( isSOP ? 0 : 1 );
}

void KarnaughWindow::SetNewShowAddress( bool on )
{
    mnuSettings->Check( SHOW_CELL_ADDRESS_MENU, on );
	gridKMap->SetCellAdresses( on );
}

void KarnaughWindow::SetNewShowZeroes( bool on )
{
    mnuSettings->Check( SHOW_ZERO_MENU, on );
	gridKMap->SetShowZeros( on );
	gridTruthTable->SetShowZeros( on );
}

void KarnaughWindow::SetSolutionSelection( GridAddresses addresses )
{
	gridKMap->ResetSelection();

	for( auto address : addresses )
		gridKMap->AddCellToSelection( address.first, address.second );
}

void KarnaughWindow::OnInputVarChange( wxSpinEvent& event )
{
	app.SetInputs( event.GetPosition() );
}

void KarnaughWindow::OnTruthTableChange( wxGridEvent& event )
{
	app.SetNewValue( event.GetRow(), gridTruthTable->GetUserInput( event ) );
}

void KarnaughWindow::OnKMapChange( wxGridEvent& event )
{
	app.SetNewValue( event.GetRow(), event.GetCol(), gridKMap->GetUserInput( event ) );
}

void KarnaughWindow::OnSolutionTypeChange( wxCommandEvent& event )
{
	app.SetNewSolutionType( event.GetSelection() ? KarnaughData::POS : KarnaughData::SOP );
}

void KarnaughWindow::OnSolutionSelect( wxTreeEvent& event )
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

void KarnaughWindow::OnShowCellAddress( wxCommandEvent& event )
{
	app.SetNewShowAddress( event.IsChecked() );
}

void KarnaughWindow::OnShowZero( wxCommandEvent& event )
{
	app.SetNewShowZeroes( event.IsChecked() );
}

void KarnaughWindow::OnAbout( wxCommandEvent& WXUNUSED( event ) )
{
    wxMessageBox( _( "This is a program for minimizing boolean functions using Karnaugh maps method."
                     "\n\nCopyright (C) 2020. Alwin Leerling" ),
                  _( "About Karnaugh Map Minimizer" ), wxOK | wxICON_INFORMATION, this );
}

