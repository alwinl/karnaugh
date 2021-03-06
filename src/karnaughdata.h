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

#ifndef KARNAUGHDATA_H
#define KARNAUGHDATA_H

#include <vector>
#include <list>
#include <string>

#include "solutionentry.h"

typedef std::pair<unsigned int, unsigned int> GridAddress;
typedef std::vector<GridAddress> GridAddresses;

class KarnaughData
{
public:
    KarnaughData();

	enum eCellValues { ZERO, ONE, DONTCARE };
	enum eSolutionType { SOP, POS };

    void set_dimension( unsigned int no_of_inputs );
    void set_value( unsigned int address, eCellValues new_value );
	void set_solution_type( eSolutionType type );

    unsigned int get_dimension( ) const { return no_of_inputs; }
    eSolutionType get_solution_type() const { return solution_type; }
    eCellValues get_value( unsigned int address ) const { return table[address]; };
    SolutionEntries find_best_solution( );

	GridAddresses get_entry_addresses( unsigned int index );
	GridAddresses get_entry_addresses( const SolutionEntry& entry );

	unsigned int calc_address( unsigned int row, unsigned int col );
	GridAddress calc_address( unsigned int address );

	std::string index_to_greycode_string( unsigned int index, unsigned int length );

private:
	unsigned int no_of_inputs;
	std::vector<eCellValues> table;
	eSolutionType solution_type;
	SolutionEntries the_solution;

	unsigned int gray_encode( unsigned int number );
	std::vector<unsigned int> number_to_binaryvector( unsigned int code, unsigned int length );
	void find_solution( std::list<SolutionEntry>& solutions );
};

static GridAddress InvalidGridAddress(-1, -1);

#endif // KARNAUGHDATA_H
