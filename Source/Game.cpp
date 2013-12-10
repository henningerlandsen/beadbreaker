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




Game::Game( BoardView* view )
: m_pView( view )
{
    
}



Game::~Game()
{
}



int
Game::run()
{
    // Animation constants:
    const int animGrabLength = 30;
    const int animSwapLength = 10;
    const int animDestroyLength = 10;

    // Animation frame counter
    uint animFrame = 0;
    
    // Setup random board
    BoardModel board;
    board.randomize();
    
    // Let the pieces fall from above
    GameState state = Falling;
    
    SDL_Point grabPosition;
    Position  currentTile(-1, -1);
    
    // Setup one minute timeout
    uint currentTime = SDL_GetTicks();
    uint maxTime     = currentTime + 60000;
    
    // Swapping direction
    int swapDX = 0;
    int swapDY = 0;
    
    SDL_Event e;
    
    // Run game loop
    while ( state != Quit )
    {
;
        
        if ( currentTime >= maxTime )
        {
            currentTime = maxTime;
            state = GameOver;
        }
        else
        {
            currentTime = SDL_GetTicks();
        }
        
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
                        currentTile = m_pView->mapToPiece( grabPosition.x, grabPosition.y );
                        
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
                        m_pView->resetState( currentTile );
                        
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

                            Position other = currentTile.shifted( swapDX, swapDY );

                            // Is move possible?
                            if ( board.move( currentTile, other ) )
                            {
                                state = SwapTest;
                                animFrame = 0;
                                
                            }
                            else
                            {
                                // Drop tile
                                m_pView->resetState( currentTile );
                                state = WaitForMove;
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
                    m_pView->animateGrab( currentTile, animFrame / (float)animGrabLength );
                }
                break;
            }
            case SwapTest:
            case SwapFail:
            {
                // Playing animation?
                if ( animFrame <= animSwapLength )
                {
                    m_pView->animateSwap(currentTile,
                                          currentTile.shifted( swapDX, swapDY ),
                                          animFrame / (float)animSwapLength );
                }
                else
                {
                    animFrame = -1;                  // Reset animation counter
                    
                    if (state == SwapTest)
                        if ( board.findMathces() )
                            state = DestroyMatches; // Mathces found
                        else
                        {
                            board.move(currentTile,
                                       currentTile.shifted( swapDX, swapDY ) );
                            state = SwapFail;       // Move back
                        }
                    else
                        state = WaitForMove;        // Finished moving back
                }
                break;
            }
            case CheckBoard:
            {
                if ( board.findMathces() )          // Any new mathces?
                {
                    animFrame = 0;
                    state = DestroyMatches;         // Destroy them as well
                }
                else
                    state = WaitForMove;            // Wait for next move
                break;
            }
            case DestroyMatches:
            {
                // Playing destroy animation?
                if ( animFrame <= animDestroyLength )
                {
                    m_pView->animateDestroying( board.getMatches(), animFrame / (float)animDestroyLength );
                }
                // Make ready for falling peaces
                else
                {
                    // Reset animation counter
                    animFrame = -1;
                    
                    // Get list of mathces
                    const IndexSet& matches = board.getMatches();
                    
                    // Reset destroy state
                    m_pView->animateDestroying( matches, 0 );
                    // Move all pieces up to where they fall from
                    m_pView->prepareFall( matches );
                    
                    // Clear matches
                    board.flushMatches();
                    state = Falling;
                }
                break;
            }
            case Falling:
            {
                // Update falling pices
                bool done = m_pView->animateFalling( animFrame );
                if ( done )
                    state = CheckBoard;
                break;
            }
            case Quit:
                continue;
                break;
                
            case WaitForMove:
            default:
                break;
        }
        
        // Draw the model
        m_pView->draw( &board, maxTime - currentTime );
        
        ++animFrame;
    }
    
    return NoError;
}