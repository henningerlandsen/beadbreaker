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

/**
 * BoardView
 * Keeps track of board pieces position
 * on the screen.
 */
class BoardView
{
public:
    BoardView();
    ~BoardView();
    
    void        animateGrab( const Position &a, float progress );
    
    void        animateSwap( const Position &a, const Position &b, float progress );
    void        movePieceUp( const Position &p );
    
    bool        animateFalling( int frame );
    
    bool        animateDestroying( const IndexSet& set, float progress );
    
    void        resetState( const Position &pos );
    
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
        
        PieceState pieces[BOARD_SIZE][BOARD_SIZE];
    };
    
    ViewModel   m_viewModel;
};



#endif /* defined(__BeadBreaker__BoardView__) */
