#include "Algorithm.h"

Algorithm::Algorithm():
    _cityMap(0x0)
{
}

Algorithm::~Algorithm()
{
}

void Algorithm::setup(CityMap &cityMap)
{
    _cityMap = &cityMap;
}
