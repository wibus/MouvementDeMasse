#include "MapElementsDepthFirst.h"

#include <vector>
#include <memory>

#include <CellarWorkbench/Misc/CellarUtils.h>
#include <CellarWorkbench/Geometry/Segment2D.h>
using namespace cellar;


MapElementsDepthFirst::MapElementsDepthFirst():
    _junctionsStack()
{
}

MapElementsDepthFirst::~MapElementsDepthFirst()
{

}

void MapElementsDepthFirst::setup(City& city)
{
    MapElementsAlgorithm::setup(city);

    // Begining of algorithm
    glm::ivec2 currentPoint(_mapSize / 2);

    while (_ground->heightAt( currentPoint ) < _ground->waterHeight())
    {
        currentPoint += glm::ivec2(1, 1);
        if (currentPoint.x >= _mapSize.x)
        {
            currentPoint = glm::ivec2(randomRange(0, _mapSize.x), randomRange(0, _mapSize.y));
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
            int pos = randomRange(0, nbElements);
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
            if(randomRange(0.0, 4.0) > 1.0)
            {
                // Choose randomly an reachable side
                int nbElements = static_cast<int>(reachableSides.size());
                int pos = randomRange(0, nbElements);
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


    // Add some lands.
    PGrid2D<Land>* lands = &_city->lands();
    for (int j = 0; j < lands->getHeight(); ++j)
    {
        for (int i = 0; i < lands->getWidth(); ++i)
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
