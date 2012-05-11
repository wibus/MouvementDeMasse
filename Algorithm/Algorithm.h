#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <MathsAndPhysics/Vector.h>
class CityMap;


class Algorithm
{
protected:
    Algorithm();

public:    
    virtual ~Algorithm();

    virtual void setup(CityMap& cityMap);
    virtual void process() = 0;

protected:
    CityMap* _cityMap;
    cellar::Vec2ui _mapSize;
};

#endif // ALGORITHM_H
