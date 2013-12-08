//
//  Painter.cpp
//  BeadBreaker
//
//  Created by Henning Erlandsen on 08.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#include "Painter.h"

#include <SDL2/SDL.h>


Painter::Painter( SDL_Renderer* renderer, SDL_Texture* sprite, int tileSize )
: m_pRenderer( renderer )
, m_pTileSprite( sprite )
, m_tileSize( tileSize )
{
    
}



Painter::~Painter()
{
    
}


void
Painter::drawBackground() const
{
    SDL_SetRenderDrawColor( m_pRenderer, 50, 50, 50, 255 );
    SDL_RenderClear( m_pRenderer );
}



void
Painter::drawTile(int x, int y, int spriteIndexX, int spriteIndexY ) const
{
    SDL_Rect clip;
    clip.x = m_tileSize * spriteIndexX;
    clip.y = m_tileSize * spriteIndexY;
    clip.w = m_tileSize;
    clip.h = m_tileSize;
    
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = rect.h = m_tileSize;
    SDL_RenderCopy( m_pRenderer, m_pTileSprite, &clip, &rect );
    
}



void
Painter::drawTile(int x, int y, int spriteIndexX, int spriteIndexY, float scale ) const
{
    SDL_Rect clip;
    clip.x = m_tileSize * spriteIndexX;
    clip.y = m_tileSize * spriteIndexY;
    clip.w = m_tileSize;
    clip.h = m_tileSize;
    
    SDL_Rect rect;
    rect.x = x - int( ( scale - 1.0f ) * m_tileSize * 0.5f );
    rect.y = y - int( ( scale - 1.0f ) * m_tileSize * 0.5f );
    rect.w = rect.h = m_tileSize * scale + 0.5f;
    SDL_RenderCopy( m_pRenderer, m_pTileSprite, &clip, &rect );

}



void
Painter::swap() const
{
    SDL_RenderPresent( m_pRenderer );
}



int
Painter::getTileSize() const
{
    return m_tileSize;
}