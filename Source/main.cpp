//
//  main.cpp
//  BeadBreaker
//
//  Created by Henning Erlandsen on 04.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#include "Game.h"
#include "Painter.h"
#include "BoardView.h"
#include "Sprite.h"

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

const int tileSize     = 64;
const int windowHeight = tileSize*BOARD_SIZE;
const int windowWidth  = windowHeight;


#define ASSERT_SDL( expr ) \
if ( !(expr) ) { \
    logError( SDL_GetError(), __FILE__, __LINE__ ); \
    SDL_ClearError(); \
}


void
logError( const char* msg, const char* file, int line )
{
    printf( "Error in %s(%i): %s\n", file, line, msg );
}


SDL_Texture* loadTexture( const std::string &file, SDL_Renderer *ren );


int
main(int argc, char * arg[])
{
    // Init SDL
    ASSERT_SDL( SDL_Init(SDL_INIT_EVERYTHING) == 0 );
    if ( !IMG_Init( IMG_INIT_PNG ) )
        logError("No png support", 0, 0);
    
    // Create a window
    SDL_Window* window = SDL_CreateWindow(
        "Bead Breaker!",            // window title
        SDL_WINDOWPOS_CENTERED,     // x position, centered
        SDL_WINDOWPOS_CENTERED,     // y position, centered
        756,                // width, in pixels
        699,               // height, in pixels
        SDL_WINDOW_OPENGL           // flags
    );
    ASSERT_SDL( window );
    
    // Create renderer
    SDL_Renderer *ren = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    ASSERT_SDL( ren );
    
    // Create textures
    Sprite* beadSprite = Sprite::Create("BeadSprite.png", ren, 1, 5 );
    ASSERT_SDL( beadSprite );
    
    Sprite* background = Sprite::Create("Background.png", ren );
    ASSERT_SDL( background );
    
    Sprite* numbers    = Sprite::Create("NumberSprite.png", ren, 11, 1 );
    ASSERT_SDL( numbers );
 
    // Setup drawing
    Painter p( ren  );
    BoardView view( &p );
    view.setBackgroundSrite( background );
    view.setBeadSprite( beadSprite );
    view.setNumberSprite( numbers );
    view.setBoardPosition( 127, 10 );
    
    // Run game
    Game game( &view );
    int exitCode = game.run();
    
    // Clean up
    delete beadSprite;
    delete background;
    delete numbers;
    
    SDL_DestroyRenderer( ren );
    SDL_DestroyWindow( window );
    SDL_Quit();
    
    return exitCode;
}