#include "MapElementsByIsland.h"

#include <memory>
#include <set>
#include <Misc/CellarUtils.h>
#include "City/City.h"

using namespace cellar;

MapElementsByIsland::MapElementsByIsland()
{
}

MapElementsByIsland::~MapElementsByIsland()
{

}

void MapElementsByIsland::setup(City &city)
{
    MapElementsAlgorithm::setup(city);

    _nbIslands = 0;
    _islandIdentifiers = Grid<int>(_mapSize.x(),
                                   _mapSize.y(),
                                   -1);

    findAndExploreIslands();
    bridgeIslands();
}

void MapElementsByIsland::findAndExploreIslands()
{
    bool prevIsLand = false;
    for (int j = 0; j < _mapSize.y(); ++j)
    {
        prevIsLand = false;
        for (int i = 0; i < _mapSize.x(); i++)
        {
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
                    direction = Vec2i(0, -1);
                }
                else  // prevIsLand == false
                {
                    position = Vec2i(i, j);
                    direction = Vec2i(0, 1);
                }
                //If it's an island that had already been explored, we won't reexplore it
                if (_islandIdentifiers.get(position) != -1)
                {
                    prevIsLand = currIsLand;
                    continue;
                }
                // We have to find where we come from so know the direction we will get at the end.
                // We do this by looking if the junctions around are "lands" or "non-land"
                if (!(isJunctionInBounds(position - direction) &&
                    isJunctionAboveWater(position - direction)))
                {
                    direction.rotateQuarterCCW();
                    if (!(isJunctionInBounds(position - direction) &&
                        isJunctionAboveWater(position - direction)))
                    {
                        direction.rotateQuarterCCW();
                    }
                }
                exploreOneIsland(position, direction);
            }
            prevIsLand = currIsLand;
        }
    }
}

void MapElementsByIsland::exploreOneIsland(Vec2i startPosition, Vec2i startDirection)
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
        if (isJunctionInBounds(newPosition) && isJunctionAboveWater(newPosition))
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
        if (isJunctionInBounds(newPosition) && isJunctionAboveWater(newPosition))
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
        if (isJunctionInBounds(newPosition) && isJunctionAboveWater(newPosition))
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
        // We then have to go back, we have to turn either two times CW or CCW.
        // Since we always keep the "land" to our right, we have to turn CW and not CCW.
        // Else we wouldn't have the "land" to our right.
    }
    while (!(currPosition == startPosition && currDirection == startDirection));

    // We have an Island and not a Lake
    if (diffClockwise > 0)
    {
        _islandEdges.push_back(positions);
        // We have an Island so we can identify an Island with the positions
        while (!positions.empty())
        {
            Vec2i position = positions.back();
            _islandIdentifiers.set(position, _nbIslands);

            positions.pop_back();
        }
        ++_nbIslands;
    }
    else
    {
        // This is a Lake. We will set the Identifiers.
        // That way, we won't visit it again.
        while (!positions.empty())
        {
            Vec2i position = positions.back();
            _islandIdentifiers.set(position, -2);

            positions.pop_back();
        }
    }
}

void MapElementsByIsland::roadOneIsland(int index)
{

}

void MapElementsByIsland::bridgeIslands()
{
    std::set<int> toBridgeIslands;
    for (int i = 0; i < _nbIslands; i++)
    {
        toBridgeIslands.insert(i);
    }

    std::set<int> bridgedIslands;
    int nbOfBridgedIslands = 1;

    int size = toBridgeIslands.size();
    int nextIsland = randomRange(0, (size - 1));
    int linkedIsland = 0;

    std::set<int>::iterator toBridgeIter = toBridgeIslands.begin();
    std::set<int>::iterator bridgedIter = bridgedIslands.begin();

    for (int i = 0; i < nextIsland; i++)
    {
        toBridgeIter++;
    }

    bridgedIslands.insert(*toBridgeIter);
    toBridgeIslands.erase(*toBridgeIter);

    while (nbOfBridgedIslands != _nbIslands)
    {
        size = toBridgeIslands.size();
        nextIsland = randomRange(0, (size - 1));
        size = bridgedIslands.size();
        linkedIsland = randomRange(0, (size - 1));

        toBridgeIter = toBridgeIslands.begin();
        bridgedIter = bridgedIslands.begin();

        for (int i = 0; i < nextIsland; i++)
        {
            toBridgeIter++;
        }

        for (int i = 0; i < linkedIsland; i++)
        {
            bridgedIter++;
        }

        bridgeTwoIslands(*toBridgeIter, *bridgedIter);

        bridgedIslands.insert(*toBridgeIter);
        toBridgeIslands.erase(*toBridgeIter);

        ++nbOfBridgedIslands;
    }
}

void MapElementsByIsland::bridgeTwoIslands(int firstIsland, int secondIsland)
{
    int bestJuncFirst;
    int bestJuncSecond;
    int bestDistance = _mapSize.x() + _mapSize.y() + 1;
    int currDistance;

    int size = _islandEdges[firstIsland].size();
    bestJuncFirst = randomRange(0, size - 1);

    for (int currPos = 0; currPos < _islandEdges[secondIsland].size(); ++currPos)
    {
        currDistance = absolute(_islandEdges[firstIsland][bestJuncFirst].x() -
                                _islandEdges[secondIsland][currPos].x())
                            +
                       absolute(_islandEdges[firstIsland][bestJuncFirst].y() -
                                _islandEdges[secondIsland][currPos].y());
        if (currDistance < bestDistance)
            bestJuncSecond = currPos;
    }

    for (int currPos = 0; currPos < _islandEdges[firstIsland].size(); ++currPos)
    {
        currDistance = absolute(_islandEdges[firstIsland][currPos].x() -
                                _islandEdges[secondIsland][bestJuncSecond].x())
                            +
                       absolute(_islandEdges[firstIsland][currPos].y() -
                                _islandEdges[secondIsland][bestJuncSecond].y());
        if (currDistance < bestDistance)
            bestJuncFirst = currPos;
    }
}

void MapElementsByIsland::landIslands()
{

}

bool MapElementsByIsland::isJunctionInBounds(Vec2i position)
{
    return !(position.x() >= _mapSize.x() ||
             position.y() >= _mapSize.y() ||
             position.x() < 0 ||
             position.y() < 0);
}

bool MapElementsByIsland::isJunctionAboveWater(Vec2i position)
{
    return (_city->ground().heightAt(position) > 0);
}

bool MapElementsByIsland::isLandInBounds(Vec2i position)
{
    return !(position.x() >= _mapSize.x() - 1 ||
             position.y() >= _mapSize.y() - 1 ||
             position.x() < 0 ||
             position.y() < 0);
}

bool MapElementsByIsland::isLandAboveWater(Vec2i position)
{
    int x = position.x();
    int y = position.y();
    return (_city->ground().heightAt(x,     y) > 0 &&
            _city->ground().heightAt(x + 1, y) > 0 &&
            _city->ground().heightAt(x,     y + 1) > 0 &&
            _city->ground().heightAt(x + 1, y + 1) > 0);
}
