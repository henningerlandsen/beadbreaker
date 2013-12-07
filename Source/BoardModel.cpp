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
BoardModel::getMatches() const
{
    return m_matches;
}



PieceType
BoardModel::getPieceType( const Position& pos ) const
{
    return m_board[pos];
}



bool
BoardModel::move( const Position &posA, const Position& posB )
{
    if ( !isValidPosition( posA) || !isValidPosition( posB ) )
        return false;
    
    // Check that pieces are neighbours
    if ( ( abs( posA.x - posB.x ) + abs( posA.y - posB.y ) ) != 1 )
        return false;
    
    // Swap pieces
    swap( posA, posB );
    
    return true;
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
BoardModel::findMathces()
{
    int matchCount = (int)m_matches.size();
    
    for ( Unit x = 0; x < BOARD_SIZE; ++x )
    {
        for ( Unit y = 0; y < BOARD_SIZE; ++y )
        {
            // Position to test
            Position p( x, y );
            
            // Check that this piece is not already matched
            if ( m_matches.find( p ) == m_matches.end() )
            {
                IndexSet tempMatches;
                PieceType type = getPieceType( p );
                
                // Count current piece as a match
                tempMatches.insert( p );
                
                // Check horizontal matches
                // ------------------------
                checkPiece( p, 1, 0, type, tempMatches );   // Right
                checkPiece( p, -1, 0, type, tempMatches );  // Left
                
                if ( tempMatches.size() >= 3 )
                {
                    // Schedule for removal
                    m_matches.insert( tempMatches.begin(), tempMatches.end() );
                }
                // Reset match count
                tempMatches.clear();
                
                // Count current piece as match
                tempMatches.insert( p );
                
                // Check vertical mathces
                // ----------------------
                checkPiece( p, 0, 1, type, tempMatches );   // Down
                checkPiece( p, 0, -1, type, tempMatches );  // Up
                
                // Is valid match?
                if ( tempMatches.size() >= 3 )
                {
                    // Schedule for removal
                    m_matches.insert( tempMatches.begin(), tempMatches.end() );
                }
                
                // Reset match count
                tempMatches.clear();
            }
        }
    }
    
    return (int)m_matches.size() - matchCount;
}



void
BoardModel::flushMatches()
{
    // Indexes are sorted by column and from top to bottom. This allows us to
    // replace top->bottom and assume we are not disturbing other removed pieces.
    IndexSet::const_iterator
        iter = m_matches.cbegin(),
        end  = m_matches.cend();
    
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
    
    m_matches.clear();
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


void
BoardModel::randomize()
{
    // This could be done smarter to avoid the step below.
    for ( Unit x = 0; x < BOARD_SIZE; ++x ) {
        for ( Unit y = 0; y < BOARD_SIZE; ++y ) {
            m_board[Position(x,y)] = generateRandomValue();
        }
    }
    
    // Remove all random generated matches until we
    // have a stable board
    while( findMathces() )
    {
        flushMatches();
    }
}
