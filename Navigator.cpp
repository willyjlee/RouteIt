#include "provided.h"
#include <string>
#include <vector>
#include <iostream>
#include <queue>
#include "MyMap.h"
#include "support.h"
#include <stack>
using namespace std;

// put snode in support.h
class snode{    // snode for storing state in A*
public:
    snode(string st, GeoSegment gs, GeoSegment p, double d, GeoCoord c, GeoCoord e){
        street=st;
        seg=gs;
        path=p;
        dist=d;
        coord=c;
        end=e;
    }
    snode(){
    }
    double getdist() const{
        return dist;
    }
    string getstreet() const{   // part of dis street
        return street;
    }
    GeoSegment getseg() const{
        return seg;
    }
    GeoCoord getcoord() const{
        return coord;
    }
    GeoSegment getpath() const{
        return path;
    }
    double segdist() const{
        return distanceEarthMiles(path.start, path.end);
    }
    double heuristic() const{
        return distanceEarthMiles(coord, end);
    }
private:
    double dist;    // miles to center
    string street;
    GeoSegment seg;
    GeoCoord coord;
    GeoSegment path;
    GeoCoord end;
};

bool operator==(const GeoSegment& l, const GeoSegment& r){

    return l.start==r.start && l.end==r.end;
}

bool operator!=(const GeoSegment& l, const GeoSegment& r){
    return l.start!=r.start || l.end!=r.end;
}

bool operator>(const GeoSegment& l, const GeoSegment& r){
    return l.start>r.start
    || (l.start==r.start && l.end>r.end);
}

bool operator<(const GeoSegment& l, const GeoSegment& r){
    return l.start<r.start
    || (l.start==r.start && l.end<r.end);
}

bool operator==(const snode& l, const snode& r){
    double ld=l.getdist()+l.heuristic();
    double rd=r.getdist()+r.heuristic();
    
    return l.getstreet()==r.getstreet() && l.getseg()==r.getseg()
    && ld==rd && l.getcoord()==r.getcoord();
}

bool operator>(const snode& l, const snode& r){ // comparison mainly by distance

    double ld=l.getdist()+l.heuristic();
    double rd=r.getdist()+r.heuristic();
    
        return ld<rd
        || (ld==rd && l.getstreet()<r.getstreet())
        || (ld==rd && l.getstreet()==r.getstreet() && l.getseg()<r.getseg())
        || (ld==rd && l.getstreet()==r.getstreet() && l.getseg()==r.getseg()
            && l.getcoord()<r.getcoord());
}

bool operator<(const snode& l, const snode& r){ // comparison mainly by distance
   
    double ld=l.getdist()+l.heuristic();
    double rd=r.getdist()+r.heuristic();
    
    return ld>rd
    || (ld==rd && l.getstreet()>r.getstreet())
    || (ld==rd && l.getstreet()==r.getstreet() && l.getseg()>r.getseg())
    || (ld==rd && l.getstreet()==r.getstreet() && l.getseg()==r.getseg()
        && l.getcoord()>r.getcoord());
}


class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions); // const?
    void filldirs(snode* n, vector<NavSegment>& directions);
    bool swapped(const GeoSegment& a, const GeoSegment& b);
    //string tostr(double angle);
    string turn(double angle);
private:
    AttractionMapper atmap;
    SegmentMapper segmap;
    MapLoader loader;
    MyMap<snode, snode>previ;
    MyMap<GeoCoord, bool>vis;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	//return false;  // This compiles, but may not be correct
    bool v=loader.load(mapFile);
    if(!v){
        return false;
    }
    atmap.init(loader); // initialize the map of attractions
    segmap.init(loader);    // intialize the map of segments
    
    return true;
}

