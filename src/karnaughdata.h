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

#ifndef KARNAUGHDATA_H
#define KARNAUGHDATA_H

#include <vector>
#include <list>
#include <string>

#include <wx/config.h>

class SolutionEntry;

class KarnaughData
{
public:
    KarnaughData( unsigned int no_of_inputs );

	enum eCellValues { ZERO, ONE, DONTCARE };
	enum eSolutionType { SOP, POS };

    void set_dimension( unsigned int no_of_inputs );
    void set_value( unsigned int address, eCellValues new_value );
	void set_solution_type( eSolutionType type );

    unsigned int get_dimension( ) const { return no_of_inputs; }
    eSolutionType get_solution_type() const { return solution_type; }
    std::list<SolutionEntry> FindBestSolution( );

	unsigned int calc_address( unsigned int row, unsigned int col );
	unsigned int calc_row( unsigned int address );
	unsigned int calc_col( unsigned int address );

	std::vector<std::string> generate_row_labels();
	std::vector<std::string> generate_col_labels();

private:
	unsigned int no_of_inputs;
	std::vector<eCellValues> table;
	eSolutionType solution_type;

	unsigned int GrayEncode( unsigned int number );
	std::vector<unsigned int> number_to_binaryvector( unsigned int code, unsigned int length );
	std::string index_to_greycode_string( unsigned int index, unsigned int length );

	void FindSolution( std::list<SolutionEntry>& solutions );
};

class SolutionEntry
{
public:
	SolutionEntry( unsigned int mask, unsigned int number );

	unsigned int ComputeXOR( SolutionEntry& rhs ) const;
	std::vector<unsigned int> GetAddresses( unsigned int max_address ) const;
	void MarkForDeletion();
	SolutionEntry ComputeNewEntry( unsigned int xor_number ) const;

	unsigned int GetMask() const { return mask; }
	unsigned int GetNumber() const { return number; }

	bool MaskEqual( SolutionEntry& rhs ) const;
	bool IsDeleted() const;
	bool operator==( SolutionEntry& rhs ) const;

private:
	unsigned int mask;
	unsigned int number;
	bool delete_me;
};

class KarnaughConfig
{
public:
    KarnaughConfig();

	enum eLANGUAGES { DEFAULT, CROATIAN, DUTCH };

    void SetLanguage( eLANGUAGES lang );
    void SetShowZeroes( bool on );
    void SetShowAddress( bool on );
    void SetInputs( int inputs );
    void SetSolutionType( KarnaughData::eSolutionType type );

    eLANGUAGES GetLanguage( );
    bool GetShowZeroes();
    bool GetShowAddress();
    int GetInputs();
    KarnaughData::eSolutionType GetSolutionType();

private:
	wxConfig config;
};


#endif // KARNAUGHDATA_H
