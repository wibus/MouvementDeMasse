#ifndef MAPELEMENTSBYISLAND_H
#define MAPELEMENTSBYISLAND_H

#include <DataStructure/Grid2D.h>
#include <DataStructure/Vector.h>
#include <stack>
#include <vector>

#include "MapElementsAlgorithm.h"

class MapElementsByIsland: public MapElementsAlgorithm
{
public:
    MapElementsByIsland();
    virtual ~MapElementsByIsland();

    virtual void setup(City& city);
protected:
    std::stack<cellar::Vec2i> _junctionsStack;
    int _nbIslands;
    cellar::Grid2D<int> _islandIdentifiers;
    cellar::Grid2D<cellar::Vec2i> _possibleBridges;
    std::vector<std::vector<cellar::Vec2i> > _islandEdges;

private:
    void findAndExploreIslands();
    void exploreOneIsland(cellar::Vec2i postion, cellar::Vec2i direction);
    void setLandsToIslands();
    void roadOneIsland(int index);
    void bridgeIslands();
    void addAPossibleBridge(int firstIsland, int secondIsland);
    void bridgeTwoIslands(int firstIsland, int secondIsland);
    void landIslands();
    float landHeightDiff(const cellar::Vec2i& landPos);
    float slope (const cellar::Vec2i& endA, const cellar::Vec2i& endB);
    bool isJunctionInBounds(cellar::Vec2i position);
    bool isJunctionAboveWater(cellar::Vec2i position);
    bool isLandInBounds(cellar::Vec2i position);
    bool isLandAboveWater(cellar::Vec2i position);

};

#endif // MAPELEMENTSBYISLAND_H
