//
//  PositionTest.m
//  BeadBreaker
//
//  Created by Henning Erlandsen on 06.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#import <XCTest/XCTest.h>

#include "coretypes.h"

@interface PositionTest : XCTestCase

@end

@implementation PositionTest

- (void)setUp
{
    [super setUp];
    // Put setup code here. This method is called before the invocation of each test method in the class.
}

- (void)tearDown
{
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testOperators
{
    Position p1( 0, 0 );
    Position p2( 0, 1 );
    Position p3( 1, 0 );
    
    XCTAssertTrue( p1 < p2 );
    XCTAssertTrue( p2 > p1 );
    XCTAssertTrue( p1 == p1 );
    XCTAssertFalse( p1 == p2 );
    XCTAssertTrue( p1 != p2 );
    XCTAssertTrue( p3 > p1 );
    XCTAssertTrue( p3 > p2 );
}

@end
