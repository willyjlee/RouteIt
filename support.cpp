//
//  support.cpp
//  Project4
//
//  Created by William Lee on 3/15/17.
//  Copyright © 2017 williamlee. All rights reserved.
//

#include "provided.h"
#include "support.h"
#include <string>

bool operator>(const GeoCoord& a, const GeoCoord& b){
    return a.latitude>b.latitude || (a.latitude==b.latitude && a.longitude > b.longitude);
}

bool operator<(const GeoCoord& a, const GeoCoord& b){
    return a.latitude<b.latitude || (a.latitude==b.latitude && a.longitude < b.longitude);
}

bool operator==(const GeoCoord& a, const GeoCoord& b){
    return a.latitude==b.latitude && a.longitude==b.longitude;
}

bool operator!=(const GeoCoord& a, const GeoCoord& b){
    return a.latitude!=b.latitude || a.longitude!=b.longitude;
}

std::string directionOfLine(const GeoSegment& gs){
    double a=angleOfLine(gs);   // find angle of the GeoSegment
    if(0<=a && a<=22.5)     // separate into cases based on angle
        return "east";
    if(22.5<a && a<=67.5)
        return "northeast";
    if(67.5<a && a<=112.5)
        return "north";
    if(112.5<a && a<=157.5)
        return "northwest";
    if(157.5<a && a<=202.5)
        return "west";
    if(202.5<a && a<=247.5)
        return "southwest";
    if(247.5<a && a<=292.5)
        return "south";
    if(292.5<a && a<=337.5)
        return "southeast";
    if(337.5<a && a<360)
        return "east";
    return "";
}
