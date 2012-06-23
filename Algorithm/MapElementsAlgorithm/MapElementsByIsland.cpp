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
            _islandIdentifiers[j][i] = -1;
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
            if (_islandIdentifiers[j][i] != -1)
                continue;

            // If the current junction is over water
            bool currIsLand = (isJunctionAboveWater(Vec2i(i, j)));

            if (prevIsLand != currIsLand)
            {
                Vec2i position(0, 0);
                Vec2i direction(0, 0);

                // The land is at the "right", while the non-land is at the "left"
                if (prevIsLand == true)
                {
                    position = Vec2i(i - 1, j);
                    direction = Vec2i(0, 1);
                }
                else  // prevIsLand == false
                {
                    position = Vec2i(i, j);
                    direction = Vec2i(0, -1);
                }
                mapOneIsland(position, direction);
            }
        }
    }
}

void MapElementsByIsland::mapOneIsland(Vec2i startPosition, Vec2i startDirection)
{
    // Small explanation about how it works.
    // The position that method receive is the position of the "land"
    // From the direction point of view, the "non-land" is at the left

    // The goal of this method is to make a full circle around an island
    // We keep the land at the right, and make turns accordingly so
    // we always have a land on the "right" and a non-land on the "left"

    // We keep a list of the positions
    std::vector<Vec2i> positions;
    positions.push_back(startPosition);

    // If we make a circle clockwise, it's "non-land" around "land".
    // Else, it's "land" around "non-land" and it's not an island.
    int diffClockwise = 0;

    Vec2i currPosition = startPosition;
    Vec2i currDirection = startDirection;

    do
    {
        // We are gonna look at three different cases.

        Vec2i newDirection = currDirection;
        newDirection.rotateQuarterCCW();
        Vec2i newPosition = currPosition + newDirection;
        // The block at the left is land. We turn left.
        if (isInBounds(newPosition) && isJunctionAboveWater(newPosition))
        {
            currDirection = newDirection;
            currPosition = newPosition;
            positions.push_back(currPosition);
            --diffClockwise;
            continue;
        }

        newDirection = currDirection;
        newPosition = currPosition + newDirection;
        // If not. The block in front is land. We continue.
        if (isInBounds(newPosition) && isJunctionAboveWater(newPosition))
        {
            currDirection = newDirection;
            currPosition = newPosition;
            positions.push_back(currPosition);
            continue;
        }

        // If not. The block in right is land. We turn right.
        newDirection = currDirection;
        newDirection.rotateQuarterCW();
        newPosition = currPosition + newDirection;
        // If not. The block in front is land. We continue.
        if (isInBounds(newPosition) && isJunctionAboveWater(newPosition))
        {
            currDirection = newDirection;
            currPosition = newPosition;
            positions.push_back(currPosition);
            ++diffClockwise;
            continue;
        }

        // If not. Peninsula. We turn around.
        currDirection = -currDirection;

        diffClockwise += 2;
        // We when have to go back, we have to turn either two times CW or CCW.
        // Since we always keep the "land" to our right, we have to turn CW and not CCW.
        // Else we wouldn't have the "land" to our right.
    }
    while (!(startPosition == currPosition /*&& startDirection == currDirection*/));

    // We have an Island and not a Lake
    if (diffClockwise > 0)
    {
        // We have an Island so we can identify an Island with the positions
        while (!positions.empty())
        {
            Vec2i position = positions.back();
            _islandIdentifiers[position.y()][position.x()] = _nbIslands;

            // LET'S TEST SOME ISLAND!!!!!
            Vec2i endA = positions.back();

            // END OF THE TEST

            positions.pop_back();


            // TEST MOAR
            if (!positions.empty())
            {
                Vec2i endB = positions.back();

                std::shared_ptr<Street> newStreet(new Street(endA, endB));

                Junction* juncA = _cityMap->junctions().get(endA);
                juncA->setType(Junction::ASPHALT);
                Junction* juncB = _cityMap->junctions().get(endB);
                juncA->attach(newStreet, toCardinal(endB - endA));
                juncB->attach(newStreet, toCardinal(endA - endB));
                // END OF TEST MOAR
            }

            ++_nbIslands;
        }
    }
    else
    {
        while (!positions.empty())
        {
            Vec2i position = positions.back();
            _islandIdentifiers[position.y()][position.x()] = -2;

            positions.pop_back();
        }
    }
}

bool MapElementsByIsland::isInBounds(Vec2i position)
{
    return !(position.x() >= _mapSize.x() ||
            position.y() >= _mapSize.y() ||
            position.x() < 0 ||
            position.y() < 0);
}

bool MapElementsByIsland::isJunctionAboveWater(Vec2i position)
{
    return (_cityMap->ground().heightAt(position) > 0);
}

bool MapElementsByIsland::isLandAboveWater(Vec2i position)
{
    int x = position.x();
    int y = position.y();
    return (_cityMap->ground().heightAt(x,     y) > 0 &&
            _cityMap->ground().heightAt(x + 1, y) > 0 &&
            _cityMap->ground().heightAt(x,     y + 1) > 0 &&
            _cityMap->ground().heightAt(x + 1, y + 1) > 0);
}
