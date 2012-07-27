#ifndef MDMTYPES_H
#define MDMTYPES_H

#include <string>

#include <DataStructure/Vector.h>


enum CardinalDirection{EAST, WEST, NORTH, SOUTH, NB_DIRECTIONS, DIRECTION_ERROR};

const std::string CARDINAL_DIRECTION_STRINGS[NB_DIRECTIONS] = {
    "EAST", "WEST", "NORTH", "SOUTH"
};
const CardinalDirection INT_TO_CARDINAL_DIRECTION[NB_DIRECTIONS] = {
     EAST,   WEST,   NORTH,   SOUTH
};

CardinalDirection turnCW(CardinalDirection dir);
CardinalDirection turnCCW(CardinalDirection dir);
CardinalDirection toDirection(const cellar::Vec2i& vec);
cellar::Vec2i     toVec(CardinalDirection dir);

#endif // MDMTYPES_H
