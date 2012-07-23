#ifndef MDMTYPES_H
#define MDMTYPES_H

#include <DataStructure/Vector.h>


enum CardinalDirection{EAST, WEST, NORTH, SOUTH, NB_DIRECTIONS, DIRECTION_ERROR};

CardinalDirection turnCW(CardinalDirection dir);
CardinalDirection turnCCW(CardinalDirection dir);
CardinalDirection toDirection(const cellar::Vec2i& vec);
cellar::Vec2i     toVec(CardinalDirection dir);

#endif // MDMTYPES_H