void NavigatorImpl::filldirs(snode* n, vector<NavSegment>& directions){
    directions.clear();
    stack<snode*>st;
    snode* x=n;
    while(!x->getstreet().empty()){ // read the path in reverse with a stack
        st.push(x);
        x=previ.find(*x);
    }
    //st.pop();
    while(!st.empty()){
        x=st.top(); // get top element
        st.pop();   // pop top element
        // dist is not dist of seg;
        NavSegment proc(directionOfLine(x->getpath()), x->getstreet(), x->segdist(),
                       x->getpath());   // a proceed segment
        directions.push_back(proc); // add to vector
        if(!st.empty()){
            snode* nxt=st.top();
            if(nxt->getstreet()!=x->getstreet()){   // must add a turn since street names are different
                NavSegment t(
                    turn(angleBetween2Lines(x->getpath(), nxt->getpath())),
                         nxt->getstreet()); // a turn style nav segment
                directions.push_back(t);    // add to vector
            }
        }
    }
}

string NavigatorImpl::turn(double a){
    if(a<180 && a>=0)
        return "left";
    if(a>=180)
        return "right";
    return "";
}

bool NavigatorImpl::swapped(const GeoSegment& a, const GeoSegment& b){
    return (a.start==b.start && a.end==b.end)
    || (a.start==b.end && a.end==b.start);
}

NavResult NavigatorImpl::navigate(string start, string endstr, vector<NavSegment> &directions)
{
	//return NAV_NO_ROUTE;  // This compiles, but may not be correct
    
    GeoCoord st;
    if(!atmap.getGeoCoord(start, st))
        return NAV_BAD_SOURCE;
    
    GeoCoord end;
    if(!atmap.getGeoCoord(endstr, end))
        return NAV_BAD_DESTINATION;
    
    GeoSegment gstart;
    snode n("", gstart, gstart, 0.0, st, end);  // initialize starting node

    priority_queue<snode>q;
    q.push(n);  // add one starting node
    
    while(!q.empty()){
        const snode t=q.top();
        q.pop();
        
        if(vis.find(t.getcoord())!=nullptr) // if already visited, just continue
            continue;
        bool b=true;
        vis.associate(t.getcoord(), b); // mark visited
        
        const vector<StreetSegment>& segs=segmap.getSegments(t.getcoord()); // get segments this GeoCoord is related to
        
        for(int i=0;i<segs.size();i++){
            if(previ.find(t)==nullptr || !swapped(segs[i].segment,t.getseg())){
                // check attractions first
                for(int j=0;j<segs[i].attractions.size();j++){
                    if(segs[i].attractions[j].geocoordinates==end){
                        
                        GeoSegment gadd(t.getcoord(), end);
                        snode add(segs[i].streetName, gadd, gadd, t.getdist()+
                                 distanceEarthMiles(t.getcoord(), end), end, end);
                        previ.associate(add, t);
                        
                        filldirs(&add, directions); // retrace the path, then complete
                        return NAV_SUCCESS;
                    }
                }
                if(segs[i].segment.start!=t.getcoord()){    // can go to start of segment
                    GeoSegment gadd(segs[i].segment.end, segs[i].segment.start);
                    GeoSegment padd(t.getcoord(), segs[i].segment.start);
                    
                    snode add(segs[i].streetName, gadd, padd, t.getdist()+
                        distanceEarthMiles(t.getcoord(), segs[i].segment.start), segs[i].segment.start, end);
                    q.push(add);            // push to priority queue
                    previ.associate(add, t);    // set previous of new node to current node
                    
                }
                if(segs[i].segment.end!=t.getcoord()){  // can go to end of segment
                    GeoSegment gadd(segs[i].segment.start, segs[i].segment.end);
                    GeoSegment padd(t.getcoord(), segs[i].segment.end);
                    
                    snode add(segs[i].streetName, gadd, padd, t.getdist()+
                              distanceEarthMiles(t.getcoord(), segs[i].segment.end), segs[i].segment.end, end);
                    q.push(add);            // push to priority queue
                    previ.associate(add, t);    // set previous of new node to current node
                }
            }
        }
    }
    
    
    return NAV_NO_ROUTE;    // no route found
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
