//
//  Board.h
//  BeadBreaker
//
//  Created by Henning Erlandsen on 04.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#ifndef BeadBreaker_Board_h
#define BeadBreaker_Board_h

#include <set>

#define BOARD_SIZE 8
#define MAX_TYPES 5


typedef unsigned short PieceType;   // Type to use for piece types
typedef short Unit;                 // Type to use for board indexes



struct Index
{
    Unit x;
    Unit y;
    
    //! Default creates invalid position
    Index( Unit _x = -1, Unit _y = -1 )
    : x(_x), y(_y)
    {
        
    }
    
    //! Move this index by dx and dy
    Index& shift( Unit dx, Unit dy )
    {
        x += dx;
        y += dy;
        
        return (*this);
    }
    
    //! Return a copy of this index shifted by dx and dy
    Index shifted( Unit dx, Unit dy ) const
    {
        return Index( x + dx, y + dy );
    }
    
    //! Returns true if index is inside board bounds
    bool isValid() const
    {
        return  x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
    }
};

inline bool operator <( const Index &lhs, const Index &rhs )
{
    return lhs.x < rhs.x || (lhs.x == rhs.x && lhs.y < rhs.y);
}

inline bool operator >( const Index &lhs, const Index &rhs )
{
    return rhs < lhs;
}

inline bool operator ==( const Index &lhs, const Index &rhs )
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator !=( const Index &lhs, const Index &rhs )
{
    return !( lhs == rhs );
}


typedef std::set< Index > IndexSet;




struct Board
{
    //! Return piece type at given index
    const PieceType& operator[]( const Index &pos ) const
    {
        return pieces[pos.y][pos.x];
    }
    
    //! Return modifiable piece type at given index
    PieceType& operator[]( const Index &pos )
    {
        // Note: To conform with the visual layout of an two-dimensional array in C
        // we swap x & y positions.
        return pieces[pos.y][pos.x];
    }
    
    //! Set board data
    void setPieces( PieceType data[8][8] )
    {
        for ( Unit i = 0; i < 8; ++i )
            for ( Unit j = 0; j < 8; ++j )
                pieces[i][j] = data[i][j];
    }
    
    PieceType pieces[8][8];
};

#endif
