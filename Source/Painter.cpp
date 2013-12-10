//
//  Painter.cpp
//  BeadBreaker
//
//  Created by Henning Erlandsen on 08.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#include "Painter.h"
#include "Sprite.h"

#include <SDL2/SDL.h>


Painter::Painter( SDL_Renderer* renderer )
: m_pRenderer( renderer )
{
    
}



Painter::~Painter()
{
    
}



void
Painter::drawSprite( Sprite* sprite, int x, int y, int column, int row ) const
{
    SDL_Rect clip;
    clip.x = sprite->getTileWidth() * column;
    clip.y = sprite->getTileHeight() * row;
    clip.w = sprite->getTileWidth();
    clip.h = sprite->getTileHeight();
    
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = sprite->getTileWidth();
    rect.h = sprite->getTileHeight();
    
    SDL_RenderCopy( m_pRenderer, sprite->getTexture(), &clip, &rect );
}



void
Painter::drawSprite( Sprite* sprite, int x, int y, float scale, int column, int row ) const
{
    SDL_Rect clip;
    clip.x = sprite->getTileWidth() * column;
    clip.y = sprite->getTileHeight() * row;
    clip.w = sprite->getTileWidth();
    clip.h = sprite->getTileHeight();
    
    SDL_Rect rect;
    rect.x = x - int( ( scale - 1.0f ) * sprite->getTileWidth() * 0.5f );
    rect.y = y - int( ( scale - 1.0f ) * sprite->getTileHeight() * 0.5f );
    rect.w = sprite->getTileWidth() * scale + 0.5f;
    rect.h = sprite->getTileHeight() * scale + 0.5f;
    
    SDL_RenderCopy( m_pRenderer, sprite->getTexture(), &clip, &rect );
}



void
Painter::swap() const
{
    SDL_RenderPresent( m_pRenderer );
}
