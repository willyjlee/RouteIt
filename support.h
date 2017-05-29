//
//  support.h
//  Project4
//
//  Created by William Lee on 3/13/17.
//  Copyright © 2017 williamlee. All rights reserved.
//

#ifndef support_h
#define support_h

#include "provided.h"
#include <string>

// implementations in support.cpp
bool operator>(const GeoCoord& a, const GeoCoord& b);   // GeoCoord compare greater than

bool operator<(const GeoCoord& a, const GeoCoord& b);   // GeoCoord compare less than

bool operator==(const GeoCoord& a, const GeoCoord& b);  // GeoCoord compare equals

bool operator!=(const GeoCoord& a, const GeoCoord& b);  // GeoCoord compare not equal

std::string directionOfLine(const GeoSegment& gs);  // give direction string given segment

#endif /* support_h */
