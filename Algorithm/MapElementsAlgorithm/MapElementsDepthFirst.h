#ifndef MAPELEMENTSDEPTHFIRST_H
#define MAPELEMENTSDEPTHFIRST_H

#include <MathsAndPhysics/Vector.h>
#include <stack>

#include "MapElementsAlgorithm.h"

class MapElementsDepthFirst: public MapElementsAlgorithm
{
public:
    MapElementsDepthFirst();
    virtual ~MapElementsDepthFirst();

    virtual void setup(CityMap& cityMap);

protected:
    std::stack<cellar::Vec2ui> _junctionsStack;
};

#endif // MAPELEMENTSDEPTHFIRST_H
