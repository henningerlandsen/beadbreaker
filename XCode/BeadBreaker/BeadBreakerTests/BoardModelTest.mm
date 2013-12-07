//
//  BeadBreakerTests.m
//  BeadBreakerTests
//
//  Created by Henning Erlandsen on 04.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#import <XCTest/XCTest.h>

#include "BoardModel.h"


@interface BoardModelTest : XCTestCase
{
    BoardModel m_boardModel;
}
@end

@implementation BoardModelTest

- (void)setUp
{
    [super setUp];
    
    PieceType pieces[8][8] = {
    // x: 0  1  2  3  4  5  6  7       y:
        { 0, 1, 2, 3, 4, 0, 1, 2 }, // 0
        { 3, 4, 0, 1, 2, 3, 4, 0 }, // 1
        { 1, 2, 1, 4, 0, 1, 2, 3 }, // 2
        { 4, 1, 1, 2, 3, 4, 0, 1 }, // 3
        { 2, 3, 4, 0, 1, 2, 3, 4 }, // 4
        { 0, 1, 2, 3, 4, 0, 1, 2 }, // 5
        { 3, 4, 0, 1, 2, 3, 4, 0 }, // 6
        { 1, 2, 3, 4, 0, 1, 2, 3 }, // 7
    };
    
    Board b;
    b.setPieces( pieces );
    
    m_boardModel.setBoard( b );
    
}

- (void)tearDown
{
    [super tearDown];
}

- (void)testInvalidMove
{
    Position p( 0, 2 );
    
    // Moving Left to (-1,2) is invalid ( out of bounds )
    XCTAssertFalse( m_boardModel.tryMove( p, Left ), @"Piece moved out of bounds" );
    
    // Moving Up to (0,1) is invalid ( no match )
    XCTAssertFalse( m_boardModel.tryMove( p, Up ), @"Made non-matching move" );
    
}

- (void)testValidMove
{
    
    Position p( 1, 3 );
    // Move [1,3] up to form a row of 1's at [0,2], [1,2], [2,2]
    XCTAssertTrue( m_boardModel.tryMove( p, Up ), @"Should be winning...");
    
    // Check that correct number of pieces was removed
    XCTAssertTrue( m_boardModel.getRemovedIndexes().size() == 3, @"Wrong number of mathces" );
    
    // Check that [1,3] now holds the initial value of [1,2]
    XCTAssertTrue( m_boardModel.getPieceType( p ) == 2, @"Swap produced incorrect state");
    
    // Check that span [0-2,1] is moved to [0-2,2]
    Position f1( 0, 2 ), f2( 1, 2 ), f3( 2, 2 );
    XCTAssertTrue( m_boardModel.getPieceType( f1 ) == 3 );
    XCTAssertTrue( m_boardModel.getPieceType( f2 ) == 4 );
    XCTAssertTrue( m_boardModel.getPieceType( f3 ) == 0 );
    
}

@end
