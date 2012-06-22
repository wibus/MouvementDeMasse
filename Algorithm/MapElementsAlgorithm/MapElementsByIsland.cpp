#include "MapElementsByIsland.h"

#include <vector>
#include <memory>
#include <Misc/CellarUtils.h>
#include "Road/Junction.h"
#include "Road/Street.h"
#include "City/CityMap.h"

using namespace cellar;

MapElementsByIsland::MapElementsByIsland()
{
}

MapElementsByIsland::~MapElementsByIsland()
{

}

void MapElementsByIsland::setup(CityMap &cityMap)
{
    MapElementsAlgorithm::setup(cityMap);

    _nbIslands = 0;
    _islandIdentifiers = new int*[_mapSize.y()];

    for (int j = 0; j < _mapSize.y(); ++j)
    {
        _islandIdentifiers[j] = new int[_mapSize.x()];

        for (int i = 0; i < _mapSize.x(); ++i)
        {
            _islandIdentifiers[i][j] = -1;
        }
    }

    findAndMapIslands();

    for (int j = 0; j < _mapSize.y(); ++j)
    {
        delete [] _islandIdentifiers[j];
    }
    delete [] _islandIdentifiers;
}

void MapElementsByIsland::findAndMapIslands()
{
    bool prevIsLand = false;
    for (int j = 0; j < _mapSize.y(); ++j)
    {
        prevIsLand = false;
        for (int i = 0; i < _mapSize.x(); i++)
        {
            // If the junction has already been explored
            if (_islandIdentifiers[i][j] != -1)
                continue;

            // If the current junction is over water
            bool currIsLand = (isAboveWater(Vec2i(i, j)));

            if (prevIsLand != currIsLand)
            {
                Vec2i position(0, 0);
                Vec2i direction(0, 0);

                // The land is at the "right", while the non-land is at the "left"
                if (prevIsLand == true)
                {
                    position = Vec2i(i - 1, j);
                    direction = Vec2i(-1, 0);
                }
                else  // prevIsLand == false
                {
                    position = Vec2i(i, j);
                    direction = Vec2i(1, 0);
                }
                mapOneIsland(position, direction);
            }
        }
    }
}

void MapElementsByIsland::mapOneIsland(Vec2i postion, Vec2i direction)
{
    // Small explanation about how it works.
    // The position that method receive is the position of the "land"
    // From the direction point of view, the "non-land" is at the left

    // The goal of this method is to make a full circle around an island
    // We keep the land at the right, and make turns accordingly so
    // we always have a land on the "right" and a non-land on the "left"

    // We are gonna look at three different cases.

    // The block at the front-left is land. We turn left.

    // If not. The block in front is land. We continue.

    // If not. The block in front-right is land. We turn right.

    // If not. Peninsula. We turn around.
}

bool MapElementsByIsland::isInBounds(Vec2i position)
{
    return (position.x() >= _mapSize.x() ||
            position.y() >= _mapSize.y() ||
            position.x() < 0 ||
            position.y() < 0);
}

bool MapElementsByIsland::isAboveWater(Vec2i position)
{
    return (_cityMap->ground().heightAt(position) > 0);
}
