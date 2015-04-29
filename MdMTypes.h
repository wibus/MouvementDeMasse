#ifndef MDMTYPES_H
#define MDMTYPES_H

#include <string>

#include <GLM/glm.hpp>


enum CardinalDirection{EAST, WEST, NORTH, SOUTH, NB_DIRECTIONS, DIRECTION_ERROR};

const std::string CARDINAL_DIRECTION_STRINGS[NB_DIRECTIONS] = {
    "EAST", "WEST", "NORTH", "SOUTH"
};
const CardinalDirection INT_TO_CARDINAL_DIRECTION[NB_DIRECTIONS] = {
     EAST,   WEST,   NORTH,   SOUTH
};

CardinalDirection turnCW(CardinalDirection dir);
CardinalDirection turnCCW(CardinalDirection dir);
CardinalDirection toDirection(const glm::ivec2& vec);
glm::ivec2        toVec(CardinalDirection dir);

#endif // MDMTYPES_H
