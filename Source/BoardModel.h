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

typedef std::set< Position > IndexSet;

class BoardModel {
    
public:
    BoardModel();
    ~BoardModel();
    
    void            setBoard( const Board &board );
    
    const Board&    getBoard() const;
    
    const IndexSet& getRemovedIndexes() const;
    
    PieceType       getPieceType( const Position& pos ) const;
    
    bool            tryMove( const Position& pos, Direction dir );
    
    bool            update();
    
    bool            isValidPosition( const Position& pos ) const;
    
private:
    void            swap( const Position& a, const Position& b );
    
    int             checkBoard();
    
    void            checkPiece( Position p, Unit dx, Unit dy, PieceType type, IndexSet &results ) const;
    
    void            updateBoardState();
    
    PieceType       generateRandomValue() const;
    
private:
    Board       m_board;
    IndexSet    m_moved;
    IndexSet    m_removed;
};


#endif /* defined(__BeadBreaker__BoardModel__) */
