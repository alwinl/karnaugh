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

#ifndef SOLUTIONENTRY_H
#define SOLUTIONENTRY_H

#include <vector>

class SolutionEntry
{
public:
	SolutionEntry( unsigned int mask, unsigned int number );

	unsigned int ComputeXOR( const SolutionEntry& rhs ) const;
	std::vector<unsigned int> GetAddresses( unsigned int max_address ) const;
	void MarkForDeletion();
	SolutionEntry ComputeNewEntry( unsigned int xor_number ) const;

	unsigned int GetMask() const { return mask; }
	unsigned int GetNumber() const { return number; }

	bool MaskEqual( const SolutionEntry& rhs ) const;
	bool IsDeleted() const;
	bool operator==( const SolutionEntry& rhs ) const;
	bool operator!=( const SolutionEntry& rhs ) const { return ! operator==( rhs ); };

private:
	unsigned int mask;
	unsigned int number;
	bool delete_me;
};

typedef std::vector<SolutionEntry> SolutionEntries;

#endif // SOLUTIONENTRY_H
