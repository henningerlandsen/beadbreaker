//
//  BoardModel.cpp
//  BeadBreaker
//
//  Created by Henning Erlandsen on 04.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#include "BoardModel.h"
#include <random>

BoardModel::BoardModel()
{
    
}



BoardModel::~BoardModel()
{
    
}


void
BoardModel::setBoard( const Board& board )
{
    m_board = board;
}



const Board&
BoardModel::getBoard() const
{
    return m_board;
}



const IndexSet&
BoardModel::getRemovedIndexes() const
{
    return m_removed;
}



PieceType
BoardModel::getPieceType( const Position& pos ) const
{
    return m_board[pos];
}


bool
BoardModel::tryMove( const Position& posA, Direction dir )
{
    Position posB = posA;
    switch ( dir ) {
        case Up:
            posB.y -= 1;
            break;
        case Down:
            posB.y += 1;
            break;
        case Left:
            posB.x -= 1;
            break;
        case Right:
            posB.x += 1;
            break;
        default:
            // Invalid direction
            return false;
    }
    
    if ( !isValidPosition( posB ) )
        return false;

    
    // swap types
    swap( posA, posB );
    
    // check for mathces
    if ( checkBoard() )
    {
        return true;
    }
    else
    {
        // swap back
        swap( posA, posB );
        return false;
    }
}



bool
BoardModel::isValidPosition( const Position &pos ) const
{
    return pos.x >= 0 && pos.x < BOARD_SIZE && pos.y >= 0 && pos.y < BOARD_SIZE;
}



void
BoardModel::swap( const Position &a, const Position &b )
{
    PieceType t = m_board[a];
    m_board[a]  = m_board[b];
    m_board[b]  = t;
}



int
BoardModel::checkBoard()
{
    int removedCount = 0;
    
    for ( Unit x = 0; x < BOARD_SIZE; ++x )
    {
        for ( Unit y = 0; y < BOARD_SIZE; ++y )
        {
            // Position to test
            Position p( x, y );
            
            // Check that this piece is not already matched
            if ( m_removed.find( p ) == m_removed.end() )
            {
                IndexSet mathces;
                PieceType type = getPieceType( p );
                
                // Count current piece as a match
                mathces.insert( p );
                
                // Check horizontal matches
                // ------------------------
                checkPiece( p, 1, 0, type, mathces );   // Right
                checkPiece( p, -1, 0, type, mathces );  // Left
                
                if ( mathces.size() >= 3 )
                {
                    // Schedule for removal
                    m_removed.insert( mathces.begin(), mathces.end() );
                    removedCount += mathces.size();
                }
                // Reset match count
                mathces.clear();
                
                // Count current piece as match
                mathces.insert( p );
                
                // Check vertical mathces
                // ----------------------
                checkPiece( p, 0, 1, type, mathces );   // Down
                checkPiece( p, 0, -1, type, mathces );  // Up
                
                // Is valid match?
                if ( mathces.size() >= 3 )
                {
                    // Schedule for removal
                    m_removed.insert( mathces.begin(), mathces.end() );
                    removedCount += mathces.size();
                }
                
                // Reset match count
                mathces.clear();
            }
        }
    }
    
    if ( removedCount )
    {
        updateBoardState();
    }
    
    return removedCount;
}



void
BoardModel::updateBoardState()
{
    // Indexes are sorted by column and from top to bottom. This allows us to
    // replace top->bottom and assume we are not disturbing other removed pieces.
    IndexSet::const_iterator
        iter = m_removed.cbegin(),
        end  = m_removed.cend();
    
    for (; iter != end; ++iter )
    {
        Position removed = *iter;
        
        // Move above pieces down one step
        while ( removed.y > 0 )
        {
            swap( removed, removed.shifted( 0, -1 ) );
            removed.y -= 1;
        }
        
        m_board[removed] = generateRandomValue();
    }
}



void
BoardModel::checkPiece( Position p, Unit dx, Unit dy, PieceType type, IndexSet &results ) const
{
    while ( isValidPosition( p.shift( dx, dy ) ) && getPieceType( p ) == type )
    {
        results.insert( p );
    }
}



PieceType
BoardModel::generateRandomValue() const
{
    return std::rand() % MAX_TYPES;
}
