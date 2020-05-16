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

#ifndef KARNAUGH_MAP_H
#define KARNAUGH_MAP_H

#include <vector>
#include <list>
#include <map>
#include <utility>

/**
 * The business logic / ADT of this application
 */
struct KarnaughNode {
    std::vector<unsigned short int> values;
    unsigned int numberOfItems;
    bool flag;
};

class KarnaughMap
{
public:
    KarnaughMap( int n );

    void Set( unsigned int x, unsigned int y, unsigned short int value );
    std::list<KarnaughNode> Solve();

private:
    void Reset();

    unsigned short int Get( unsigned int x, unsigned int y );
    unsigned int GetMapValue( unsigned int x, unsigned int y );
    std::vector<unsigned short int> GetMapBoolValue( unsigned int x, unsigned int y );

    void Solve2();
    unsigned int GrayEncode( unsigned int g );
    unsigned int IsJoinable( std::vector<unsigned short int> a, std::vector<unsigned short int> b );
    bool IsAtCell( int x, int y, std::vector<unsigned short int> a );

    std::map<std::pair<unsigned int, unsigned int>, unsigned short int> kmap;
    std::map<std::pair<unsigned int, unsigned int>, unsigned int> kmapValues;
    std::map<std::pair<unsigned int, unsigned int>, bool> kmapDCare;

    std::list<KarnaughNode> blocks;

    unsigned int numberOfDCares;
    unsigned int numberOfVariables;
    unsigned int width, height;
};

#endif // KARNAUGH_MAP_H
