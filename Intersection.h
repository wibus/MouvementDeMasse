#ifndef INTERSECTION_H
#define INTERSECTION_H

#include <MathsAndPhysics/Vector3D.h>


enum CardinalDirection {EAST, WEST, NORTH, SOUTH, NB_DIRECTIONS};

class Intersection
{
public:
    Intersection();

    void setNeightbor(CardinalDirection direction, Intersection* neightbor);
    Intersection* getNeightbor(CardinalDirection direction) const;

private:
    cellar::Vector3D<float> _position;
    Intersection* _neightbors[NB_DIRECTIONS];
};

#endif // INTERSECTION_H
