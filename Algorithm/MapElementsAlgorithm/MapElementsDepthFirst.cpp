#include "MapElementsDepthFirst.h"
#include <vector>
#include <memory>
#include <Misc/CellarUtils.h>
#include "Road/Junction.h"
#include "Road/Street.h"
#include "CityMap.h"

using namespace cellar;

MapElementsDepthFirst::MapElementsDepthFirst():
    _junctionsStack()
{
}

MapElementsDepthFirst::~MapElementsDepthFirst()
{

}

void MapElementsDepthFirst::setup(CityMap& cityMap)
{
    // TODO : Complete the algorithm

    MapElementsAlgorithm::setup(cityMap);

    const float waterHeight = 0.0f;

    Vec2ui currentPoint(_mapSize / 2);

    while (_cityMap->junctions().get(currentPoint)->height() < 0)
    {
        currentPoint += Vec2ui(1, 1);
        if (currentPoint.x() > _mapSize.x())
        {
            currentPoint(random(0, (int)_mapSize.x()), random(0, (int)_mapSize.y()));
        }
    }
    cityMap.junctions().get(currentPoint)->setType(Junction::ASPHALT);
    _junctionsStack.push(currentPoint);

    while (!_junctionsStack.empty())
    {
        Vec2ui currPos = _junctionsStack.top();
        Junction* currJunc = _cityMap->junctions().get(currPos);
        currJunc->setType(Junction::ASPHALT);

        // Check wich sides are free (not under water and not visited)
        std::vector<Vec2i> freeSides;

        Vec2i direction(1, 0);

        for (unsigned int i = 0; i < 4; i++)
        {
            direction.rotateQuarterCCW();
            Vec2ui neighPos = getNeighbor(currPos, direction);
            if (neighPos.x() >= (_mapSize.x() - 2) || neighPos.y() >= (_mapSize.y() - 2) ||
                _cityMap->junctions().get(neighPos)->height() < waterHeight )
                continue;

            if (_cityMap->junctions().get(neighPos)->type() != Junction::GROUND)
                continue;

            freeSides.push_back(direction);
        }


        // if none
        //      link with a road or not
        //      get back to last free junction in the stack
        if (freeSides.empty())
        {
            if (random(2.0) > 1)
            {
                for (unsigned int i = 0; i < 4; i++)
                {

                    direction.rotateQuarterCCW();
                    Vec2ui neighPos = getNeighbor(currPos, direction);
                    if (neighPos.x() >= (_mapSize.x() - 2) || neighPos.y() >= (_mapSize.y() - 2) ||
                        _cityMap->junctions().get(neighPos)->height() < waterHeight )
                        continue;


                    ///////////////////////////////////////////////////////
                    //if (_cityMap->junctions().get(currPos)->);
                    //    continue;

                    //freeSides.push_back(neighPos);
                    //////////////////////////////////////////////////////


                    std::shared_ptr<Street> newStreet(new Street(currPos, neighPos));
                    currJunc->attach(newStreet, toCardinal(direction));

                }
            }
            _junctionsStack.pop();
        }
        else
        {
            // Choose randomly a free side
            int nbElements = freeSides.size();
            int pos = cellar::random(0, nbElements);
            Vec2i nextDirection = freeSides[pos];

            std::shared_ptr<Street> newStreet(new Street(currPos, getNeighbor(currPos, nextDirection)));
            currJunc->attach(newStreet, toCardinal(direction));

            _junctionsStack.push(getNeighbor(currPos, nextDirection));
        }

        // Add the curent junction in the stack
    }
}
