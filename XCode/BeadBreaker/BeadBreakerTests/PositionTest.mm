//
//  PositionTest.m
//  BeadBreaker
//
//  Created by Henning Erlandsen on 06.12.13.
//  Copyright (c) 2013 Henning Erlandsen. All rights reserved.
//

#import <XCTest/XCTest.h>

#include "Board.h"

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
    XCTAssertTrue( Index(1,5) < Index(2,3), @"Test column priority");
    XCTAssertFalse( Index(1,5) > Index(2,3), @"Test column priority");
}

- (void)testValidity
{

    XCTAssertFalse( Index().isValid(), @"Index should be invalid by default" );
    XCTAssertFalse( Index(0,8).isValid(), @"Index should be out of bounds");
    XCTAssertFalse( Index(8,0).isValid(), @"Index should be out of bounds");
    XCTAssertTrue( Index(0,0).isValid(), @"Should be valid position");
    XCTAssertTrue( Index(0,7).isValid(), @"Should be valid position");
    XCTAssertTrue( Index(7,0).isValid(), @"Should be valid position");
    XCTAssertTrue( Index(5,5).isValid(), @"Should be valid position");
    
}

@end
