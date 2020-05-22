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

#ifndef _KARNAUGHAPP_H_
#define _KARNAUGHAPP_H_

#include <wx/wx.h>

#include "karnaughdata.h"

class KarnaughWindow;
class KarnaughConfig;

/**
 * @short BLAM boolean function minimizer app
 * @author Robert Kovacevic <robert.kovacevic@etfos.hr>
 * @version 0.1
 */

class KarnaughApp : public wxApp
{
public:
    void SelectLanguage();
	void CreateGUI();

	void SetNewValue( unsigned int address, KarnaughData::eCellValues new_value );
	void SetInputs( unsigned int no_of_inputs );
	void SetSolutionSelection( unsigned int index );
	void SetNewSolutionType( KarnaughData::eSolutionType type );
	void SetNewShowAddress( bool on );
	void SetNewShowZeroes( bool on );

protected:
    virtual bool OnInit();
	virtual int OnExit();

private:
	KarnaughData * data;
	KarnaughConfig * config;
	KarnaughWindow * frame;

	void RunSolver();
};

#endif // _KARNAUGHAPP_H_
