#ifndef MAPELEMENTSBYISLAND_H
#define MAPELEMENTSBYISLAND_H

#include <DataStructure/Grid.h>
#include <DataStructure/Vector.h>
#include <stack>
#include <vector>

#include "MapElementsAlgorithm.h"

class MapElementsByIsland: public MapElementsAlgorithm
{
protected:
    struct distance{
        cellar::Vec2i endA;
        cellar::Vec2i endB;
        int distance;
    };

public:
    MapElementsByIsland();
    virtual ~MapElementsByIsland();

    virtual void setup(City& city);
protected:
    std::stack<cellar::Vec2i> _junctionsStack;
    int _nbIslands;
    cellar::Grid<int> _islandIdentifiers;
    std::vector<std::vector<cellar::Vec2i> > _islandEdges;

private:
    void findAndExploreIslands();
    void exploreOneIsland(cellar::Vec2i postion, cellar::Vec2i direction);
    void roadOneIsland(int index);
    void bridgeIslands();
    void bridgeTwoIslands(int firstIsland, int secondIsland);
    void landIslands();
    float slope (const cellar::Vec2i& endA, const cellar::Vec2i& endB);
    bool isJunctionInBounds(cellar::Vec2i position);
    bool isJunctionAboveWater(cellar::Vec2i position);
    bool isLandInBounds(cellar::Vec2i position);
    bool isLandAboveWater(cellar::Vec2i position);

};

#endif // MAPELEMENTSBYISLAND_H
