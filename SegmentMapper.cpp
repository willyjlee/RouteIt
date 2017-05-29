#include "provided.h"
#include <vector>

#include "MyMap.h"
#include "support.h"
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
    
    void addto(GeoCoord& gc, StreetSegment& seg);
private:
    MyMap<GeoCoord, vector<StreetSegment>>map;
};

SegmentMapperImpl::SegmentMapperImpl()
{
}

SegmentMapperImpl::~SegmentMapperImpl()
{
}

void SegmentMapperImpl::init(const MapLoader& ml)
{
    size_t nsegs=ml.getNumSegments();
    StreetSegment s;
    GeoCoord st;
    GeoCoord end;
    for(size_t i=0;i<nsegs;i++){
        ml.getSegment(i, s);
        
        st=s.segment.start; // start is on this segment
        end=s.segment.end;  // end is on this segment
        addto(st, s);   // add this segment as related with start
        
        if(st!=end)
            addto(end, s);  // add this segment as related with end
        
        GeoCoord atc;
        for(int j=0;j<s.attractions.size();j++){    // loop all attractions
            atc=s.attractions[j].geocoordinates;
            if(atc!=st && atc!=end)
                addto(atc, s);      // add this segment to this attraction
        }
    }
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
    vector<StreetSegment> segs;
    
    const vector<StreetSegment>* f=map.find(gc);    // find the related StreetSegments with this GeoCoord
    if(f==nullptr)
        return segs;
    return *f;
}

void SegmentMapperImpl::addto(GeoCoord& gc, StreetSegment& seg){
    vector<StreetSegment>* vptr = map.find(gc);
    if(vptr==nullptr){          // if no related segments, add a new vector
        vector<StreetSegment> v;
        v.push_back(seg);
        map.associate(gc, v);
        return;
    }
    vptr->push_back(seg);   // add to vector of related StreetSegments
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
