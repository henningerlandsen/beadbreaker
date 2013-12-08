//
//  Game.cpp
//  BeadBreaker
//
//  Created by Henning Erlandsen on 08.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#include "Game.h"
#include "BoardModel.h"
#include "BoardView.h"
#include "Painter.h"

#include <SDL2/SDL.h>
#include <iostream>




Game::Game( Painter* painter )
: m_pPainter( painter )
{
    m_tileSize = m_pPainter->getTileSize();
}



Game::~Game()
{
}



int
Game::run()
{
    if ( !m_pPainter )
        return Error;

    // Animation constants:
    const int animGrabLength = 30;
    const int animSwapLength = 30;

    // Animation frame counter
    uint animFrame = 0;
    
    // Setup random board
    BoardModel board;
    board.randomize();
    
    BoardView boardView;
    
    // Initial game state
    GameState state = Falling;
    
    SDL_Point grabPosition;
    Position  currentTile(-1, -1);
    
    // Swapping direction
    int swapDX = 0;
    int swapDY = 0;
    
    SDL_Event e;
    
    // Run game loop
    while ( state != Quit )
    {
        // Handle user input events
        while ( SDL_PollEvent( &e ) )
        {
            switch ( e.type )
            {
                case SDL_MOUSEBUTTONDOWN:
                    if ( state == WaitForMove )
                    {
                        grabPosition.x = e.button.x;
                        grabPosition.y = e.button.y;
                        
                        // Note: Assumes integer division
                        currentTile.x = grabPosition.x / m_tileSize;
                        currentTile.y = grabPosition.y / m_tileSize;
                        
                        if ( board.isValidPosition( currentTile ) )
                        {
                            // Trigger drag
                            state = Grabbing;
                            animFrame = 0;
                        }
                        else
                        {
                            // Set to invalid position
                            currentTile = Position(-1,-1);
                        }
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    // Drop piece
                    if ( state == Grabbing )
                    {
                        state = WaitForMove;
                        // Reset item state
                        boardView.resetState( currentTile );
                        
                        currentTile = Position( -1, -1 );
                    }
                    break;
                    
                case SDL_MOUSEMOTION:
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
                            // Find drag direction
                            if ( adx > ady )
                            {
                                swapDX = dx / adx; // Produces 1 or -1
                                swapDY = 0;
                            }
                            else
                            {
                                swapDY = dy / ady;
                                swapDX = 0;
                            }
                            std::cout << "Moving:" << swapDX << ", " << swapDY << std::endl;
                            Position other = currentTile.shifted( swapDX, swapDY );

                            // Is move possible?
                            if ( board.move( currentTile, other ) )
                            {
                                // Did move result in a match
                                if ( board.findMathces() )
                                    state = SwapWin;
                                else
                                    state = SwapFail;
                            }
                            else
                            {
                                
                            }
                        }
                    }
                    break;
                    
                case SDL_QUIT:
                    state = Quit;
                    break;
                    
                default:
                    break;
            }
        }
        
        switch ( state )
        {
            case Grabbing:
            {
                // Update grab animation
                if ( animFrame <= animGrabLength )
                {
                    boardView.animateGrab( currentTile, animFrame / (float)animGrabLength );
                }
                break;
            }
            case SwapWin:
                // Update swap animation
                // When finished, check board for matches
                if ( )
                break;
            case SwapFail:
                // Update swap animation
                // When finished, await user move
                break;
            case CheckBoard:
                // Check for matches
                break;
            case Falling:
            {
                                    
                    if ( doneFalling )
                        state = WaitForMove;
                }
                break;
            }
            case Quit:
                continue;
                break;
                
            default:
                break;
        }
        
        
        m_pPainter->drawBackground();
        
        for ( Unit x = 0; x < BOARD_SIZE; ++x )
        {
            for ( Unit y = 0; y < BOARD_SIZE; ++y )
            {
                Position p( x, y );
                PieceType type = board.getPieceType( p );
                PieceState state = viewModel[p];
                m_pPainter->drawTile(m_tileSize * (x + state.dx),
                                     m_tileSize * (y + state.dy),
                                     0,
                                     type,
                                     state.scale );
            }
        }
        
        m_pPainter->swap();
        
        ++animFrame;
    }
    
    return NoError;
}