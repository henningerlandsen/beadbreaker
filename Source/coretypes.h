//
//  typdefs.h
//  BeadBreaker
//
//  Created by Henning Erlandsen on 04.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#ifndef BeadBreaker_typdefs_h
#define BeadBreaker_typdefs_h

#define BOARD_SIZE 8

// Directions on the board
enum Direction {
    Up,
    Down,
    Left,
    Right
};

// All types of peices
enum PieceTypes {
    Red,
    Green,
    Blue,
    Orange,
    Purple,
    MAX_TYPES,
    // Shorthand versions
    R = Red,
    G = Green,
    B = Blue,
    O = Orange,
    P = Purple,
};
typedef unsigned short PieceType;

typedef short Unit;




struct Position
{
    Unit x;
    Unit y;
    
    Position( Unit _x, Unit _y )
    : x(_x), y(_y)
    {
        
    }
    
    Position& shift( Unit dx, Unit dy )
    {
        x += dx;
        y += dy;
        
        return (*this);
    }
    
    Position shifted( Unit dx, Unit dy ) const
    {
        return Position( x + dx, y + dy );
    }
};

inline bool operator <( const Position &lhs, const Position &rhs )
{
    return lhs.x < rhs.x || lhs.y < rhs.y;
}

inline bool operator >( const Position &lhs, const Position &rhs )
{
    return rhs < lhs;
}

inline bool operator ==( const Position &lhs, const Position &rhs )
{
    return lhs.x == rhs.x && lhs.y == rhs.y;
}

inline bool operator !=( const Position &lhs, const Position &rhs )
{
    return !( lhs == rhs );
}




struct Board
{
    const PieceType* operator[]( unsigned char index ) const
    {
        return pieces[index];
    }
    
    PieceType* operator[]( unsigned char index )
    {
        return pieces[index];
    }
    
    const PieceType& operator[]( const Position &pos ) const
    {
        return pieces[pos.y][pos.x];
    }
    
    PieceType& operator[]( const Position &pos )
    {
        // To conform with the visual layout of an two-dimensional array in C
        // we swap x & y positions.
        return pieces[pos.y][pos.x];
    }
    
    void setPieces( PieceType data[8][8] )
    {
        for ( Unit i = 0; i < 8; ++i )
            for ( Unit j = 0; j < 8; ++j )
                pieces[i][j] = data[i][j];
    }
    
    PieceType pieces[8][8];
};

#endif
