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
            if (_cityMap->junctions().get(neighPos)->type() != Junction::GRASS)
                continue;

            freeSides.push_back(direction);
        }

        // Decide with which junction to link by road
        if (!freeSides.empty())
        {
            // Choose randomly a free side
            int nbElements = freeSides.size();
            int pos = cellar::random(0, nbElements);
            Vec2i nextDirection = freeSides[pos];
            Vec2i nextPos = currPos + nextDirection;

            std::shared_ptr<Street> newStreet(new Street(currPos, nextPos));

            currJunc->attach(newStreet, toDirection(nextDirection));
            _cityMap->junctions().get(nextDirection+currPos)->attach(newStreet, toDirection(-nextDirection));

            // Add the next junction
            _junctionsStack.push( nextPos );
        }
        else
        {
            if(!reachableSides.empty())
            if(random(4.0) > 1.0)
            {
                // Choose randomly an reachable side
                int nbElements = reachableSides.size();
                int pos = cellar::random(0, nbElements);
                Vec2i nextDirection = reachableSides[pos];
                Vec2i nextPos = currPos + nextDirection;

                std::shared_ptr<Street> newStreet(new Street(currPos, nextPos));

                currJunc->attach(newStreet, toDirection(nextDirection));
                _cityMap->junctions().get(nextDirection+currPos)->attach(newStreet, toDirection(-nextDirection));
            }

            // Pop the current junction
            _junctionsStack.pop();
        }
    }


    // Add some lands.
    PGrid<Land>* lands = &_cityMap->lands();
    for (int j = 0; j < lands->height(); ++j)
    {
        for (int i = 0; i < lands->width(); ++i)
        {
            if (_cityMap->ground().heightAt(i, j) > 0)
            {
                int landType = random(0, (int) Land::NB_TYPES);

                lands->get(i, j)->setType((Land::Type) landType);
                lands->get(i, j)->setNbStories(2+cellar::random(6));
            }
        }
    }
}
