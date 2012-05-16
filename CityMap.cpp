#include "CityMap.h"
#include "Algorithm/DrawAlgorithm/DrawAlgorithm.h"
#include "Algorithm/HeightsAlgorithm/HeightsAlgorithm.h"
#include "Algorithm/MapElementsAlgorithm/MapElementsAlgorithm.h"


using namespace std;

CityMap::CityMap(uint width, uint height):
    _size(width, height),
    _lands(width, height),
    _junctions(width+1, height+1),
    _drawAlgo(),
    _heightsAlgo(),
    _mapElementsAlgo()
{
}

void CityMap::setDrawAlgorithm(DrawAlgorithm *algorithm)
{
    _drawAlgo.reset(algorithm);
}

void CityMap::setHeightsAlgorithm(HeightsAlgorithm *algorithm)
{
    _heightsAlgo.reset(algorithm);
}

void CityMap::setMapElementsAlgorthm(MapElementsAlgorithm *algorithm)
{
    _mapElementsAlgo.reset(algorithm);
}

DrawAlgorithm& CityMap::drawAlgorithm()
{
    return *_drawAlgo;
}

HeightsAlgorithm& CityMap::heightsAlgorithm()
{
    return *_heightsAlgo;
}

MapElementsAlgorithm& CityMap::mapElementsAlgorithm()
{
    return *_mapElementsAlgo;
}

void CityMap::setup()
{
    setJunctions();

    if( _heightsAlgo )
        _heightsAlgo->setup( *this );

    if( _mapElementsAlgo )
        _mapElementsAlgo->setup( *this );

    if( _drawAlgo )
        _drawAlgo->setup( *this );
}

bool CityMap::load(const string &fileName)
{
    return false;
}

bool CityMap::save(const string &fileName)
{
    return false;
}

void CityMap::setJunctions()
{
    for(unsigned int j=0; j<_junctions.height(); ++j)
        for(unsigned int i=0; i<_junctions.width(); ++i)
            _junctions.set(i, j, new Junction());
}
