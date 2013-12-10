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
    const int animSwapLength = 10;
    const int animDestroyLength = 10;

    // Animation frame counter
    uint animFrame = 0;

    PieceType pieces[8][8] = {
        { 0, 1, 0, 1, 0, 1, 0, 1 },
        { 3, 3, 3, 3, 3, 3, 3, 3 },
        { 0, 1, 0, 1, 0, 1, 0, 1 },
        { 3, 3, 3, 3, 3, 3, 3, 3 },
        { 0, 1, 0, 1, 0, 1, 0, 1 },
        { 3, 3, 3, 3, 3, 3, 3, 3 },
        { 0, 1, 0, 1, 0, 1, 0, 1 },
        { 3, 3, 3, 3, 3, 3, 3, 3 },
    };
    
    Board b;
    b.setPieces( pieces );
    
    // Setup random board
    BoardModel board;
    board.setBoard( b );
    //board.randomize();
    
    BoardView boardView;
    
    // Let the pieces fall from above
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

                            Position other = currentTile.shifted( swapDX, swapDY );

                            // Is move possible?
                            if ( board.move( currentTile, other ) )
                            {
                                state = SwapTest;
                                animFrame = 0;
                                
                                // Did move result in a match?
//                                if ( board.findMathces() )
//                                {
//                                    state = SwapTest;
//                                }
//                                else
//                                {
//                                    state = SwapFail;
//                                }
                            }
                            else
                            {
                                // Drop tile
                                boardView.resetState( currentTile );
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
                    boardView.animateGrab( currentTile, animFrame / (float)animGrabLength );
                }
                break;
            }
            case SwapTest:
            case SwapFail:
            {
                // Playing animation?
                if ( animFrame <= animSwapLength )
                {
                    boardView.animateSwap(currentTile,
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
                    boardView.animateDestroying( board.getMatches(), animFrame / (float)animDestroyLength );
                }
                // Make ready for falling peaces
                else
                {
                    // Reset animation counter
                    animFrame = -1;
                    
                    // Get list of mathces
                    const IndexSet& matches = board.getMatches();
                    
                    // Reset destroy state
                    boardView.animateDestroying( matches, 0 );
                    // Move all pieces up to where they fall from
                    boardView.prepareFall( matches );
                    
                    // Clear matches
                    board.flushMatches();
                    state = Falling;
                }
                break;
            }
            case Falling:
            {
                // Update falling pices
                bool done = boardView.animateFalling( animFrame );
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
        boardView.draw( m_pPainter, &board );
        
        ++animFrame;
    }
    
    return NoError;
}