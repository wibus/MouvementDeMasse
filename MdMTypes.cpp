#include "MdMTypes.h"

CardinalDirection turnCW(CardinalDirection dir)
{
    switch(dir)
    {
    case EAST :  return SOUTH;
    case WEST :  return NORTH;
    case NORTH : return EAST;
    case SOUTH : return WEST;
    default : return DIRECTION_ERROR;
    }
}

CardinalDirection turnCCW(CardinalDirection dir)
{
    switch(dir)
    {
    case EAST :  return NORTH;
    case WEST :  return SOUTH;
    case NORTH : return WEST;
    case SOUTH : return EAST;
    default : return DIRECTION_ERROR;
    }
}

CardinalDirection toDirection(const glm::ivec2& vec)
{
         if(vec == glm::ivec2( 1,  0)) return EAST;
    else if(vec == glm::ivec2(-1,  0)) return WEST;
    else if(vec == glm::ivec2( 0,  1)) return NORTH;
    else if(vec == glm::ivec2( 0, -1)) return SOUTH;

    return DIRECTION_ERROR;
}

glm::ivec2 toVec(CardinalDirection dir)
{
    switch(dir)
    {
    case EAST : return glm::ivec2(1, 0);
    case WEST : return glm::ivec2(-1, 0);
    case NORTH : return glm::ivec2(0, 1);
    case SOUTH : return glm::ivec2(0, -1);
    default : return glm::ivec2(0, 0);
    }
}
