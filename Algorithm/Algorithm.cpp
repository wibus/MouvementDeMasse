#include "Algorithm.h"
#include "City/City.h"

Algorithm::Algorithm():
    _city(0x0),
    _mapSize(0, 0)
{
}

Algorithm::~Algorithm()
{
}

void Algorithm::setup(City &city)
{
    _city = &city;
    _ground  = &(city.ground());
    _mapSize(_city->junctions().width(),
             _city->junctions().height());
}
