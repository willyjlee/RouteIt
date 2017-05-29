#include "provided.h"
#include <string>

#include "MyMap.h"
#include <string>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
    string lower(string s) const;
private:
    MyMap<string, GeoCoord> map;
};

AttractionMapperImpl::AttractionMapperImpl()
{
}

AttractionMapperImpl::~AttractionMapperImpl()
{
}

void AttractionMapperImpl::init(const MapLoader& ml)
{
    size_t nsegs=ml.getNumSegments();
    StreetSegment s;
    for(size_t i=0;i<nsegs;i++){    // loop all segments
        ml.getSegment(i, s);    // get the segment
        for(int j=0;j<s.attractions.size();j++){
            map.associate(lower(s.attractions[j].name), s.attractions[j].geocoordinates);   // store the attraction in the map
        }
    }
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
    const GeoCoord* get=map.find(lower(attraction));    // find the coordinate of the attraction
    if(get!=nullptr){
        gc=*get;
        return true;
    }
    return false;
}

string AttractionMapperImpl::lower(string s) const{
    for(int i=0;i<s.length();i++)   // to ensure case doesn't matter in attraction names
        s[i]=tolower(s[i]);
    return s;
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
