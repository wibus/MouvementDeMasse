#ifndef MAPELEMENTSDEPTHFIRST_H
#define MAPELEMENTSDEPTHFIRST_H

#include <stack>

#include "MapElementsAlgorithm.h"

class MapElementsDepthFirst: public MapElementsAlgorithm
{
public:
    MapElementsDepthFirst();
    virtual ~MapElementsDepthFirst();

    virtual void setup(City& city);

protected:
    std::stack<glm::ivec2> _junctionsStack;
};

#endif // MAPELEMENTSDEPTHFIRST_H
