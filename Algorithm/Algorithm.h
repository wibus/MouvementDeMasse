#ifndef ALGORITHM_H
#define ALGORITHM_H

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
    glm::ivec2    _mapSize;

    static const CardinalDirection _cardinalDirections[NB_DIRECTIONS];
};

#endif // ALGORITHM_H
