#include "CityMap.h"
#include "Algorithm/DrawAlgorithm/DrawAlgorithm.h"
#include "Algorithm/HeightsAlgorithm/HeightsAlgorithm.h"
#include "Algorithm/MapElementsAlgorithm/MapElementsAlgorithm.h"


using namespace std;

CityMap::CityMap(uint width, uint height):
    _size(width, height),
    _lands(width, height),
    _junctions(width, height),
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

void CityMap::setup()
{
    return;
}

bool CityMap::load(const string &fileName)
{
    return false;
}

bool CityMap::save(const string &fileName)
{
    return false;
}
