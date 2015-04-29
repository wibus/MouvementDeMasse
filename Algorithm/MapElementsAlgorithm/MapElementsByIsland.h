#ifndef MAPELEMENTSBYISLAND_H
#define MAPELEMENTSBYISLAND_H

#include <CellarWorkbench/DataStructure/Grid2D.h>
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
    std::stack<glm::ivec2> _junctionsStack;
    int _nbIslands;
    cellar::Grid2D<int> _islandIdentifiers;
    cellar::Grid2D<glm::ivec2> _possibleBridges;
    std::vector<std::vector<glm::ivec2> > _islandEdges;

private:
    void findAndExploreIslands();
    void exploreOneIsland(glm::ivec2 postion, glm::ivec2 direction);
    void setLandsToIslands();
    void roadOneIsland(int index);
    void bridgeIslands();
    void addAPossibleBridge(int firstIsland, int secondIsland);
    void bridgeTwoIslands(int firstIsland, int secondIsland);
    void landIslands();
    float landHeightDiff(const glm::ivec2& landPos);
    float slope (const glm::ivec2& endA, const glm::ivec2& endB);
    bool isJunctionInBounds(glm::ivec2 position);
    bool isJunctionAboveWater(glm::ivec2 position);
    bool isLandInBounds(glm::ivec2 position);
    bool isLandAboveWater(glm::ivec2 position);

};

#endif // MAPELEMENTSBYISLAND_H
