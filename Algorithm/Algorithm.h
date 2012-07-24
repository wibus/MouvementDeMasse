#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <DataStructure/Vector.h>

#include "City/City.h"


class Algorithm
{
protected:
    Algorithm();

public:    
    virtual ~Algorithm();

    virtual void setup(City& city);

protected:
    City*         _city;
    Ground*       _ground;
    Description*  _description;
    cellar::Vec2i _mapSize;

    static const CardinalDirection _cardinalDirections[NB_DIRECTIONS];
};

#endif // ALGORITHM_H
