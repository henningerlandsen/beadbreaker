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
        { 4, 1, 1, 2, 3, 4, 0, 0 }, // 3
        { 2, 3, 4, 0, 1, 2, 3, 0 }, // 4
        { 0, 1, 2, 3, 4, 0, 0, 2 }, // 5
        { 3, 4, 0, 1, 2, 3, 4, 0 }, // 6
        { 1, 2, 3, 4, 0, 1, 2, 0 }, // 7
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
    
    XCTAssertFalse( m_boardModel.move( p, p.shifted( 1, 1 ) ), @"Made diagonal move");
    
    XCTAssertFalse( m_boardModel.move( p, p.shifted( 0, 3 ) ), @"Made long move");
    
    // Moving Left to (-1,2) is invalid ( out of bounds )
    XCTAssertFalse( m_boardModel.move( p, p.shifted(-1, 0) ), @"Piece moved out of bounds" );
}


- (void)testNoMatches
{
    XCTAssertFalse( m_boardModel.findMathces(), @"There should be no initial matches on this board");
}


- (void)testValidMove
{
    
    Position p( 1, 3 );
    // Move [1,3] up to form a row of 1's at [0,2], [1,2], [2,2]
    XCTAssertTrue( m_boardModel.move( p, p.shifted( 0, -1 ) ), @"Failed to make valid move");
    
    XCTAssertTrue(m_boardModel.findMathces() == 3, @"Sould be winning...");
    
    // Check that correct number of pieces was matched
    XCTAssertTrue( m_boardModel.getMatches().size() == 3, @"Wrong number of mathces" );
    
    // Check that [1,3] now holds the initial value of [1,2]
    XCTAssertTrue( m_boardModel.getPieceType( p ) == 2, @"Swap produced incorrect state");
    
    m_boardModel.flushMatches();
    
    XCTAssertTrue( m_boardModel.getMatches().empty(), @"Mathces should be cleared now");
    
    // Check that span [0-2,1] is moved down to [0-2,2]
    Position f1( 0, 2 ), f2( 1, 2 ), f3( 2, 2 );
    XCTAssertTrue( m_boardModel.getPieceType( f1 ) == 3 );
    XCTAssertTrue( m_boardModel.getPieceType( f2 ) == 4 );
    XCTAssertTrue( m_boardModel.getPieceType( f3 ) == 0 );
}


- (void)testAngularCombo
{
    Position p( 7, 4 );
    XCTAssertTrue( m_boardModel.move( p, p.shifted( 0, 1 ) ) );
    XCTAssertTrue( m_boardModel.findMathces() == 5, @"Angular Combo failed");
}


- (void)testFiveXCombo
{
    Position p( 6, 5 );
    XCTAssertTrue( m_boardModel.move( p, p.shifted( 1, 0 ) ) );
    XCTAssertTrue( m_boardModel.findMathces() == 5, @"FiveX Combo failed");
}

@end
