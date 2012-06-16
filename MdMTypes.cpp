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

CardinalDirection toDirection(const cellar::Vec2i& vec)
{
         if(vec == cellar::Vec2i( 1,  0)) return EAST;
    else if(vec == cellar::Vec2i(-1,  0)) return WEST;
    else if(vec == cellar::Vec2i( 0,  1)) return NORTH;
    else if(vec == cellar::Vec2i( 0, -1)) return SOUTH;

    return DIRECTION_ERROR;
}

cellar::Vec2i toVec(CardinalDirection dir)
{
    switch(dir)
    {
    case EAST : return cellar::Vec2i(1, 0);
    case WEST : return cellar::Vec2i(-1, 0);
    case NORTH : return cellar::Vec2i(0, 1);
    case SOUTH : return cellar::Vec2i(0, -1);
    default : return cellar::Vec2i(0, 0);
    }
}
