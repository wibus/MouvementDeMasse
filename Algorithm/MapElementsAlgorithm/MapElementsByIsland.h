#ifndef MAPELEMENTSBYISLAND_H
#define MAPELEMENTSBYISLAND_H

#include <DataStructures/Grid.h>
#include <MathsAndPhysics/Vector.h>
#include <stack>

#include "MapElementsAlgorithm.h"

class MapElementsByIsland: public MapElementsAlgorithm
{
public:
    MapElementsByIsland();
    virtual ~MapElementsByIsland();

    virtual void setup(CityMap& cityMap);
protected:
    std::stack<cellar::Vec2i> _junctionsStack;
    int _nbIslands;
    cellar::Grid<int> _islandIdentifiers;

private:
    void findAndMapIslands();
    void mapOneIsland(cellar::Vec2i postion, cellar::Vec2i direction);
    bool isJunctionInBounds(cellar::Vec2i position);
    bool isJunctionAboveWater(cellar::Vec2i position);
    bool isLandAboveWater(cellar::Vec2i position);

};

#endif // MAPELEMENTSBYISLAND_H
