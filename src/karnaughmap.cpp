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

#include "karnaughmap.h"
#include <cmath>
#include <iostream>

KarnaughMap::KarnaughMap( int n ) : numberOfDCares(0), numberOfVariables(n)
{
    // Determine width and height from n. of vars
    height = 1 << (numberOfVariables / 2);
    width = 1 << ((numberOfVariables + 1) / 2);

    // Fill map with 0s and clear the list of KarnaughNodes
    Reset();

#if 0
    // Fill map kmapValues with values that each cell in the map
    // has. Look here for rules:
    // http://www.allaboutcircuits.com/vol_4/chpt_8/3.html
    if( numberOfVariables>2 ) {
        for( unsigned int i=0; i<height; i++ ) {
            for( unsigned int j=0; j<width; j++ ) {
				kmapValues[std::make_pair( j, i )] = GrayEncode( i * width + j );
#if 0
                // Set every 4x4 block's first 4 vars to gray code
                if( width>=4 )
                    kmapValues[std::make_pair( j, i )] = GrayEncode( j%4+( ( i%4 )*4 ) );
                else
                    kmapValues[std::make_pair( j, i )] = GrayEncode( j%4+( ( i%4 )*width ) );
                // Combine 4x4 blocks into map (5th and higher variables)
                kmapValues[std::make_pair( j, i )]+=16*( j/4+( ( i/4 )*( width/4 ) ) );
#endif
            }
        }
    } else {
        if( numberOfVariables==2 ) {
            kmapValues[std::make_pair( 0, 0 )]=0;
            kmapValues[std::make_pair( 1, 0 )]=1;
            kmapValues[std::make_pair( 0, 1 )]=2;
            kmapValues[std::make_pair( 1, 1 )]=3;
        }
        if( numberOfVariables==1 ) {
            kmapValues[std::make_pair( 0, 0 )]=0;
            kmapValues[std::make_pair( 1, 0 )]=1;
        }
    }
#else
	for( unsigned int row=0; row<height; row++ )
		for( unsigned int col=0; col<width; col++ )
			kmapValues[std::make_pair( col, row )] = ( GrayEncode(row) << ((numberOfVariables + 1) / 2) ) + GrayEncode(col);

#endif
}

void KarnaughMap::Reset()
{
    /* Fills map with zeros and deletes all nodes from the solution list */

    for( unsigned int i=0; i<height; i++ )
        for( unsigned int j=0; j<width; j++ )
            Set( j, i, 0 );
    blocks.clear();
}

std::list<KarnaughNode> KarnaughMap::Solve()
{
    /* Iterates through all possible ways that 'Don't cares' can be
    arranged, and finds one with fewest number of nodes in the solution
    (bestc). If there are more ways that give the same number of nodes
    in the solution choose the one with biggest nodes (bestsc) */

    std::list<KarnaughNode> best;
    int bestc=-1;
    unsigned int bestsc=0;

    for( unsigned int i=0; i<pow( 2.0, ( double )numberOfDCares ); i++ ) {
        std::vector<bool> b;
        unsigned int j=i;
        while( j>0 ) {
            b.insert( b.begin(), ( bool )( j%2 ) );
            j=j/2;
        }
        for( j=b.size(); j<numberOfDCares; j++ )
            b.insert( b.begin(), 0 );

        blocks.clear();

        unsigned int c=0;
        for( unsigned int k=0; k<height; k++ ) {
            for( unsigned int l=0; l<width; l++ ) {
                if( kmapDCare[std::make_pair( l, k )]==1 ) {
                    if( b[c]==1 )
                        kmap[std::make_pair( l, k )]=1;
                    else
                        kmap[std::make_pair( l, k )]=0;
                    c++;
                }
            }
        }

        Solve2();

        if( ( bestc==-1 ) || ( blocks.size()<=( unsigned int )bestc ) ) {
            unsigned int sc=0;
            std::list<KarnaughNode>::const_iterator iter;
            for( iter=blocks.begin(); iter != blocks.end(); iter++ ) {
                for( unsigned int i=0; i<iter->values.size(); i++ )
                    if( iter->values[i]==2 )
                        sc++;
            }

            if( ( bestc==-1 ) || ( blocks.size()<( unsigned int )bestc ) ) {
                best=blocks;
                bestc=best.size();
                bestsc=sc;
            } else {
                if( sc>bestsc ) {
                    best=blocks;
                    bestc=best.size();
                    bestsc=sc;
                }
            }
        }
    }
    blocks=best;

    return blocks;
}

