#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <MathsAndPhysics/Vector.h>
class CityMap;
class Ground;


class Algorithm
{
protected:
    Algorithm();

public:    
    virtual ~Algorithm();

    virtual void setup(CityMap& cityMap);

protected:
    CityMap* _cityMap;
    Ground*  _ground;
    cellar::Vec2i _mapSize;
    float _minHeight;
    float _maxHeight;
};

#endif // ALGORITHM_H
