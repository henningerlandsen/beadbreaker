//
//  BoardModel.h
//  BeadBreaker
//
//  Created by Henning Erlandsen on 04.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#ifndef __BeadBreaker__BoardModel__
#define __BeadBreaker__BoardModel__

#include "Board.h"
#include <set>


class BoardModel {
    
public:
    BoardModel();
    ~BoardModel();
    
    //! Set board data
    void            setBoard( const Board &board );
    
    //! Returns the current board
    const Board&    getBoard() const;
    
    //! Returns true if a move is within the board bounds and A and B are
    //! neighbouring pieces
    bool            canMove( const Index &posA, const Index &posB );
    
    //! Swaps the values for the given positions if conditions for canMove() are
    //! met. Returns true if a swap was made.
    bool            move( const Index& posA, const Index& posB );
    
    //! Returns the number of matches found since the last call to findMatches()
    int             findMathces();
    
    //! Retuns the mathces found by findMatches()
    const IndexSet& getMatches() const;
    
    //! Clears the matches from the board, moves above pieces down and replaces
    //! empty tiles with new random pieces
    void            flushMatches();
    
    //! Returns the value of the given index
    PieceType       getPieceType( const Index& pos ) const;
    
    //! Returns true if the index is inside the board
    bool            isValidPosition( const Index& pos ) const;
    
    //! Randomizes all the values on the board. Note: random is not
    //! automatically seeded, so it will produce the same board every time.
    void            randomize();
    
private:
    //! Simple swap operation
    void            swap( const Index& a, const Index& b );
    
    //! Finds all the mathces in a given direction and stores them in results
    void            checkPiece( Index p, Unit dx, Unit dy, PieceType type, IndexSet &results ) const;
    
    //! Generates a new random tile value
    PieceType       generateRandomValue() const;
    
private:
    Board       m_board;
    IndexSet    m_matches;
};


#endif /* defined(__BeadBreaker__BoardModel__) */
