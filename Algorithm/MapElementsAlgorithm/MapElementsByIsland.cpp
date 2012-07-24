#include "MapElementsByIsland.h"

#include <memory>
#include <set>
using namespace std;

#include <Misc/CellarUtils.h>
using namespace cellar;


MapElementsByIsland::MapElementsByIsland()
{
}

MapElementsByIsland::~MapElementsByIsland()
{

}

void MapElementsByIsland::setup(City &city)
{
    _junctionsStack = stack<cellar::Vec2i>();
    _islandIdentifiers = Grid<int>();
    _islandEdges = std::vector<std::vector<cellar::Vec2i> >();

    MapElementsAlgorithm::setup(city);

    _nbIslands = 0;
    _islandIdentifiers = Grid<int>(_mapSize.x(),
                                   _mapSize.y(),
                                   -1);

    findAndExploreIslands();
    bridgeIslands();
    landIslands();
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
    // The general idea here is to take an unlinked island
    // and link it with an island that is already linked
    // thus making an island mob
    // When all the islands are linked to the mob,
    // the "graph" connectivity is true

    // All islands start unbridged
    std::set<int> toBridgeIslands;
    for (int i = 0; i < _nbIslands; i++)
    {
        toBridgeIslands.insert(i);
    }

    // We will randomly pick an island.
    // That island will be the first one of the mob
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

    // We bridge other islands
    while (nbOfBridgedIslands != _nbIslands)
    {
        // We randomly choose what islands will be bridges
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

        // There is one less island to bridge.
        // and one more bridged
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
    bestJuncSecond = 0;

    for (uint currPos = 0; currPos < _islandEdges[secondIsland].size(); ++currPos)
    {
        currDistance = absolute(_islandEdges[firstIsland][bestJuncFirst].x() -
                                _islandEdges[secondIsland][currPos].x())
                            +
                       absolute(_islandEdges[firstIsland][bestJuncFirst].y() -
                                _islandEdges[secondIsland][currPos].y());
        if (currDistance < bestDistance)
        {
            bestDistance = currDistance;
            bestJuncSecond = currPos;
        }
    }

    bestDistance = _mapSize.x() + _mapSize.y() + 1;

    for (uint currPos = 0; currPos < _islandEdges[firstIsland].size(); ++currPos)
    {
        currDistance = absolute(_islandEdges[firstIsland][currPos].x() -
                                _islandEdges[secondIsland][bestJuncSecond].x())
                            +
                       absolute(_islandEdges[firstIsland][currPos].y() -
                                _islandEdges[secondIsland][bestJuncSecond].y());
        if (currDistance < bestDistance)
        {
            bestDistance = currDistance;
            bestJuncFirst = currPos;
        }
    }

    Vec2i endA(_islandEdges[firstIsland][bestJuncFirst]);
    Vec2i endB(_islandEdges[secondIsland][bestJuncSecond]);

    Vec2i atob = endB - endA;

    int t = maxVal(absolute(atob.x()), absolute(atob.y()));

    Vec2i currentPos;

    Vec2i newBridgeEndA = endA;
    Vec2i newBridgeEndB;

    bool wasUnderWater = false;

    for (int i = 0; i <= t; ++i)
    {
        currentPos = endA + (i * atob) / t;

        if (isJunctionAboveWater(currentPos))
        {
            if (wasUnderWater)
            {
                newBridgeEndB = currentPos;
                _city->bridges().push_back(Bridge(newBridgeEndA, newBridgeEndB));
            }
            newBridgeEndA = currentPos;
            wasUnderWater = false;
        }
        else
            wasUnderWater = true;
    }
}

void MapElementsByIsland::landIslands()
{
    for (int i = 0; i < _nbIslands; ++i)
    {
        // Begining of algorithm
        Vec2i currentPoint = _islandEdges[i][0];

        while (_ground->heightAt( currentPoint ) < _ground->waterHeight())
        {
            currentPoint += Vec2i(1, 1);
            if (currentPoint.x() >= _mapSize.x())
            {
                currentPoint(randomRange(0, _mapSize.x()), randomRange(0, _mapSize.y()));
            }
        }

        _junctionsStack.push(currentPoint);


        // Main loop
        while (!_junctionsStack.empty())
        {
            Vec2i currPos = _junctionsStack.top();
            Junction* currJunc = _city->junctions().get(currPos);
            currJunc->setType(Junction::ASPHALT);

            // Check wich sides are free (not under water and not visited)
            // And wich are reachable (not under water)
            std::vector<Vec2i> reachableSides;
            std::vector<Vec2i> freeSides;

            Vec2i direction(1, 0);

            for (int i = 0; i < 4; i++)
            {
                direction.rotateQuarterCCW();
                Vec2i neighPos = currPos + direction;

                // In bounds
                if (neighPos.x() >= _mapSize.x() ||
                    neighPos.y() >= _mapSize.y() ||
                    neighPos.x() < 0 ||
                    neighPos.y() < 0)
                    continue;

                // Not under water
                if(_ground->heightAt( neighPos ) < _ground->waterHeight())
                    continue;

                reachableSides.push_back(direction);

                // Wasn't visited
                if (_city->junctions().get(neighPos)->type() != Junction::GRASS)
                    continue;

                freeSides.push_back(direction);
            }

            // Decide with which junction to link by road
            if (!freeSides.empty())
            {
                // Choose randomly a free side
                int nbElements = freeSides.size();
                int pos = randomRange(0, nbElements);
                Vec2i nextDirection = freeSides[pos];
                Vec2i nextPos = currPos + nextDirection;

                std::shared_ptr<Street> newStreet(new Street(currPos, nextPos));

                currJunc->attach(newStreet, toDirection(nextDirection));
                _city->junctions().get(nextDirection+currPos)->attach(newStreet, toDirection(-nextDirection));

                // Add the next junction
                _junctionsStack.push( nextPos );
            }
            else
            {
                if(!reachableSides.empty())
                if(randomRange(0.0, 4.0) > 1.0)
                {
                    // Choose randomly an reachable side
                    int nbElements = reachableSides.size();
                    int pos = randomRange(0, nbElements);
                    Vec2i nextDirection = reachableSides[pos];
                    Vec2i nextPos = currPos + nextDirection;

                    std::shared_ptr<Street> newStreet(new Street(currPos, nextPos));

                    currJunc->attach(newStreet, toDirection(nextDirection));
                    _city->junctions().get(nextDirection+currPos)->attach(newStreet, toDirection(-nextDirection));
                }

                // Pop the current junction
                _junctionsStack.pop();
            }
        }
    }


    // Add some lands.
    PGrid<Land>* lands = &_city->lands();
    for (int j = 0; j < lands->height(); ++j)
    {
        for (int i = 0; i < lands->width(); ++i)
        {
            if (_city->ground().heightAt(i, j) > 0)
            {
                int landType = randomRange(0, (int) Land::NB_TYPES);

                lands->get(i, j)->setType((Land::Type) landType);
                lands->get(i, j)->setNbStories(2+randomRange(0, Land::maxNbStories()-2));
            }
        }
    }

}

float MapElementsByIsland::slope (const cellar::Vec2i& endA, const cellar::Vec2i& endB)
{
    float heightDiff = _city->ground().heightAt(endA) -
                       _city->ground().heightAt(endB);

    float distance = cellar::Vec2f(endA - endB).length();

    return heightDiff / distance;
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
