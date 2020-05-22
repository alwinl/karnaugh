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

#ifndef KARNAUGHAPP_H
#define KARNAUGHAPP_H

#include <wx/wx.h>

#include "karnaughdata.h"

class KarnaughWindow;
class KarnaughConfig;

class KarnaughApp : public wxApp
{
public:
    void SelectLanguage();
	void CreateGUI();

	void SetNewValue( unsigned int address, KarnaughData::eCellValues new_value );
	void SetNewValue( int row, int col, KarnaughData::eCellValues new_value );
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

#endif // KARNAUGHAPP_H
