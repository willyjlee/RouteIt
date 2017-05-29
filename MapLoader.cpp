#include "provided.h"
#include <string>

#include <vector>
#include <fstream>

using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
    
    int lastchar(string s, char c);
    string read(int& ind, const string& s);
private:
    vector<StreetSegment*> segs;
};

MapLoaderImpl::MapLoaderImpl()
{
}

MapLoaderImpl::~MapLoaderImpl()
{
    for(int i=0;i<segs.size();i++){
        delete segs[i]; // deallocate all pointers to StreetSegments
    }
}

int MapLoaderImpl::lastchar(string s, char c){  // helper to find last occurrence of a character in string. Used to find pipeline
    for(int i=s.length()-1;i>=0;i--)
        if(s[i]==c)
            return i;
    return -1;
}

string MapLoaderImpl::read(int& ind, const string& s){
    string str="";
    while(ind<s.length() && s[ind]!=',' && s[ind]!=' '){    // pass through a number
        str+=s[ind];
        ind++;
    }
    while(ind<s.length() && (s[ind]==',' || s[ind]==' '))   // pass through commas or spaces
        ind++;
    return str;
}

bool MapLoaderImpl::load(string mapFile)
{
	//return false;  // This compiles, but may not be correct
    ifstream in(mapFile);
    if(!in)
        return false;
    
    string name;
    StreetSegment* add;
    string segline;
    string lat;
    string longt;
    string atline;
    while(getline(in, name)){   // read the name of the StreetSegment
        add=new StreetSegment;
        
        add->streetName = name;
        
        getline(in, segline);
        
        int ind=0;
        lat=read(ind, segline);
        longt=read(ind, segline);
        GeoCoord a(lat, longt);
        
        lat=read(ind, segline);
        longt=read(ind, segline);
        GeoCoord b(lat, longt);
        
        GeoSegment seg(a,b);
        add->segment = seg;
        
        int num=0;
        in >> num;
        in.ignore(10000, '\n');
        vector<Attraction> attr;
        for(int i=0;i<num;i++){ // read attractions
            getline(in, atline);
            int p=lastchar(atline, '|');    // find index of the pipeline
            
            ind=p+1;
            lat=read(ind, atline);
            longt=read(ind, atline);
            GeoCoord acoord(lat, longt);
            
            Attraction addat;
            addat.name = atline.substr(0,p);
            addat.geocoordinates = acoord;
            
            attr.push_back(addat);  // add the Attraction to the vector
        }
        
        add->attractions = attr;
        segs.push_back(add);    // add a new StreetSegment here
    }
    return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	//return 0; // This compiles, but may not be correct
    return segs.size();
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
    if(segNum<0 || segNum>=segs.size())
        return false;
    seg = *segs[segNum];
    return true;
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
