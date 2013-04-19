#include "CitizensEqualAlgo.h"

using namespace std;
using namespace cellar;


CitizensEqualAlgo::CitizensEqualAlgo()
{
}

void CitizensEqualAlgo::setup(City &city)
{
    CitizensAlgorithm::setup(city);

    for(int j=0; j<_mapSize.y(); ++j)
    {
        for(int i=0; i<_mapSize.x(); ++i)
        {
            if(_city->junctions().get(i, j)->type() == Junction::ASPHALT)
            {
                Citizen ctz;
                ctz.position(i, j, _ground->heightAt(i, j));
                ctz.walkingSpeed = (0.8f + randomRange(-0.5f, 0.5f)) * _city->description().unitPerMeter;

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
            Vec2i pos(floor(ctz.position.x() + 0.5f),  floor(ctz.position.y() + 0.5f));
            Vec2i dir(floor(ctz.direction.x() + 0.5f), floor(ctz.direction.y() + 0.5f));
            dir = perpCCW(dir);

            Junction& junc = *_city->junctions().get( pos );

            while(! junc.getStreet(toDirection( dir )))
                dir = perpCW(dir);

            Vec2i right = dir;
            right = perpCW(right);
            Vec2f rightSide = Vec2f(right) * roadQuarterWidth;

            ctz.curState = CITIZEN_GOTO_WORK;
            ctz.direction(dir.x(), dir.y(), 0.0f);
            ctz.homeToWorkPath.destination = pos + dir;
            ctz.position(pos.x() + rightSide.x(),
                         pos.y() + rightSide.y(),
                         _ground->heightAt(pos));
        }
        else
        {
            ctz.position += ctz.direction * ctz.walkingSpeed;
            ctz.position.setZ( _ground->heightAt(ctz.position.x(), ctz.position.y()));

            if(Vec2f(ctz.position).distanceTo(Vec2f(ctz.homeToWorkPath.destination)) < ctz.walkingSpeed + roadQuarterWidth)
                ctz.curState = CITIZEN_AT_HOME;
        }
    }
}
