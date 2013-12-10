//
//  Sprite.h
//  BeadBreaker
//
//  Created by Henning Erlandsen on 10.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#ifndef __BeadBreaker__Sprite__
#define __BeadBreaker__Sprite__

#include <string>

struct SDL_Texture;
struct SDL_Renderer;

class Sprite
{
public:
    ~Sprite();
    
    SDL_Texture*    getTexture() const { return m_pTexture; }
    
    int             getTileHeight() const { return m_height; }
    
    int             getTileWidth() const { return m_width; }
    
    int             getRowCount() const { return m_rows; }
    
    int             getColumnCount() const { return m_cols; }
    
    static Sprite*  Create( const std::string& file, SDL_Renderer* renderer, int columns = 1, int rows = 1 );

private:
    Sprite();
    
private:
    SDL_Texture*    m_pTexture;
    int             m_height;
    int             m_width;
    int             m_rows;
    int             m_cols;
};



#endif /* defined(__BeadBreaker__Sprite__) */
