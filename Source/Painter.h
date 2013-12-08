//
//  Painter.h
//  BeadBreaker
//
//  Created by Henning Erlandsen on 08.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#ifndef __BeadBreaker__Painter__
#define __BeadBreaker__Painter__

#include "coretypes.h"

#include <string>

class SDL_Window;
class SDL_Texture;
class SDL_Renderer;

class Painter
{
public:
    Painter( SDL_Renderer* renderer, SDL_Texture* sprite, int tileSize );
    
    ~Painter();
    
    void drawBackground() const;
    
    void drawTile( int x, int y, int spriteIndexX, int spriteIndexY ) const;
    
    void drawTile( int x, int y, int spriteIndexX, int spriteIndexY, float scale ) const;

    int  getTileSize() const;
    
    void swap() const;
    
private:
    SDL_Renderer*   m_pRenderer;
    SDL_Texture*    m_pTileSprite;
    int             m_tileSize;
};

#endif /* defined(__BeadBreaker__Painter__) */
