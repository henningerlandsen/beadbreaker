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
    XCTAssertTrue( Position(1,5) < Position(2,3), @"Test column priority");
    XCTAssertFalse( Position(1,5) > Position(2,3), @"Test column priority");
}

- (void)testValidity
{

    XCTAssertFalse( Position().isValid(), @"Position should be invalid by default" );
    XCTAssertFalse( Position(0,8).isValid(), @"Position should be out of bounds");
    XCTAssertFalse( Position(8,0).isValid(), @"Position should be out of bounds");
    XCTAssertTrue( Position(0,0).isValid(), @"Should be valid position");
    XCTAssertTrue( Position(0,7).isValid(), @"Should be valid position");
    XCTAssertTrue( Position(7,0).isValid(), @"Should be valid position");
    XCTAssertTrue( Position(5,5).isValid(), @"Should be valid position");
    
}

@end
