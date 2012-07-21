#include "CitizensEqualAlgo.h"

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

                _city->citizens().push_back(ctz);
            }
        }
    }
}

void CitizensEqualAlgo::update()
{
    const float roadQuarterWidth = _city->description().roadWidth *0.25f;

    for(size_t c=0; c<_city->citizens().size(); ++c)
    {
        Citizen& ctz = _city->citizens()[c];

        if(ctz.state == Citizen::AT_HOME)
        {
            Vec2i pos(round(ctz.position.x()),  round(ctz.position.y()));
            Vec2i dir(round(ctz.direction.x()), round(ctz.direction.y()));
            dir.rotateQuarterCCW();

            Junction& junc = *_city->junctions().get( pos );

            while(! junc.getStreet(toDirection( dir )))
                dir.rotateQuarterCW();

            Vec2i right = dir;
            right.rotateQuarterCW();
            Vec2f rightSide = Vec2f(right) * roadQuarterWidth;

            ctz.state = Citizen::MOVING;
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

            if(vec2(ctz.position).distanceTo(Vec2f(ctz.homeToWorkPath.destination)) < ctz.walkingSpeed + roadQuarterWidth)
                ctz.state = Citizen::AT_HOME;
        }
    }
}
