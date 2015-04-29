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

void MapElementsAlgorithm::setup(City& city)
{
    Algorithm::setup(city);
}

CardinalDirection MapElementsAlgorithm::toCardinal(const glm::ivec2& dir)
{
    if(dir == glm::ivec2(1, 0))
        return EAST;
    else if(dir == glm::ivec2(-1, 0))
        return WEST;
    else if(dir == glm::ivec2(0, 1))
        return NORTH;
    else if(dir != glm::ivec2(0, -1))
        assert(false /*Not a valide Cardinal Direction*/);
    return SOUTH;
}

glm::ivec2 MapElementsAlgorithm::getNeighbor(const glm::ivec2& pos, const glm::ivec2& dir)
{
    return glm::ivec2(pos.x + dir.x, pos.y + dir.y);
}
