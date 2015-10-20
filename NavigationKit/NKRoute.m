//
//  NKRoute.m
//  Pods
//
//  Created by Axel Möller on 11/12/14.
//  Copyright (c) 2014 Sendus Sverige AB. All rights reserved.
//

#import "NKRoute.h"
#import "NKRouteStep.h"

@implementation NKRoute

- (instancetype)initWithMKRoute:(MKRoute *)route {
    self = [super init];

    if(self && route) {

        // Convert Polyline coordinates to GMSPath
        NSMutableArray *path = [NSMutableArray arrayWithCapacity:[[route steps] count]];
        for(MKRouteStep *routeStep in [route steps]) {

            NSInteger stepPoints = routeStep.polyline.pointCount;
            CLLocationCoordinate2D *coordinates = malloc(stepPoints * sizeof(CLLocationCoordinate2D));
            [routeStep.polyline getCoordinates:coordinates range:NSMakeRange(0, stepPoints)];

            for(int i = 0; i < stepPoints; i++) {
                [path addObject:[NSValue valueWithMKCoordinate:coordinates[i]]];
            }
        }

        self.path = path;

        // Save polyline
        self.polyline = [route polyline];

        // Convert array of MKRouteStep's to NKRouteStep's
        NSMutableArray *routeSteps = [[NSMutableArray alloc] init];

        for(MKRouteStep *step in [route steps]) {
            NKRouteStep *routeStep = [[NKRouteStep alloc] initWithMKRouteStep:step];
            [routeSteps addObject:routeStep];
        }

        self.steps = routeSteps;

        // Save expectedTravelTime
        self.expectedTravelTime = [route expectedTravelTime];
        self.distance = route.distance;
    }

    return self;
}

- (instancetype)initWithGoogleMapsRoute:(NSDictionary *)route {
    self = [super init];

    if (self && route) {

        // Decode the Path and convert coordinates to MKPolyline
//        NSString *encodedPolyline = [[route valueForKey:@"overview_polyline"] valueForKey:@"points"];
//        GMSPath *gmsPath = [GMSPath pathFromEncodedPath:encodedPolyline];

        // Find all steps and convert to NKRouteStep's
        NSArray *legs = [route valueForKey:@"legs"];
        NSTimeInterval time = 0;
        NSMutableArray *routeSteps = [@[] mutableCopy];
        for (NSDictionary *leg in legs) {
            NSArray *steps = leg[@"steps"];
            for (NSDictionary *step in steps) {
                NKRouteStep *routeStep = [[NKRouteStep alloc] initWithGoogleMapsStep:step];
                // Remove the last step if it's a baby step because it is usually having us turn down an alley.
                if (step == steps.lastObject && routeStep.distance <= 60) continue;
                [routeSteps addObject:routeStep];

            }
            time += [[[leg valueForKey:@"duration"] valueForKey:@"value"] doubleValue];
        }

        NSMutableArray *path = [@[] mutableCopy];
        // Move all of the directions descriptions back one and remove the first direction description.
        for (NSUInteger i = 0; i < routeSteps.count; i++) {
            NKRouteStep *step = routeSteps[i];
            if (i + 1 < routeSteps.count) {
                NKRouteStep *nextStep = routeSteps[i + 1];
                step.instructions = nextStep.instructions;
                step.maneuver = nextStep.maneuver;
            } else {
                step.instructions = @"Arrived at destination.";
                step.maneuver = NKRouteStepManeuverArrived;
            }
            for (NSUInteger i = 0; i < step.gmsPath.count; i++) {
                [path addObject:[NSValue valueWithMKCoordinate:[step.gmsPath coordinateAtIndex:i]]];
            }
        }
        self.path = path;

        CLLocationCoordinate2D *coordinates = calloc([self.path count], sizeof(CLLocationCoordinate2D));

        for (int i = 0; i < [self.path count]; i++) {
            coordinates[i] = ((NSValue *)self.path[i]).MKCoordinateValue;
        }

        self.polyline = [MKPolyline polylineWithCoordinates:coordinates count:[self.path count]];

        free(coordinates);



        self.steps = routeSteps;

        // Find expectedTravelTime
        self.expectedTravelTime = time;
    }

    return self;
}
@end
