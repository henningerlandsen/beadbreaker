//
//  ViewModel.cpp
//  BeadBreaker
//
//  Created by Henning Erlandsen on 08.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#include "BoardView.h"
#include "Painter.h"
#include "BoardModel.h"
#include <math.h>
#include <map>

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
    float dx = (a.x - b.x) * (1-progress);
    float dy = (a.y - b.y) * (1-progress);
    
    m_viewModel[a].dx = -dx;
    m_viewModel[a].dy = -dy;
    m_viewModel[b].dx = dx;
    m_viewModel[b].dy = dy;
}



void
BoardView::moveUpColumn( Position p, int rows )
{
    if ( !p.isValid() )
        return;
    
    for (Unit y = BOARD_SIZE-1; y >= 0; --y ) {
        PieceState& cand = m_viewModel[Position(p.x, y )]
        ;
        if ( y + cand.dy <= p.y )
        {
            cand.dy -= rows;
        }
    }
    // Move all pieces up from position p by rows.
}



void
BoardView::prepareFall(const IndexSet &pieces)
{
    Position prev(-1,-1);
    int count = 0;
    
    IndexSet::const_reverse_iterator
    iter = pieces.crbegin(),
    end  = pieces.crend();
    for ( ; iter != end; ++iter )   // For each match...
    {
        Position p = *iter;
        if ( p.x != prev.x || p.y - prev.y != 1 )
        {
            moveUpColumn( prev, count );
            count = 0;
        }
        prev = p;
        ++count;
    }
    moveUpColumn( prev, count );

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
BoardView::animateDestroying(const IndexSet &set, float progress )
{
    IndexSet::const_iterator
    iter = set.cbegin(),
    end  = set.cend();
    for ( ; iter != end; ++iter )   // For each match...
    {
        m_viewModel[*iter].scale = 1.0f-progress;
    }

}



void
BoardView::resetState(const Position &pos)
{
    m_viewModel[pos].dx = 0.0f;
    m_viewModel[pos].dy = 0.0f;
}



void
BoardView::draw( const Painter* painter, const BoardModel* board ) const
{
    painter->drawBackground();
    
    int tileSize = painter->getTileSize();
    
    for ( Unit x = 0; x < BOARD_SIZE; ++x )
    {
        for ( Unit y = 0; y < BOARD_SIZE; ++y )
        {
            Position p( x, y );
            PieceType type = board->getPieceType( p );
            PieceState state = m_viewModel[p];
            painter->drawTile(tileSize * (x + state.dx),
                              tileSize * (y + state.dy),
                              0,
                              type,
                              state.scale );
        }
    }
    
    painter->swap();
    
    
}



void
BoardView::setTileSize(int tileSize)
{
    m_tileSize = tileSize;
}