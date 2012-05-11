#include "Algorithm.h"
#include "CityMap.h"

Algorithm::Algorithm():
    _cityMap(0x0),
    _mapSize(0, 0)
{
}

Algorithm::~Algorithm()
{
}

void Algorithm::setup(CityMap &cityMap)
{
    _cityMap = &cityMap;
    _mapSize(_cityMap->size().x(), _cityMap->size().y());
}
