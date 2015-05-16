#include "CitizensEqualAlgo.h"

#include <GLM/gtc/random.hpp>

#include <PropRoom2D/Prop/Shape/Segment2D.h>

using namespace std;
using namespace cellar;
using namespace prop2;


CitizensEqualAlgo::CitizensEqualAlgo()
{
}

void CitizensEqualAlgo::setup(City &city)
{
    CitizensAlgorithm::setup(city);

    for(int j=0; j<_mapSize.y; ++j)
    {
        for(int i=0; i<_mapSize.x; ++i)
        {
            if(_city->junctions().get(i, j)->type() == Junction::ASPHALT)
            {
                Citizen ctz;
                ctz.position = glm::vec3(i, j, _ground->heightAt(i, j));
                ctz.walkingSpeed = (0.8f + glm::linearRand(-0.5f, 0.5f)) * _city->description().unitPerMeter;

                _city->citizens().insert(make_pair(ctz.id(), ctz));
            }
        }
    }
}

void CitizensEqualAlgo::update()
{
    const float roadQuarterWidth = _city->description().roadWidth *0.25f;

    int nbCitizens = static_cast<int>(_city->citizens().size());
    for(int c=0; c < nbCitizens; ++c)
    {
        Citizen& ctz = _city->citizens()[c];

        if(ctz.curState == CITIZEN_AT_HOME)
        {
            glm::ivec2 pos(floor(ctz.position.x + 0.5f),  floor(ctz.position.y + 0.5f));
            glm::ivec2 dir(floor(ctz.direction.x + 0.5f), floor(ctz.direction.y + 0.5f));
            dir = Segment2D::perpCCW(dir);

            Junction& junc = *_city->junctions().get( pos );

            while(! junc.getStreet(toDirection( dir )))
                dir = Segment2D::perpCW(dir);

            glm::ivec2 right = dir;
            right = Segment2D::perpCW(right);
            glm::vec2 rightSide = glm::vec2(right) * roadQuarterWidth;

            ctz.curState = CITIZEN_GOTO_WORK;
            ctz.direction = glm::vec3(dir.x, dir.y, 0.0f);
            ctz.homeToWorkPath.destination = pos + dir;
            ctz.position = glm::vec3(
                pos.x + rightSide.x,
                pos.y + rightSide.y,
                _ground->heightAt(pos));
        }
        else
        {
            ctz.position += ctz.direction * ctz.walkingSpeed;
            ctz.position.z = _ground->heightAt(ctz.position.x, ctz.position.y);

            if(glm::length(glm::vec2(ctz.position) - (glm::vec2(ctz.homeToWorkPath.destination)))
                    < ctz.walkingSpeed + roadQuarterWidth)
                ctz.curState = CITIZEN_AT_HOME;
        }
    }
}
