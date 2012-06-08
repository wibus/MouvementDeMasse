#include "Algorithm.h"
#include "CityMap.h"

Algorithm::Algorithm():
    _cityMap(0x0),
    _mapSize(0, 0),
    _minHeight(0.0f),
    _maxHeight(0.0f)
{
}

Algorithm::~Algorithm()
{
}

void Algorithm::setup(CityMap &cityMap)
{
    _cityMap = &cityMap;
    _ground  = &(cityMap.ground());
    _mapSize(_cityMap->junctions().width(),
             _cityMap->junctions().height());
    _minHeight = _ground->minHeight();
    _maxHeight = _ground->maxHeight();
}
