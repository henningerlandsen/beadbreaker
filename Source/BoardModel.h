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
    
    void            setBoard( const Board &board );
    
    const Board&    getBoard() const;
    
    bool            canMove( const Index &posA, const Index &posB );
    
    bool            move( const Index& posA, const Index& posB );
    
    int             findMathces();
    
    const IndexSet& getMatches() const;
    
    void            flushMatches();
    
    PieceType       getPieceType( const Index& pos ) const;
    
    bool            isValidPosition( const Index& pos ) const;
    
    void            randomize( unsigned int seed = 1 );
    
private:
    void            swap( const Index& a, const Index& b );
    
    void            checkPiece( Index p, Unit dx, Unit dy, PieceType type, IndexSet &results ) const;
    
    void            removeMatches();
    
    PieceType       generateRandomValue() const;
    
private:
    Board       m_board;
    IndexSet    m_matches;
};


#endif /* defined(__BeadBreaker__BoardModel__) */
