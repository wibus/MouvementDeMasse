#include "MapElementsByIsland.h"

#include <memory>
#include <set>

#include <GLM/gtc/random.hpp>

#include <PropRoom2D/Prop/Shape/Segment2D.h>

#include <Algorithm/Kruskal/KruskalAlgorithm.h>

using namespace std;
using namespace cellar;
using namespace prop2;


const int LAND_UNDER_WATER = -1;
const int UNKNOWN_ISLAND = -2;


MapElementsByIsland::MapElementsByIsland()
{
}

MapElementsByIsland::~MapElementsByIsland()
{

}

void MapElementsByIsland::setup(City &city)
{
    _junctionsStack = stack<glm::ivec2>();
    _islandIdentifiers = Grid2D<int>();
    _islandEdges = std::vector<std::vector<glm::ivec2> >();

    MapElementsAlgorithm::setup(city);

    _nbIslands = 0;
    _islandIdentifiers = Grid2D<int>(_mapSize.x,
                                   _mapSize.y,
                                   -1);

    findAndExploreIslands();
    setLandsToIslands();
    bridgeIslands();
    landIslands();
}

void MapElementsByIsland::findAndExploreIslands()
{
    bool prevIsLand = false;
    for (int j = 0; j < _mapSize.y; ++j)
    {
        prevIsLand = false;
        for (int i = 0; i < _mapSize.x; i++)
        {
            // If the current junction is over water
            bool currIsLand = (isJunctionAboveWater(glm::ivec2(i, j)));

            if (prevIsLand != currIsLand)
            {
                glm::ivec2 position(0, 0);
                glm::ivec2 direction(0, 0);

                // The land is at the "right", while the non-land is at the "left"
                if (prevIsLand == true)
                {
                    position = glm::ivec2(i - 1, j);
                    direction = glm::ivec2(0, -1);
                }
                else  // prevIsLand == false
                {
                    position = glm::ivec2(i, j);
                    direction = glm::ivec2(0, 1);
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
                    direction = Segment2D::perpCCW(direction);
                    if (!(isJunctionInBounds(position - direction) &&
                        isJunctionAboveWater(position - direction)))
                    {
                        direction = Segment2D::perpCCW(direction);
                    }
                }
                exploreOneIsland(position, direction);
            }
            prevIsLand = currIsLand;
        }
    }
}

void MapElementsByIsland::exploreOneIsland(glm::ivec2 startPosition, glm::ivec2 startDirection)
{

    // Small explanation about how it works.
    // The position that method receive is the position of the "land"
    // From the direction point of view, the "non-land" is at the left

    // The goal of this method is to make a full circle around an island
    // We keep the land at the right, and make turns accordingly so
    // we always have a land on the "right" and a non-land on the "left"

    // We keep a list of the positions
    std::vector<glm::ivec2> positions;
    positions.push_back(startPosition);

    // If we make a circle clockwise, it's "non-land" around "land".
    // Else, it's "land" around "non-land" and it's not an island.
    int diffClockwise = 0;

    glm::ivec2 currPosition = startPosition;
    glm::ivec2 currDirection = startDirection;

    do
    {
        // We are gonna look at three different cases.

        glm::ivec2 newDirection = currDirection;
        newDirection = Segment2D::perpCCW(newDirection);
        glm::ivec2 newPosition = currPosition + newDirection;
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
        newDirection = Segment2D::perpCW(newDirection);
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
            glm::ivec2 position = positions.back();
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
            glm::ivec2 position = positions.back();
            _islandIdentifiers.set(position, UNKNOWN_ISLAND);

            positions.pop_back();
        }
    }
}

void MapElementsByIsland::setLandsToIslands()
{
    int currIslandIdentifier = UNKNOWN_ISLAND;
    for (int j = 0; j < _mapSize.y - 1; j++)
    {
        for (int i = 0; i < _mapSize.x - 1; i++)
        {
            if (!isJunctionAboveWater(glm::ivec2(i, j)))
            {
                currIslandIdentifier = LAND_UNDER_WATER;
            }
            else
            {
                if (_islandIdentifiers.get(i, j) != - 1)
                {
                    currIslandIdentifier = _islandIdentifiers.get(i, j);
                }
                else
                {
                    _islandIdentifiers.set(glm::ivec2(i, j), currIslandIdentifier);
                }
                if (isLandAboveWater(glm::ivec2(i, j)))
                {
                    _city->lands().get(glm::ivec2(i, j))->setIslandIdentifier(currIslandIdentifier);
                }
            }
        }
    }
}

