#include "MapElementsAlgorithm.h"

#include <cassert>

using namespace cellar;


MapElementsAlgorithm::MapElementsAlgorithm()
{
    _typesOfLand[0] = Land::GRASS;
    _typesOfLand[1] = Land::RESIDENTIAL;
    _typesOfLand[2] = Land::COMMERCIAL;
}

MapElementsAlgorithm::~MapElementsAlgorithm()
{
}

void MapElementsAlgorithm::setup(CityMap& cityMap)
{
    Algorithm::setup(cityMap);
}

CardinalDirection MapElementsAlgorithm::toCardinal(const Vec2i& dir)
{
    if(dir == cellar::Vec2i(1, 0))
        return EAST;
    else if(dir == cellar::Vec2i(-1, 0))
        return WEST;
    else if(dir == cellar::Vec2i(0, 1))
        return NORTH;
    else if(dir != cellar::Vec2i(0, -1))
        assert(false /*Not a valide Cardinal Direction*/);
    return SOUTH;
}

Vec2i MapElementsAlgorithm::getNeighbor(const Vec2i& pos, const Vec2i& dir)
{
    return Vec2i(pos.x() + dir.x(), pos.y() + dir.y());
}
