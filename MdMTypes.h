#ifndef MMTYPES_H
#define MMTYPES_H


// Enums
enum Alignment {HORIZONTAL, VERTICAL, NB_ALIGNMENTS};
enum CardinalDirection {SOUTH, EAST, NORTH, WEST, NB_DIRECTIONS};
enum HSide {LEFT, RIGHT, NB_H_SIDES};
enum VSide {DOWN, UP,    NB_V_SIDES};

// Classes
class Intersection;
class HStreet;
class VStreet;
class Land;

#endif // MMTYPES_H