void MapElementsByIsland::roadOneIsland(int)
{

}

void MapElementsByIsland::bridgeIslands()
{
    _possibleBridges = cellar::Grid2D<glm::ivec2>(_nbIslands, _nbIslands);
    KruskalAlgorithm kruskal;
    // The general idea here is: Kruskal
    for (int i = 0; i < _nbIslands; i++)
    {
        for (int j = 0; j < i; j++)
        {
            addAPossibleBridge(i, j);
            double distance =
                glm::length(glm::vec2(
                    _possibleBridges[i][j] -
                    _possibleBridges[j][i]));
            kruskal.addSegment(i, j, distance);
        }
    }

    Graph cheapestGraph = kruskal.getCheapestGraph();

    for (auto itr = cheapestGraph.begin(); itr != cheapestGraph.end(); itr++)
    {
        bridgeTwoIslands(itr->edgeA(), itr->edgeB());
    }
}

void MapElementsByIsland::addAPossibleBridge(int firstIsland, int secondIsland)
{
    int bestJuncFirst;
    int bestJuncSecond;
    double bestDistance = _mapSize.x + _mapSize.y;
    double currDistance;

    int size = static_cast<int>(_islandEdges[firstIsland].size());
    bestJuncFirst = glm::linearRand(0, glm::max(size - 2, 0));
    bestJuncSecond = 0;

    int nbIslandEdges2 = static_cast<int>(_islandEdges[secondIsland].size());
    for (int currPos = 0; currPos < nbIslandEdges2; ++currPos)
    {
        currDistance =
                glm::length(glm::vec2(
                    _islandEdges[firstIsland][bestJuncFirst] -
                    _islandEdges[secondIsland][currPos]));

        if (currDistance < bestDistance)
        {
            bestDistance = currDistance;
            bestJuncSecond = currPos;
        }
    }

    bestDistance = _mapSize.x + _mapSize.y + 1;

    int nbIslandEdges1 = static_cast<int>(_islandEdges[firstIsland].size());
    for (int currPos = 0; currPos < nbIslandEdges1; ++currPos)
    {
        currDistance =
            glm::length(glm::vec2(
                _islandEdges[firstIsland][currPos] -
                _islandEdges[secondIsland][bestJuncSecond]));

        if (currDistance < bestDistance)
        {
            bestDistance = currDistance;
            bestJuncFirst = currPos;
        }
    }

    glm::ivec2 endA(_islandEdges[firstIsland][bestJuncFirst]);
    glm::ivec2 endB(_islandEdges[secondIsland][bestJuncSecond]);

    _possibleBridges[firstIsland][secondIsland] = endA;
    _possibleBridges[secondIsland][firstIsland] = endB;


//     Legacy code I like.
//     It takes two points on the map, and make bridges over water
//     But no bridge over land.
//    glm::ivec2 atob = endB - endA;

//    int t = glm::max(glm::abs(atob.x), glm::abs(atob.y));

//    glm::ivec2 currentPos;

//    glm::ivec2 newBridgeEndA = endA;
//    glm::ivec2 newBridgeEndB;

//    bool wasUnderWater = false;

//    for (int i = 0; i <= t; ++i)
//    {
//        currentPos = endA + (i * atob) / t;

//        if (isJunctionAboveWater(currentPos))
//        {
//            if (wasUnderWater)
//            {
//                newBridgeEndB = currentPos;
//                _possibleBridges[firstIsland][secondIsland] = newBridgeEndA;
//                _possibleBridges[secondIsland][firstIsland] = newBridgeEndB;
//            }
//            newBridgeEndA = currentPos;
//            wasUnderWater = false;
//        }
//        else
//            wasUnderWater = true;
//    }
}

void MapElementsByIsland::bridgeTwoIslands(int firstIsland, int secondIsland)
{
    _city->bridges().push_back(Bridge(_possibleBridges[firstIsland][secondIsland],
                                      _possibleBridges[secondIsland][firstIsland]));
}

