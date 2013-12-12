//
//  Game.h
//  BeadBreaker
//
//  Created by Henning Erlandsen on 08.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#ifndef __BeadBreaker__Game__
#define __BeadBreaker__Game__


class BoardView;

class Game
{
    
public:
    enum ExitCode
    {
        Exit = 0,
        Error = 1
    };
    
public:
    Game( BoardView* m_pView );
    ~Game();
    
    int run();
    
private:
    enum GameState
    {
        WaitForMove,
        Grabbing,
        SwapTest,
        SwapFail,
        CheckBoard,
        DestroyMatches,
        Falling,
        Quit,
        GameOver,
    };
    
    BoardView*  m_pView;
};

#endif /* defined(__BeadBreaker__Game__) */
