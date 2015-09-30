//
//  NKRoute.h
//  Pods
//
//  Created by Axel Möller on 11/12/14.
//  Copyright (c) 2014 Sendus Sverige AB. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MapKit/MapKit.h>

@class MKPolyline;
@interface NKRoute : NSObject

@property (nonatomic, strong) NSArray *path;
@property (nonatomic, strong) MKPolyline *polyline;
@property (nonatomic, strong) NSArray *steps;
@property (nonatomic) NSTimeInterval expectedTravelTime;

@property (nonatomic) CLLocationDistance distance;

- (instancetype)initWithMKRoute:(MKRoute *)route;

- (instancetype)initWithGoogleMapsRoute:(NSDictionary *)route;
@end