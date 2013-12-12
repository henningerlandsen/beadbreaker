//
//  Painter.h
//  BeadBreaker
//
//  Created by Henning Erlandsen on 08.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#ifndef __BeadBreaker__Painter__
#define __BeadBreaker__Painter__


#include <string>

class SDL_Renderer;
class Sprite;

class Painter
{
public:
    Painter( SDL_Renderer* renderer );
    
    ~Painter();
    
    //! Draw a sprite at position [x, y] using column and row index of sprite
    //! sheet.
    void drawSprite( Sprite* sprite, int x, int y, int column = 0, int row = 0 ) const;
    
    //! Draw a sprite at position [x, y] with a given scale. Scale is applied from the
    //! center of the tile.
    void drawSprite( Sprite* sprite, int x, int y, float scale, int column = 0, int row = 0 ) const;
    
    void swap() const;
    
private:
    SDL_Renderer*   m_pRenderer;
};

#endif /* defined(__BeadBreaker__Painter__) */
