//
//  ViewModel.cpp
//  BeadBreaker
//
//  Created by Henning Erlandsen on 08.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#include "BoardView.h"
#include <math.h>

// Some hardcoded settings
const float animGrabFreq = 15.0f;
const float animGrabAmpl = 0.1f;
const float fallAccelMin = 0.004f;
const float fallAccelMax = 0.01f;


BoardView::BoardView()
{
    
}



BoardView::~BoardView()
{
    
}



void
BoardView::animateGrab( const Position &a, float progress )
{
    // Use sinus function to do a litte wobble
    float amp    = animGrabAmpl * (1.0f - progress);
    float offset = animGrabAmpl + sinf( progress * animGrabFreq ) * amp;
    m_viewModel[a].dy = -offset;
}



void
BoardView::animateSwap(const Position &a, const Position &b, float progress)
{
    float dx = (a.x - b.x) * progress;
    float dy = (a.y - b.y) * progress;
    
    m_viewModel[a].dx = dx;
    m_viewModel[a].dy = dy;
    m_viewModel[b].dx = -dx;
    m_viewModel[b].dy = -dy;
}



void
BoardView::movePieceUp(const Position &p)
{
    m_viewModel[p].dy -= 1.0f;
}



bool
BoardView::animateFalling(int frame)
{
    bool doneFalling = true;
    
    float fallAccelVar = (fallAccelMax - fallAccelMin) / BOARD_SIZE;
    
    for ( Unit y = 0; y < BOARD_SIZE; ++y ) {
        // Create a variating acceleration based on row of the board.
        // Lower pieces accelerate faster.
        float fallAccel = fallAccelMin + fallAccelVar * y;
        
        for ( Unit x = 0; x < BOARD_SIZE; ++x )
        {
            Position p( x, y );
            // Is piece falling?
            if ( m_viewModel[p].dy < 0.0f )
            {
                doneFalling = false;
                
                float fallSpeed = fallAccel * frame;
                
                m_viewModel[p].dy += fallSpeed;
                
                if ( m_viewModel[p].dy > 0.0f )
                {
                    m_viewModel[p].dy = 0.0f;
                }
            }
        }
    }
    
    return doneFalling;
}



void
BoardView::resetState(const Position &pos)
{
    m_viewModel[pos].dx = 0.0f;
    m_viewModel[pos].dy = 0.0f;
}