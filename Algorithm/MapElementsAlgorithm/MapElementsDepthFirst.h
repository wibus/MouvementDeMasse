#ifndef MAPELEMENTSDEPTHFIRST_H
#define MAPELEMENTSDEPTHFIRST_H

#include <DataStructure/Vector.h>
#include <stack>

#include "MapElementsAlgorithm.h"

class MapElementsDepthFirst: public MapElementsAlgorithm
{
public:
    MapElementsDepthFirst();
    virtual ~MapElementsDepthFirst();

    virtual void setup(City& city);

protected:
    std::stack<cellar::Vec2i> _junctionsStack;
};

#endif // MAPELEMENTSDEPTHFIRST_H