void KarnaughMap::Solve2()
{
    /* Check for special case that all cells in the map are the same */
	unsigned short int cell_zero_value = kmap[std::make_pair( 0, 0 )];
	unsigned int idx;

    for( idx=0; idx<(height*width); ++idx ) {
		if( kmap[std::make_pair( idx % width, idx / width )] != cell_zero_value )
			break;
    }

    if( idx == (height*width) ) {		// all cells are the same

        /* Clear the list so that all those nodes with one item are deleted */
        blocks.clear();

        // If there are only zeros in the map there's nothing to solve
        // If there are only ones, solution is one element as big as the map
        if( kmap[std::make_pair( 0, 0 )] != 0 ) {
			// create one node and make that the one and only block
            KarnaughNode n;

            n.numberOfItems = width*height;
            n.values.resize( n.numberOfItems, 2 );

            blocks.push_back( n );
        }

		return;
    }

#if 0
    /* Check for special case that all cells in the map are the same */
    bool a=1;
    for( unsigned int i=0; i<height; i++ ) {
        if( a==0 )
            break;
        for( unsigned int j=0; j<width; j++ )
            if( kmap[std::make_pair( j, i )] != kmap[std::make_pair( 0, 0 )] ) {
                a=0;
                break;
            }
    }

    if( a==1 ) {
        /* Clear the list so that all those nodes with one item are deleted */
        blocks.clear();

        // If there are only zeros in the map there's nothing to solve
        if( kmap[std::make_pair( 0, 0 )]==0 )
            return;
        else {
            // If there are only ones, solution is one element as big as the map
            KarnaughNode n;
            n.numberOfItems = width*height;
            for( unsigned int j=0; j<numberOfVariables; j++ )
                n.values.insert( n.values.begin(), 2 );
            blocks.push_back( n );
            return;
        }
    }
#endif // 0

    /* Put all blocks with 1 element in list */
    for( unsigned int i=0; i<height; i++ ) {
        for( unsigned int j=0; j<width; j++ ) {
            if( kmap[std::make_pair( j, i )]==1 ) {
                KarnaughNode n;
                n.numberOfItems=1;
                n.flag=0;
                n.values=GetMapBoolValue( j, i );
                blocks.push_back( n );
            }
        }
    }

    // Joining blocks into blocks with 2^i elements
    for( int i = 1; i <= ( int )log2( width*height ); i++ ) {
        /* Check every block with every other block and see if they can be joined
        into a bigger block */

        for( std::list<KarnaughNode>::iterator a=blocks.begin(); a!=blocks.end(); a++ ) {
            for( std::list<KarnaughNode>::iterator b=blocks.begin(); b!=blocks.end(); b++ ) {
                if(	( a->numberOfItems == ( unsigned int )pow( 2.0, i-1 ) ) &&
                        ( b->numberOfItems == ( unsigned int )pow( 2.0, i-1 ) ) ) {
                    unsigned int x=IsJoinable( a->values, b->values );
                    if( x>0 ) {
                        /* If they can be joined make a new block with 2 in the place
                        of the one bit where they a and b are different */
                        KarnaughNode n;
                        n.numberOfItems=a->numberOfItems*2;
                        n.flag=0;
                        for( unsigned int i=0; i<a->values.size(); i++ ) {
                            if( i!=( x-1 ) )
                                n.values.insert( n.values.end(), a->values[i] );
                            else
                                n.values.insert( n.values.end(), 2 );
                        }

                        /* Mark that a node is part of a larger node */
                        a->flag=1;
                        b->flag=1;

                        /* Check if that block already exists in the list */
                        bool exist=0;
                        for( std::list<KarnaughNode>::const_iterator c=blocks.begin(); c!=blocks.end(); c++ )
                            if( n.values==c->values )
                                exist=1;

                        if( !exist )
                            blocks.push_back( n );
                    }
                }
            }
        }
    }

    /* Deletes nodes that are contained in larger nodes */
    for( std::list<KarnaughNode>::iterator a=blocks.begin(); a != blocks.end(); /*nothing*/ ) {
        if( a->flag==1 )
            blocks.erase( a++ );
		else
            ++a;
    }

    /* Deletes unneeded nodes. Draws a temp map with all nodes but one
    and if that map is same as the main map, node that wasn't drawn can be deleted */

    std::map<std::pair<unsigned int, unsigned int>, unsigned short int> temp;

    for( std::list<KarnaughNode>::iterator a=blocks.begin(); a!=blocks.end(); a++ ) {
        for( unsigned int i=0; i<height; i++ )
            for( unsigned int j=0; j<width; j++ )
                temp[std::make_pair( j, i )]=0;

        for( std::list<KarnaughNode>::iterator b=blocks.begin(); b!=blocks.end(); b++ ) {
            if( a!=b ) {
                for( unsigned int i=0; i<height; i++ ) {
                    for( unsigned int j=0; j<width; j++ ) {
                        if( IsAtCell( j, i, b->values ) )
                            temp[std::make_pair( j, i )]=1;
                    }
                }
            }
        }

        bool del=1;
        for( unsigned int i=0; i<height; i++ ) {
            for( unsigned int j=0; j<width; j++ ) {
                if( temp[std::make_pair( j, i )]!=kmap[std::make_pair( j, i )] ) {
                    del=0;
                    break;
                }
            }
            if( !del )
                break;
        }


        if( del ) {
            a=blocks.erase( a );
            a--;
        }
    }
}

