//
//  main.cpp
//  BeadBreaker
//
//  Created by Henning Erlandsen on 04.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#include "BoardModel.h"

#include <iostream>

#include <SDL2/SDL.h>

const int tileSize     = 24;
const int windowHeight = tileSize*BOARD_SIZE;
const int windowWidth  = windowHeight;

void render( SDL_Renderer* renderer, const BoardModel& board, SDL_Texture* sprite );
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
    
    BoardModel boardModel;
    boardModel.randomize();
    
    SDL_Texture* beadSprite = loadTexture( "BeadSprite.bmp", ren );
    
    
    // Start main loop
    SDL_Event e;
    bool quit = false;
    
    SDL_Point grabPosition;
    int       dragDirX = 0;
    int       dragDirY = 0;
    Position  currentTile(-1,-1);
    
    enum State {
        Idle,
        Grabbing,
        Dragging,
    };
    State state = Idle;
    
    while ( !quit )
    {
        // Handle events
        while ( SDL_PollEvent( &e ) )
        {
            switch ( e.type ) {
                case SDL_MOUSEBUTTONDOWN:
                    grabPosition.x = e.button.x;
                    grabPosition.y = e.button.y;
                    
                    // Note: Assumes integer division
                    currentTile.x = grabPosition.x / tileSize;
                    currentTile.y = grabPosition.y / tileSize;
                    
                    std::cout << "Picked:" << currentTile.x << ", " << currentTile.y << std::endl;
                    
                    if ( boardModel.isValidPosition( currentTile ) )
                    {
                        // Trigger drag
                        state = Grabbing;
                    }
                    else
                    {
                        // Set to invalid position
                        currentTile = Position(-1,-1);
                    }
                    break;
                    
                case SDL_MOUSEBUTTONUP:
                    // Drop tile
                    
                    break;
                    
                case SDL_MOUSEMOTION:
                    // Is grabbing tile?
                    if ( state == Grabbing )
                    {
                        SDL_Point pos;
                        pos.x = e.motion.x;
                        pos.y = e.motion.y;
                        
                        int dx = pos.x - grabPosition.x;
                        int dy = pos.y - grabPosition.y;
                        int adx = abs( dx );
                        int ady = abs( dy );
                        
                        // Has dragged past treshold?
                        if ( ( adx + ady ) >= 5 )
                        {
                            // Trigger drag state
                            state = Dragging;
                            
                            // Find drag direction
                            if ( adx > ady )
                            {
                                dragDirX = dx / adx; // Produces 1 or -1
                                dragDirY = 0;
                            }
                            else
                            {
                                dragDirY = dy / ady;
                                dragDirX = 0;
                            }
                            std::cout << "Moving:" << dragDirX << ", " << dragDirY << std::endl;
                            Position other = currentTile.shifted( dragDirX, dragDirY );
                            
                            if ( boardModel.move( currentTile, other ) )
                            {
                                render( ren, boardModel, beadSprite );
                                SDL_Delay( 1000 );
                                
                                std::cout << "Valid move...";
                                // Did move result in matches?
                                if ( boardModel.findMathces() )
                                {
                                    std::cout << "MATCH!" << std::endl;
                                    do
                                    {
                                        boardModel.flushMatches();
                                        SDL_Delay( 1000 );
                                        render( ren, boardModel, beadSprite );
                                    }
                                    while ( boardModel.findMathces() );
                                }
                                else
                                {
                                    std::cout << "No match :(" << std::endl;
                                    // Revert move
                                    boardModel.move( currentTile, other );
                                }
                            }
                            state = Idle;
                        }
                    }
                    // Is moving tile?
                    //if ( state == Dragging )
                    //{
                        
                    //}
                    break;
                    
                case SDL_QUIT:
                    // User requested quit
                    quit = true;
                    
                default:
                    break;
            }
        }
        
        if ( !quit )
            render( ren, boardModel, beadSprite );
    }
    
    // clean up
    SDL_DestroyTexture( beadSprite );
    SDL_DestroyRenderer( ren );
    SDL_DestroyWindow( window );
    SDL_Quit();
    
    return 0;
}



void
render( SDL_Renderer* renderer, const BoardModel& board, SDL_Texture* sprite )
{
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255 );
    SDL_RenderClear( renderer );
    
    for ( Unit x = 0; x < BOARD_SIZE; ++x )
    {
        for ( Unit y = 0; y < BOARD_SIZE; ++y )
        {
            PieceType type = board.getPieceType( Position(x, y) );
            SDL_Rect clip;
            clip.y = tileSize*type;
            clip.x = 0;
            clip.w = tileSize;
            clip.h = tileSize;
            
            SDL_Rect rect;
            rect.x = x * tileSize;
            rect.y = y * tileSize;
            rect.w = tileSize;
            rect.h = tileSize;
            SDL_RenderCopy( renderer, sprite, &clip, &rect );
        }
    }
    
    SDL_RenderPresent( renderer );
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