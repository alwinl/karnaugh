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
#include <wx/config.h>

#include "blamapp.h"
#include "blamframe.h"

IMPLEMENT_APP( blamapp )

bool blamapp::OnInit()
{
	m_translation_helper = new TranslationHelper( *this );
	CreateGUI();

    return true;
}

int blamapp::OnExit()
{
	delete m_translation_helper;
	return 0;
}

void blamapp::CreateGUI()
{
	wxWindow * topwindow = GetTopWindow();

	if( topwindow ) {
		SetTopWindow( NULL );
		topwindow->Destroy();
	}

    blamFrame *frame = new blamFrame( );

    SetTopWindow( frame );
    frame->Show();
}

void blamapp::SelectLanguage( )
{
    if( m_translation_helper->AskUserForLanguage( GetTopWindow() ) )
		CreateGUI();
}

