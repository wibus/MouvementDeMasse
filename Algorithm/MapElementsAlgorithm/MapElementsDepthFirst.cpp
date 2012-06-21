#include "MapElementsDepthFirst.h"
#include <vector>
#include <memory>
#include <Misc/CellarUtils.h>
#include "Road/Junction.h"
#include "Road/Street.h"
#include "City/CityMap.h"

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
    MapElementsAlgorithm::setup(cityMap);

    // Begining of algorithm
    Vec2i currentPoint(_mapSize / 2);

    while (_ground->heightAt( currentPoint ) < _ground->waterHeight())
    {
        currentPoint += Vec2i(1, 1);
        if (currentPoint.x() >= _mapSize.x())
        {
            currentPoint(random(0, _mapSize.x()), random(0, _mapSize.y()));
        }
    }

    _junctionsStack.push(currentPoint);


    // Main loop
    while (!_junctionsStack.empty())
    {
        Vec2i currPos = _junctionsStack.top();
        Junction* currJunc = _cityMap->junctions().get(currPos);
        currJunc->setType(Junction::ASPHALT);

        // Check wich sides are free (not under water and not visited)
        std::vector<Vec2i> freeSides;

        Vec2i direction(1, 0);

        for (int i = 0; i < 4; i++)
        {
            direction.rotateQuarterCCW();
            Vec2i neighPos = currPos + direction;
            if (neighPos.x() >= _mapSize.x() ||
                neighPos.y() >= _mapSize.y() ||
                neighPos.x() < 0 ||
                neighPos.y() < 0 ||
                _ground->heightAt( neighPos ) < _ground->waterHeight() )
                continue;

            if (_cityMap->junctions().get(neighPos)->type() != Junction::GRASS)
                continue;

            freeSides.push_back(direction);
        }


        // if none
        //      link with a road or not
        //      get back to last free junction in the stack
        if (freeSides.empty())
        {/*
            if (random(2.0) > 1)
            {
                for (int i = 0; i < 4; i++)
                {

                    direction.rotateQuarterCCW();
                    Vec2i neighPos = currPos + direction;
                    if (neighPos.x() >= _mapSize.x() ||
                        neighPos.y() >= _mapSize.y() ||
                        neighPos.x() < 0 ||
                        neighPos.y() < 0 ||
                        _ground->heightAt( neighPos ) < _ground->waterHeight() )
                        continue;


                    ///////////////////////////////////////////////////////
                    //if (_cityMap->junctions().get(currPos)->);
                    //    continue;

                    //freeSides.push_back(neighPos);
                    //////////////////////////////////////////////////////


                    std::shared_ptr<Street> newStreet(new Street(currPos, neighPos));

                    currJunc->attach(newStreet, toDirection(direction));
                    _cityMap->junctions().get(neighPos)->attach(newStreet, toDirection(-direction));
                }
            }*/
            _junctionsStack.pop();
        }
        else
        {
            // Choose randomly a free side
            int nbElements = freeSides.size();
            int pos = cellar::random(0, nbElements);
            Vec2i nextDirection = freeSides[pos];
            Vec2i nextPos = currPos + nextDirection;

            std::shared_ptr<Street> newStreet(new Street(currPos, nextPos));

            currJunc->attach(newStreet, toDirection(nextDirection));
            _cityMap->junctions().get(nextDirection+currPos)->attach(newStreet, toDirection(-nextDirection));

            _junctionsStack.push( nextPos );
        }
    }
}
