#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <MathsAndPhysics/Vector.h>
class City;
class Ground;


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
    cellar::Vec2i _mapSize;
};

#endif // ALGORITHM_H
