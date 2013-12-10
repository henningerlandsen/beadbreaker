//
//  Game.h
//  BeadBreaker
//
//  Created by Henning Erlandsen on 08.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#ifndef __BeadBreaker__Game__
#define __BeadBreaker__Game__


class Painter;
class BoardModel;

class Game
{
    
public:
    enum ExitCode
    {
        NoError = 0,
        Error = 1
    };
    
public:
    Game( Painter* painter );
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
        Quit
    };
    
    Painter*    m_pPainter;
    int         m_tileSize;
};

#endif /* defined(__BeadBreaker__Game__) */
