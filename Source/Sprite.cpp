//
//  Sprite.cpp
//  BeadBreaker
//
//  Created by Henning Erlandsen on 10.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#include "Sprite.h"

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>


SDL_Texture*
loadTexture( const std::string &file, SDL_Renderer *ren )
{
	//Initialize to nullptr to avoid dangling pointer issues
	SDL_Texture *texture = nullptr;
	//Load the image
	SDL_Surface *loadedImage = IMG_Load( file.c_str() );
	//If the loading went ok, convert to texture and return the texture
	if ( loadedImage != nullptr )
    {
		texture = SDL_CreateTextureFromSurface( ren, loadedImage );
		SDL_FreeSurface( loadedImage );
	}
    
	return texture;
}



Sprite::Sprite()
: m_pTexture( NULL )
, m_rows( 0 )
, m_cols( 0 )
, m_height( 0 )
, m_width( 0 )
{
}



Sprite::~Sprite()
{
    SDL_DestroyTexture( m_pTexture );
    m_pTexture = nullptr;
}



Sprite*
Sprite::Create( const std::string& file, SDL_Renderer* renderer, int columns, int rows )
{
    int w, h;
    //Initialize to nullptr to avoid dangling pointer issues
	SDL_Texture *texture = nullptr;
	//Load the image
	SDL_Surface *loadedImage = IMG_Load( file.c_str() );
	//If the loading went ok, convert to texture and return the texture
	if ( loadedImage != nullptr )
    {
		texture = SDL_CreateTextureFromSurface( renderer, loadedImage );
        w = loadedImage->w;
        h = loadedImage->h;
		SDL_FreeSurface( loadedImage );
	}
    
    if ( !texture )
        return nullptr;
    
    Sprite* sprite = new Sprite();
    sprite->m_pTexture = texture;
    sprite->m_height   = h / rows;
    sprite->m_width    = w / columns;
    sprite->m_rows     = rows;
    sprite->m_cols     = columns;
    
    return sprite;
}






