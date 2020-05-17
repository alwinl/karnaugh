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

#include "solutionentry.h"

SolutionEntry::SolutionEntry( unsigned int mask, unsigned int number )
{
	this->mask = mask;
	this->number = number;
	delete_me = false;
}

bool SolutionEntry::MaskEqual( SolutionEntry& rhs ) const
{
	return mask == rhs.mask;
}

unsigned int SolutionEntry::ComputeXOR( SolutionEntry& rhs ) const
{
	return (number ^ rhs.number) & mask;
}

SolutionEntry SolutionEntry::ComputeNewEntry( unsigned int xor_number ) const
{
	unsigned int new_mask = mask & ~xor_number;
	unsigned int new_number = number & ~xor_number;

	return SolutionEntry( new_mask, new_number );
}

void SolutionEntry::MarkForDeletion()
{
	delete_me = true;
}

bool SolutionEntry::IsDeleted() const
{
	return delete_me;
}

bool SolutionEntry::operator==( SolutionEntry& rhs ) const
{
	return (mask == rhs.mask) && (number == rhs.number);
}

std::vector<unsigned int> SolutionEntry::GetAddresses( unsigned int max_address ) const
{
	std::vector<unsigned int> result;

	for( unsigned int address = 0; address < max_address; ++address )
		if( (address & mask) == number )
			result.push_back( address );

	return result;
}