void MapElementsByIsland::landIslands()
{
    for (int i = 0; i < _nbIslands; ++i)
    {
        // Begining of algorithm
        glm::ivec2 currentPoint = _islandEdges[i][0];

        while (_ground->heightAt( currentPoint ) < _ground->waterHeight())
        {
            currentPoint += glm::ivec2(1, 1);
            if (currentPoint.x >= _mapSize.x)
            {
                currentPoint = glm::ivec2(
                    glm::linearRand(0, _mapSize.x-1),
                    glm::linearRand(0, _mapSize.y-1));
            }
        }

        _junctionsStack.push(currentPoint);


        // Main loop
        while (!_junctionsStack.empty())
        {
            glm::ivec2 currPos = _junctionsStack.top();
            Junction* currJunc = _city->junctions().get(currPos);
            currJunc->setType(Junction::ASPHALT);

            // Check wich sides are free (not under water and not visited)
            // And wich are reachable (not under water)
            std::vector<glm::ivec2> reachableSides;
            std::vector<glm::ivec2> freeSides;

            glm::ivec2 direction(1, 0);

            for (int i = 0; i < 4; i++)
            {
                direction = Segment2D::perpCCW(direction);
                glm::ivec2 neighPos = currPos + direction;

                // In bounds
                if (neighPos.x >= _mapSize.x ||
                    neighPos.y >= _mapSize.y ||
                    neighPos.x < 0 ||
                    neighPos.y < 0)
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
                int nbElements = static_cast<int>(freeSides.size());
                int pos = glm::linearRand(0, nbElements-1);
                glm::ivec2 nextDirection = freeSides[pos];
                glm::ivec2 nextPos = currPos + nextDirection;

                std::shared_ptr<Street> newStreet(new Street(currPos, nextPos));

                currJunc->attach(newStreet, toDirection(nextDirection));
                _city->junctions().get(nextDirection+currPos)->attach(newStreet, toDirection(-nextDirection));

                // Add the next junction
                _junctionsStack.push( nextPos );
            }
            else
            {
                if(!reachableSides.empty())
                if(glm::linearRand(0.0, 4.0) > 1.0)
                {
                    // Choose randomly an reachable side
                    int nbElements = static_cast<int>(reachableSides.size());
                    int pos = glm::linearRand(0, nbElements-1);
                    glm::ivec2 nextDirection = reachableSides[pos];
                    glm::ivec2 nextPos = currPos + nextDirection;

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
    PGrid2D<Land>* lands = &_city->lands();
    for (int j = 0; j < lands->getHeight(); ++j)
    {
        for (int i = 0; i < lands->getWidth(); ++i)
        {
            if (_city->lands().get(glm::ivec2(i, j))->getIslandIdentifier() != -1)
            {
                int landType = glm::linearRand(0, (int) Land::NB_TYPES-1);

                lands->get(i, j)->setType((Land::Type) landType);
                lands->get(i, j)->setNbStories(2+glm::linearRand(0, Land::maxNbStories()-3));
            }
        }
    }
}

float MapElementsByIsland::landHeightDiff(const glm::ivec2& landPos)
{
    return  max(    max(_city->ground().heightAt( landPos ), _city->ground().heightAt( landPos + glm::ivec2(0,1) ) ),
                    max(_city->ground().heightAt( landPos + glm::ivec2(1,0) ), _city->ground().heightAt( landPos + glm::ivec2(1,1) ) ) );
}

float MapElementsByIsland::slope (const glm::ivec2& endA, const glm::ivec2& endB)
{
    float heightDiff = _city->ground().heightAt(endA) -
                       _city->ground().heightAt(endB);

    float distance = glm::length(glm::vec2(endA - endB));

    return heightDiff / distance;
}

bool MapElementsByIsland::isJunctionInBounds(glm::ivec2 position)
{
    return !(position.x >= _mapSize.x ||
             position.y >= _mapSize.y ||
             position.x < 0 ||
             position.y < 0);
}

bool MapElementsByIsland::isJunctionAboveWater(glm::ivec2 position)
{
    return (_city->ground().heightAt(position) > 0);
}

bool MapElementsByIsland::isLandInBounds(glm::ivec2 position)
{
    return !(position.x >= _mapSize.x - 1 ||
             position.y >= _mapSize.y - 1 ||
             position.x < 0 ||
             position.y < 0);
}

bool MapElementsByIsland::isLandAboveWater(glm::ivec2 position)
{
    int x = position.x;
    int y = position.y;
    return (_city->ground().heightAt(x,     y) > 0 &&
            _city->ground().heightAt(x + 1, y) > 0 &&
            _city->ground().heightAt(x,     y + 1) > 0 &&
            _city->ground().heightAt(x + 1, y + 1) > 0);
}
