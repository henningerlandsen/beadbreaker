//
//  BoardModel.h
//  BeadBreaker
//
//  Created by Henning Erlandsen on 04.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#ifndef __BeadBreaker__BoardModel__
#define __BeadBreaker__BoardModel__

#include "coretypes.h"
#include <set>


class BoardModel {
    
public:
    BoardModel();
    ~BoardModel();
    
    void            setBoard( const Board &board );
    
    const Board&    getBoard() const;
    
    bool            move( const Position& posA, const Position& posB );
    
    int             findMathces();
    
    const IndexSet& getMatches() const;
    
    void            flushMatches();
    
    PieceType       getPieceType( const Position& pos ) const;
    
    bool            isValidPosition( const Position& pos ) const;
    
    void            randomize();
    
private:
    void            swap( const Position& a, const Position& b );
    
    void            checkPiece( Position p, Unit dx, Unit dy, PieceType type, IndexSet &results ) const;
    
    void            removeMatches();
    
    PieceType       generateRandomValue() const;
    
private:
    Board       m_board;
    IndexSet    m_matches;
};


#endif /* defined(__BeadBreaker__BoardModel__) */
