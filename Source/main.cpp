//
//  main.cpp
//  BeadBreaker
//
//  Created by Henning Erlandsen on 04.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#include "Game.h"
#include "Painter.h"

#include <iostream>
#include <SDL2/SDL.h>

const int tileSize     = 24;
const int windowHeight = tileSize*BOARD_SIZE;
const int windowWidth  = windowHeight;



SDL_Texture* loadTexture( const std::string &file, SDL_Renderer *ren );


int
main(int argc, char * arg[])
{
    // init SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cout << "ERROR SDL_Init" << std::endl;
        
        return -1;
    }
    
    
    // create a window
    SDL_Window* window = SDL_CreateWindow(
        "Bead Breaker!",            // window title
        SDL_WINDOWPOS_CENTERED,     // x position, centered
        SDL_WINDOWPOS_CENTERED,     // y position, centered
        windowWidth,                // width, in pixels
        windowHeight,               // height, in pixels
        SDL_WINDOW_OPENGL           // flags
    );
    
    SDL_Renderer *ren = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_Texture* beadSprite = loadTexture( "BeadSprite.bmp", ren );
 
    Painter p( ren, beadSprite, tileSize );
    Game game( &p );
    
    int exitCode = game.run();
    
    // clean up
    SDL_DestroyTexture( beadSprite );
    SDL_DestroyRenderer( ren );
    SDL_DestroyWindow( window );
    SDL_Quit();
    
    return exitCode;
}



SDL_Texture*
loadTexture( const std::string &file, SDL_Renderer *ren )
{
	//Initialize to nullptr to avoid dangling pointer issues
	SDL_Texture *texture = nullptr;
	//Load the image
	SDL_Surface *loadedImage = SDL_LoadBMP(file.c_str());
	//If the loading went ok, convert to texture and return the texture
	if ( loadedImage != nullptr )
    {
		texture = SDL_CreateTextureFromSurface( ren, loadedImage );
		SDL_FreeSurface( loadedImage );
		//Make sure converting went ok too
		//if (texture == nullptr)
		//	logSDLError(std::cout, "CreateTextureFromSurface");
	}
	//else
	//	logSDLError(std::cout, "LoadBMP");
    
	return texture;
}