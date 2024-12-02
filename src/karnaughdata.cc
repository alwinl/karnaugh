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

#include "karnaughdata.h"

#include <functional>

#include "solutionentry.h"

KarnaughData::KarnaughData()
{
	no_of_inputs = 0;
	table.resize( 1 << 8, ZERO );
	solution_type = SOP;
	the_solution.clear();

}

void KarnaughData::set_value( unsigned int address, KarnaughData::eCellValues new_value )
{
	if( address > (unsigned int)(1 << no_of_inputs) )
		return;

	table[address] = new_value;
}

void KarnaughData::set_dimension( unsigned int no_of_inputs )
{
	if( no_of_inputs > 8 )
		no_of_inputs = 8;

	this->no_of_inputs = no_of_inputs;

	std::fill( table.begin(), table.end(), ZERO );
}

void KarnaughData::set_solution_type( eSolutionType type )
{
	solution_type = type;
}

unsigned int KarnaughData::calc_address( unsigned int row, unsigned int col )
{
	return ( gray_encode(row) << ((no_of_inputs + 1) / 2) ) + gray_encode(col);
}

GridAddress KarnaughData::calc_address( unsigned int address )
{
	for( int row = 0; row < (1 << (no_of_inputs / 2)); ++row )
		for( int col = 0; col < (1 << ((no_of_inputs + 1) / 2)); ++col )
			if( calc_address( row, col ) == address )
				return GridAddress( row, col );

	return InvalidGridAddress;
}

unsigned int KarnaughData::gray_encode( unsigned int number )
{
	return number ^ (number >> 1);
}

std::vector<unsigned int> KarnaughData::number_to_binaryvector( unsigned int code, unsigned int length )
{
	if( length == 0 )
		length = 1;

    std::vector<unsigned int> result;
	result.resize( length, 0 );

	for( int i = length; i > 0; --i ) {
		result[i-1] = code % 2;
		code /= 2;
	}

	return result;
}

std::string KarnaughData::index_to_greycode_string( unsigned int index, unsigned int length )
{
	unsigned int gray_code = gray_encode(index);

	if( length == 0 )
		length = 1;

	std::string result;
	result.resize( length, '0' );

	for( int i = length; i > 0; --i ) {
		result[i-1] += gray_code % 2;
		gray_code /= 2;
	}

	return result;
}

void KarnaughData::find_solution( std::list<SolutionEntry>& solutions )
{
	for( std::list<SolutionEntry>::iterator it = solutions.begin(); it != solutions.end(); ++it ) {
		for( std::list<SolutionEntry>::iterator it2 = std::next( it, 1); it2 != solutions.end(); ++it2 ) {

			// filter out duplicates
			if( *it == *it2 ) {
				(*it).MarkForDeletion();
				break;
			}

			if( (*it).MaskEqual( *it2 ) ) {
				unsigned int xor_number = (*it).ComputeXOR( *it2);

				/* Using Brian Kernighan’s Algorithm to check that there is one and only one bit in a a number
				 * If one bit and only one bit is set in a number it must be a multiple of 2.
				 * If you deduct 1 you create a mask for the lower order bits.
				 * if you AND the original number with that mask the result should be zero.
				 * if the result is non zero, there must be a one in the lower order bits, thus we have more
				 * than one bit set
				 */
				if( (xor_number != 0) && ( (xor_number & (xor_number - 1)) == 0) ) {
					solutions.push_back( (*it).ComputeNewEntry( xor_number ) );
					(*it).MarkForDeletion();
					(*it2).MarkForDeletion();
				}
			}

		}
	}

	solutions.remove_if(  std::function<bool( const SolutionEntry& )>( [](const SolutionEntry& rhs) { return rhs.IsDeleted(); } )  );
}

SolutionEntries KarnaughData::find_best_solution( )
{
	std::list<SolutionEntry> solutions;
	std::vector<unsigned int> dontcares;
	std::list<SolutionEntry> best_solution;

	the_solution.clear();

	/* fill list with maxterms ( minterms if POS )
	 */
	unsigned int mask = (1 << no_of_inputs) - 1;

	for( int address = 0; address < (1 << no_of_inputs); ++address ) {
		if( table[address] == ((solution_type == POS) ? ZERO : ONE) )
			solutions.push_back( SolutionEntry(mask, address ) );

		if( table[address] == DONTCARE )
			dontcares.push_back( address );
	}

	/*	We have four edge cases:
	 *	If type is SOP we fill the solution with maxterms, thus if the table is all zeroes
	 *	(or only zeroes and don't cares) the solution is empty. If the table is all ones
	 *	( or only ones and don't cares) the solution resolves to one entry spanning the whole table.
	 *
	 *	If type is POS we fill the solution with minterms, thus if the table is all ones
	 *	(or only ones and don't cares) the solution is empty. If the table is all zeroes
	 *	( or only zeroes and don't cares) the solution resolves to one entry spanning the whole table.
	 *
	 *	We can combine two pairs of cases, one where the solution is empty and one where
	 *	the solution is "full" (ie spans the whole table)
	 *
	 *	In the first instance the solution is and remains empty.
	 *	In the second case the solution has one entry spanning the whole table
	 */
	if( solutions.empty() )
		return the_solution;

	if( (solutions.size() + dontcares.size()) == (unsigned int)(1 << no_of_inputs) ) {
		the_solution.push_back( SolutionEntry(0,0) );
		return the_solution;
	}

	/* we need to deal with the don't cares
	 * the only way I currently know is to brute force all possible scenarios
	 * Each don't care can be either 0 or one so with dontcares.size() don't cares
	 * we have 2 to the power of dontcares.size() possible scenarios.
	 * Start from the base solutions list and add a subset of don't care addresses
	 * Solve, rate and repeat with a different subset
	 */
	for( int scenario = 0; scenario < (1 << dontcares.size()); ++scenario ) {

		std::list<SolutionEntry> scenario_list( solutions );

		/* add the appropriate subset of dont cares
		 * As it so happens we have a function that transforms a number into a binary vector
		 * Lets transform the scenario number and add the addresses from the dontcares vector where
		 * the corresponding binary vector entry is 1
		 */
		std::vector<unsigned int> add_address = number_to_binaryvector( scenario, dontcares.size() );

		for( unsigned int index = 0; index < dontcares.size(); ++index )
			if( add_address[index] )
				scenario_list.push_back( SolutionEntry(mask, dontcares[index]) );

		find_solution( scenario_list );

		if( best_solution.empty() || scenario_list.size() < best_solution.size() )
			best_solution = scenario_list;
	}

	the_solution.reserve( best_solution.size() );
	std::copy( std::begin(best_solution), std::end(best_solution), std::back_inserter(the_solution) );

	return the_solution;
}

GridAddresses KarnaughData::get_entry_addresses( const SolutionEntry& entry )
{
	GridAddresses addresses;

	for( unsigned int adress : entry.GetAddresses( 1 << no_of_inputs ) ) {
		GridAddress grid_address = calc_address( adress );
		addresses.push_back( GridAddress(grid_address.first, grid_address.second) );
	}

	return addresses;
}

GridAddresses KarnaughData::get_entry_addresses( unsigned int index )
{
	if( index < the_solution.size() )
		return get_entry_addresses( the_solution[index] );

	return GridAddresses();
}
