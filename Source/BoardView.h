//
//  ViewModel.h
//  BeadBreaker
//
//  Created by Henning Erlandsen on 08.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#ifndef __BeadBreaker__BoardView__
#define __BeadBreaker__BoardView__

#include "Board.h"

class Painter;
class BoardModel;
class Sprite;

/**
 * BoardView
 * Keeps track of board pieces position
 * on the screen.
 */
class BoardView
{
public:
    BoardView( Painter* painter );
    ~BoardView();
    
    //! Update grab animation on index a to progress [0-1]
    void        animateGrab( const Index &a, float progress );
    
    //! Update swap animation on indices to progress [0-1]
    void        animateSwap( const Index &a, const Index &b, float progress );
    
    //! Translates all the pieces from index and up the given number of rows up.
    void        moveUpColumn( Index from, int rows );
    
    //! Translates all the pieces and the ones above to where they should fall
    //! from.
    void        prepareFall( const IndexSet &pieces );
    
    //! Update falling animation. Returns true when no more pieces are falling.
    bool        animateFalling( int frame );
    
    //! Update destroying animation on given indices to progress [0-1]
    void        animateDestroying( const IndexSet& set, float progress );
    
    //! Resets the translation and scale of the tile.
    void        resetState( const Index &pos );
    
    //! Draws the scene
    void        draw( const BoardModel* board, unsigned int time ) const;
    
    //! Set the spritesheet containing bead sprites
    void        setBeadSprite( Sprite* sprite );
    
    // Set the position of the board on the screen
    void        setBoardPosition( int left, int top );
    
    //! Set the background image
    void        setBackgroundSrite( Sprite* sprite );
    
    //! Sets sprite used to draw numbers.
    void        setNumberSprite( Sprite* sprite );
    
    //! Sets the sprite to flash when ten seconds remain
    void        setWarningSprite( Sprite* sprite );
    
    // Get the index located at the given screen position.
    Index       mapToIndex( int windowX, int windowY ) const;
    
    // Set the painter used to draw the game
    void        setPainter( Painter* painter );

    
private:
    struct PieceState
    {
        PieceState() // quick hack to make pieces fall on start
        : dx(0), dy(-9.0f), scale(1.0f), frame(0), endFrame(0) {}
        
        float dx;
        float dy;
        float scale;
        int   frame;
        int   endFrame;
    };
    
    struct ViewModel
    {

        PieceState& operator []( const Index &p )
        {
            return pieces[p.x][p.y];
        }
        
        const PieceState& operator []( const Index &p ) const
        {
            return pieces[p.x][p.y];
        }
        
        PieceState pieces[BOARD_SIZE][BOARD_SIZE];
    };
    
    
    ViewModel   m_viewModel;
    Sprite*     m_pBeadSprite;
    Sprite*     m_pBackground;
    Sprite*     m_pNumberSprite;
    Sprite*     m_pWarningSprite;
    bool        m_showWarning;
    Painter*    m_pPainter;
    int         m_boardX;
    int         m_boardY;
};



#endif /* defined(__BeadBreaker__BoardView__) */
