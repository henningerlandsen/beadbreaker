//
//  ViewModel.h
//  BeadBreaker
//
//  Created by Henning Erlandsen on 08.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#ifndef __BeadBreaker__BoardView__
#define __BeadBreaker__BoardView__

#include "coretypes.h"

class Painter;
class BoardModel;
class Sprite;

/**
 * BoardView
 * Keeps track of board pieces position
 * on the screen.
 */
class BoardView
{
public:
    BoardView( Painter* painter );
    ~BoardView();
    
    void        animateGrab( const Position &a, float progress );
    
    void        animateSwap( const Position &a, const Position &b, float progress );
    void        moveUpColumn( Position from, int rows );
    
    void        prepareFall( const IndexSet &pieces );
    
    bool        animateFalling( int frame );
    
    void        animateDestroying( const IndexSet& set, float progress );
    
    void        resetState( const Position &pos );
    
    void        draw( const BoardModel* board, unsigned int time ) const;
    
    void        setTileSize( int tileSize );
    
    void        setBeadSprite( Sprite* sprite );
    
    void        setBackgroundSrite( Sprite* sprite );
    
    void        setNumberSprite( Sprite* sprite );
    
    Position    mapToPiece( int windowX, int windowY ) const;
    
    void        setPainter( Painter* painter );
    
    void        setBoardPosition( int left, int top );
    
private:
    struct PieceState
    {
        PieceState()
        : dx(0), dy(-9.0f), scale(1.0f), frame(0), endFrame(0) {}
        
        float dx;
        float dy;
        float scale;
        int   frame;
        int   endFrame;
    };
    
    struct ViewModel
    {

        PieceState& operator []( const Position &p )
        {
            return pieces[p.x][p.y];
        }
        
        const PieceState& operator []( const Position &p ) const
        {
            return pieces[p.x][p.y];
        }
        
        PieceState pieces[BOARD_SIZE][BOARD_SIZE];
    };
    
    
    int         m_tileSize;
    ViewModel   m_viewModel;
    Sprite*     m_pBeadSprite;
    Sprite*     m_pBackground;
    Sprite*     m_pNumberSprite;
    Painter*    m_pPainter;
    int         m_boardX;
    int         m_boardY;
};



#endif /* defined(__BeadBreaker__BoardView__) */
