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
#include "Sprite.h"
#include <math.h>
#include <map>

// Some hardcoded settings
const float animGrabFreq = 15.0f;
const float animGrabAmpl = 0.1f;
const float fallAccelMin = 0.004f;
const float fallAccelMax = 0.02f;


BoardView::BoardView( Painter* painter )
: m_pBackground( nullptr )
, m_pNumberSprite( nullptr )
, m_pBeadSprite( nullptr )
, m_pWarningSprite( nullptr )
, m_pPainter( painter )
{
    
}



BoardView::~BoardView()
{
    
}



void
BoardView::animateGrab( const Index &a, float progress )
{
    // Use sinus function to do a litte wobble
    float amp    = animGrabAmpl * (1.0f - progress);
    float offset = animGrabAmpl + sinf( progress * animGrabFreq ) * amp;
    m_viewModel[a].dy = -offset;
}



void
BoardView::animateSwap(const Index &a, const Index &b, float progress)
{
    float dx = (a.x - b.x) * (1-progress);
    float dy = (a.y - b.y) * (1-progress);
    
    m_viewModel[a].dx = -dx;
    m_viewModel[a].dy = -dy;
    m_viewModel[b].dx = dx;
    m_viewModel[b].dy = dy;
}



void
BoardView::moveUpColumn( Index p, int rows )
{
    if ( !p.isValid() )
        return;
    
    for (Unit y = BOARD_SIZE-1; y >= 0; --y ) {
        PieceState& cand = m_viewModel[Index(p.x, y )]
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
    Index prev(-1,-1);
    int count = 0;
    
    IndexSet::const_reverse_iterator
    iter = pieces.crbegin(),
    end  = pieces.crend();
    for ( ; iter != end; ++iter )   // For each match...
    {
        Index p = *iter;
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
            Index p( x, y );
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
BoardView::resetState(const Index &pos)
{
    m_viewModel[pos].dx = 0.0f;
    m_viewModel[pos].dy = 0.0f;
}



void
BoardView::draw( const BoardModel* board, unsigned int time ) const
{
    m_pPainter->drawSprite( m_pBackground, 0, 0 );
    
    for ( Unit x = 0; x < BOARD_SIZE; ++x )
    {
        for ( Unit y = 0; y < BOARD_SIZE; ++y )
        {
            Index p( x, y );
            PieceType type = board->getPieceType( p );
            PieceState state = m_viewModel[p];
            m_pPainter->drawSprite(m_pBeadSprite,
                                m_boardX + m_pBeadSprite->getTileWidth() * (x + state.dx),
                                m_boardY + m_pBeadSprite->getTileHeight() * (y + state.dy),
                                state.scale,
                                0, type );
        }
    }
    
    // Less than ten seconds?
    if ( time <= 10000 )
    {
        // flash sprite every second
        if ( (time / 1000) % 2 == 1 )
            m_pPainter->drawSprite( m_pWarningSprite, 63, 520 );
    }
    
    // Draw numbers
    int x = 333;
    int y = 585;
    
    int digit = time / 10000;
    time -= digit * 10000;
    m_pPainter->drawSprite(m_pNumberSprite,
                           x, y, digit, 0 );
    x += m_pNumberSprite->getTileWidth();
    
    digit = time / 1000;
    m_pPainter->drawSprite(m_pNumberSprite,
                           x, y, digit, 0 );
    
    
    m_pPainter->swap();
    
    
}



void
BoardView::setBackgroundSrite(Sprite *sprite)
{
    m_pBackground = sprite;
}



void
BoardView::setBeadSprite(Sprite *sprite)
{
    m_pBeadSprite = sprite;
}



void
BoardView::setNumberSprite(Sprite *sprite)
{
    m_pNumberSprite = sprite;
}



void
BoardView::setWarningSprite(Sprite *sprite)
{
    m_pWarningSprite = sprite;
}



void
BoardView::setBoardPosition(int left, int top)
{
    m_boardX = left;
    m_boardY = top;
}



Index
BoardView::mapToIndex(int windowX, int windowY) const
{
    return Index((windowX - m_boardX) / m_pBeadSprite->getTileWidth(),
                    (windowY - m_boardY) / m_pBeadSprite->getTileHeight());
}

