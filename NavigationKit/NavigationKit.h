//
//  NavigationKit.h
//  Pods
//
//  Created by Axel Möller on 11/12/14.
//  Copyright (c) 2014 Sendus Sverige AB. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <MapKit/MapKit.h>

#import "NKRoute.h"
#import "NKRouteStep.h"

typedef enum NavigationKitDirectionsService {
    NavigationKitDirectionsServiceAppleMaps,
    NavigationKitDirectionsServiceGoogleMaps
} NavigationKitDirectionsService;

// Used in navigationKitEnteredRouteStep to let the delegate know what distance the notification is for.
typedef NS_ENUM(NSInteger, NavigationKitNotificationDistanceType) {
  NavigationKitNotificationDistanceTypeNewDirection = 0,
  NavigationKitNotificationDistanceTypeSmall,
  NavigationKitNotificationDistanceTypeMedium,
  NavigationKitNotificationDistanceTypeLarge
};

@protocol NavigationKitDelegate <NSObject>

- (void)navigationKitCalculatedRoute:(NKRoute *)route;
- (void)navigationKitError:(NSError *)error;
- (void)navigationKitStartedNavigation;
- (void)navigationKitEnteredRouteStep:(NKRouteStep *)step nextStep:(NKRouteStep *)nextStep;
- (void)navigationKitCalculatedDistanceToEndOfPath:(CLLocationDistance)distance;
- (void)navigationKitCalculatedDistanceToEndOfRoute:(CLLocationDistance)distance;
- (void)navigationKitCalculatedTimeToEndOfRoute:(NSTimeInterval)time;

- (void)navigationKitCalculatedNotificationForStep:(NKRouteStep *)step
                                        inDistance:(CLLocationDistance)distance
                                   forDistanceType:(enum NavigationKitNotificationDistanceType)type;
- (void)navigationKitCalculatedCamera:(MKMapCamera *)camera;
- (void)navigationKitStartedRecalculation;
- (void)navigationKitStoppedNavigation;
- (void)navigationKitArrivedAtDestination;

@end

@interface NavigationKit : NSObject

@property (nonatomic, assign) id<NavigationKitDelegate> delegate;

// User settings
@property (nonatomic, assign) NSInteger recalculatingTolerance;
@property (nonatomic, assign) NSInteger cameraAltitude;

@property (nonatomic) BOOL isNavigating;

// Customize the distance for the next turn reminders. Up to three reminders are sent per next turn:
// 1: When the user enters the new path, e.g. they just turned onto a street and need to hear their next direction.
// 2: When the user is EITHER medium or large distance from the next turn.
// 3: When the user is small distance from the next turn.
@property (nonatomic) CGFloat nextTurnNotifSmallEtaSeconds;

@property (nonatomic) CGFloat nextTurnNotifMediumEtaSeconds;

@property (nonatomic) CGFloat nextTurnNotifLargeEtaSeconds;

@property (nonatomic, strong) NSObject *googleApiKey;

- (id)initWithSource:(CLLocationCoordinate2D)source destination:(CLLocationCoordinate2D)destination transportType:(MKDirectionsTransportType)transportType directionsService:(NavigationKitDirectionsService)directionsService;

- (void)calculateDirectionsWithHeading:(CLLocationDirection)heading;

- (void)startNavigation;
- (void)stopNavigation;
- (void)recalculateNavigation;

- (void)calculateActionForLocation:(CLLocation *)location;
+ (CLLocationCoordinate2D)coordinate:(CLLocationCoordinate2D)fromCoordinate atDistance:(double)distance bearing:
    (double)bearing;
@end