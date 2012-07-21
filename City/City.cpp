#include "City.h"

#include <cmath>
using namespace std;
using namespace cellar;


City::City(int width, int height):
    _size(  width, height),
    _citizens(),
    _sky(128, 128),
    _sun(Vec4f(-1, -1, 2, 0), Vec3f(-1.0, -1.0, -0.5)),
    _ground(   width+1, height+1),    
    _lands(    width,   height),
    _junctions(width+1, height+1),
    _bridges(),
    _calendar(),
    _description()
{
}

void City::reset()
{
    resetJunctions();
    resetLands();

    _citizens.clear();
    _bridges.clear();
}

bool City::load(const string &)
{
    return false;
}

bool City::save(const string &)
{
    return false;
}

void City::update()
{
    _calendar.tic();
    _sun.setTime(_calendar.date().hour,
                 _calendar.date().minute);
    _sky.update();
}

void City::resetJunctions()
{
    for(int j=0; j<_junctions.height(); ++j)
    {
        for(int i=0; i<_junctions.width(); ++i)
        {
            Junction* junc = _junctions.get(i, j);
            if(junc != 0x0)
                delete junc;
            _junctions.set(i, j, new Junction());
        }
    }
}

void City::resetLands()
{
    for(int j=0; j<_lands.height(); ++j)
    {
        for(int i=0; i<_lands.width(); ++i)
        {
            Land* land = _lands.get(i, j);
            if(land != 0x0)
                delete land;
            _lands.set(i, j, new Land());
        }
    }
}
