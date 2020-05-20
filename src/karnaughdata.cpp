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

#include "karnaughdata.h"

#include <functional>

#include "solutionentry.h"

KarnaughData::KarnaughData( unsigned int no_of_inputs )
{
	table.resize( 1 << 8, ZERO );
    set_dimension( no_of_inputs );
    solution_type = SOP;
}

void KarnaughData::set_value( unsigned int address, KarnaughData::eCellValues new_value )
{
	if( address > (1 << no_of_inputs) )
		return;

	table[address] = new_value;
}

void KarnaughData::set_dimension( unsigned int no_of_inputs )
{
	if( no_of_inputs > 8 )
		no_of_inputs = 8;

	this->no_of_inputs = no_of_inputs;

	for( eCellValues& cell : table )
		cell = ZERO;
}

void KarnaughData::set_solution_type( eSolutionType type )
{
	solution_type = type;
}

unsigned int KarnaughData::calc_address( unsigned int row, unsigned int col )
{
	return ( GrayEncode(row) << ((no_of_inputs + 1) / 2) ) + GrayEncode(col);
}

unsigned int KarnaughData::calc_row( unsigned int address )
{
	for( int row = 0; row < (1 << (no_of_inputs / 2)); ++row )
		for( int col = 0; col < (1 << ((no_of_inputs + 1) / 2)); ++col ) {
			if( calc_address( row, col ) == address )
				return row;
		}

	return -1;
}

unsigned int KarnaughData::calc_col( unsigned int address )
{
	for( int row = 0; row < (1 << (no_of_inputs / 2)); ++row )
		for( int col = 0; col < (1 << ((no_of_inputs + 1) / 2)); ++col ) {
			if( calc_address( row, col ) == address )
				return col;
		}

	return -1;
}

unsigned int KarnaughData::GrayEncode( unsigned int number )
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
	unsigned int gray_code = GrayEncode(index);

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

std::string KarnaughData::generate_row_label( unsigned int row )
{
	return index_to_greycode_string( row, no_of_inputs / 2 );
}

std::string KarnaughData::generate_col_label( unsigned int col )
{
	return index_to_greycode_string( col, (no_of_inputs + 1) / 2 );
}

/*
	fill list with maxterms, number is address and mask (mask is (1 << no_of_inputs) - 1 )
	for each item in list
		for each item_second following the item
			if item_second.mask == item.mask
				xor_number = (item_second.number xor item.number) & item.mask
				if( count_bits( xor_number ) == 1 )
					make new item with
						number = item.number & ~ xor_number
						mask = item.mask & ~xor_number
					push back item
					mark item and item_second to be deleted

	copy non deleted items to a new list
	return the new list
*/

void KarnaughData::FindSolution( std::list<SolutionEntry>& solutions )
{
	for( std::list<SolutionEntry>::iterator it = solutions.begin(); it != solutions.end(); ++it ) {
		for( std::list<SolutionEntry>::iterator it2 = std::next( it, 1); it2 != solutions.end(); ++it2 ) {

			// filter out duplicates
			if( *it == *it2 ) {
				(*it).MarkForDeletion();
				break;
			}

			if( (*it).MaskEqual( *it2 ) ) {		// compare the masks
				unsigned int xor_number = (*it).ComputeXOR( *it2);
				/*
				 * Using Brian Kernighan’s Algorithm to check that there is one and only one bit in a a number
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

std::vector<SolutionEntry> KarnaughData::FindBestSolution( )
{
	std::list<SolutionEntry> solutions;
	std::vector<unsigned int> dontcares;
	std::list<SolutionEntry> best_solution;

	// fill list with maxterms
	unsigned int mask = (1 << no_of_inputs) - 1;

	for( int address = 0; address < (1 << no_of_inputs); ++address ) {
		if( table[address] == ((solution_type == POS) ? ZERO : ONE) )
			solutions.push_back( SolutionEntry(mask, address ) );

		if( table[address] == DONTCARE )
			dontcares.push_back( address );
	}

	/** @TODO Solution not quite right on all ones or all zeroes */
	// we better deal with our special case when the solution list is empty
	//if( solutions.empty() )
	// if the solution has one one entry, which spans the whole table, delete this entry
	if( solutions.empty() )
		if( solution_type == POS)
			solutions.push_back( SolutionEntry(0,0) );

	//	solutions.push_back( SolutionEntry(0, 0 ) );	// Solution entry mask 0, address 0 is the whole table

	// special case, table is all ones or all zeros
	// when type is sop and table is 1 solutions.size + dontcares.size = 16
	// when type is pos and table is 1 solutions.size = 0
	// when type is sop and table is 0 solutions.size = 0
	// when type is pos and table is 0 solutions.size + dontcares.size = 16

	/* we need to deal with the don't cares
	 * the only way I currently know is to brute force all possible scenarios
	 * Each don't care can be either 0 or one so with dontcares.size() don't cares
	 * we have 2 to the power of dontcares.size() possible scenarios.
	 * Start from the base solutions list and add a subset of don't care addresses
	 * Solve, rate and repeat with a different subset
	 */

	for( int scenario = 0; scenario < (1 << dontcares.size()); ++scenario ) {

		std::list<SolutionEntry> scenario_list( solutions );

		/*
		 * add the appropriate subset of dont cares
		 * As it so happens we have a function that transforms a number into a binary vector
		 * Lets transform the scenario number and add the addresses from the dontcares vector where
		 * the corresponding binary vector entry is 1
		 */
		std::vector<unsigned int> add_address = number_to_binaryvector( scenario, dontcares.size() );

		for( unsigned int index = 0; index < dontcares.size(); ++index )
			if( add_address[index] )
				scenario_list.push_back( SolutionEntry(mask, dontcares[index]) );

		FindSolution( scenario_list );

		if( best_solution.empty() || scenario_list.size() < best_solution.size() )
			best_solution = scenario_list;
	}

	the_solution.clear();
	the_solution.reserve( best_solution.size() );
	std::copy( std::begin(best_solution), std::end(best_solution), std::back_inserter(the_solution) );

	return the_solution;
}

SolutionAddresses KarnaughData::GetEntryAddresses( unsigned int index )
{
	SolutionAddresses addresses;

	if( index < the_solution.size() ) {
		SolutionEntry entry = the_solution[index];

		for( unsigned int adress : entry.GetAddresses( 1 << no_of_inputs ) )
			addresses.push_back( std::make_pair( calc_row(adress), calc_col(adress)) );
	}

	return addresses;
}

SolutionAddresses KarnaughData::GetEntryAddresses( SolutionEntry& entry )
{
	SolutionAddresses addresses;

	for( unsigned int adress : entry.GetAddresses( 1 << no_of_inputs ) )
		addresses.push_back( std::make_pair( calc_row(adress), calc_col(adress)) );

	return addresses;
}
