#include "CityMap.h"
#include "Algorithm/DrawAlgorithm/DrawAlgorithm.h"
#include "Algorithm/HeightsAlgorithm/HeightsAlgorithm.h"
#include "Algorithm/MapElementsAlgorithm/MapElementsAlgorithm.h"

#include "Road/Junction.h"

#include <cmath>
using namespace std;
using namespace cellar;


CityMap::CityMap(int width, int height):
    _size(width, height),
    _sun(Vec4f(-1, -1, 2, 0), Vec3f(-1.0, -1.0, -0.5)),
    _ground(   width+1, height+1),
    _lands(    width,   height),
    _junctions(width+1, height+1)
{
}

void CityMap::reset()
{
    resetJunctions();
}

bool CityMap::load(const string &)
{
    return false;
}

bool CityMap::save(const string &)
{
    return false;
}

void CityMap::resetJunctions()
{
    for(int j=0; j<_junctions.height(); ++j)
    {
        for(int i=0; i<_junctions.width(); ++i)
        {
            Junction* junc = _junctions.get(i, j);
            if(junc != 0x0)
                delete junc;
            _junctions.set(i, j, new Junction());
        }
    }
}