bool KarnaughMap::IsAtCell( int x, int y, std::vector<unsigned short int> a )
{
    std::vector<unsigned short int> b = GetMapBoolValue( x, y );

    for( unsigned int i=0; i<a.size(); i++ )
        if( ( a[i]!=b[i] ) && ( a[i]!=2 ) )
            return 0;
    return 1;
}

std::vector<unsigned short int> KarnaughMap::GetMapBoolValue( unsigned int x, unsigned int y )
{
    std::vector<unsigned short int> b;

    unsigned int i=GetMapValue( x, y );

    while( i>0 ) {
        b.insert( b.begin(), i%2 );
        i=i/2;
    }
    for( unsigned int j=b.size(); j<numberOfVariables; j++ )
        b.insert( b.begin(), 0 );

    return b;
}

/* Checks if 2 karnaugh nodes with values a and b are joinable (only differ in one bit),
and if they are returns (place where they differ + 1), otherwise returns 0 */
unsigned int KarnaughMap::IsJoinable( std::vector<unsigned short int> a, std::vector<unsigned short int> b )
{
    int c=0;
    int x;

    for( unsigned int i=0; i<a.size(); i++ ) {
        if( a[i]!=b[i] ) {
            c++;
            x=i;
        }
    }
    if( c==1 )
        return x+1;
    else
        return 0;
}

unsigned int KarnaughMap::GrayEncode( unsigned int g )
{
	return g ^ (g >> 1);
}

void KarnaughMap::Set( unsigned int x, unsigned int y, unsigned short int value )
{
    kmap[std::make_pair( x, y )]=value;
    if( value==2 ) {
        kmapDCare[std::make_pair( x, y )]=1;
        numberOfDCares++;
    } else
        kmapDCare[std::make_pair( x, y )]=0;
}

unsigned short int KarnaughMap::Get( unsigned int x, unsigned int y )
{
    if( !kmapDCare[std::make_pair( x,y )] )
        return kmap[std::make_pair( x,y )];
    else
        return 2;
}

unsigned int KarnaughMap::GetMapValue( unsigned int x, unsigned int y )
{
    return kmapValues[std::make_pair( x,y )];
}

