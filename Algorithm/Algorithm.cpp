#include "Algorithm.h"


const CardinalDirection Algorithm::_cardinalDirections[NB_DIRECTIONS] =
    {EAST, WEST, NORTH, SOUTH};

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
    _description  = &(city.description());
    _mapSize(_city->junctions().getWidth(),
             _city->junctions().getHeight());
}
