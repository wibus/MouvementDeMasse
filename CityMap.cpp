#include "CityMap.h"
#include "Algorithm/DrawAlgorithm/DrawAlgorithm.h"
#include "Algorithm/HeightsAlgorithm/HeightsAlgorithm.h"
#include "Algorithm/MapElementsAlgorithm/MapElementsAlgorithm.h"

#include "Road/Junction.h"

#include <cmath>
using namespace std;
using namespace cellar;


CityMap::CityMap(uint width, uint height, const Vec2f &heightsRange):
    _size(width, height),
    _heightsRange(heightsRange),
    _lands(    width,   height),
    _junctions(width+1, height+1),
    _drawAlgo(),
    _heightsAlgo(),
    _mapElementsAlgo()
{
}

void CityMap::setDrawAlgorithm(DrawAlgorithm *algorithm)
{
    _drawAlgo.reset(algorithm);
}

void CityMap::setHeightsAlgorithm(HeightsAlgorithm *algorithm)
{
    _heightsAlgo.reset(algorithm);
}

void CityMap::setMapElementsAlgorthm(MapElementsAlgorithm *algorithm)
{
    _mapElementsAlgo.reset(algorithm);
}

DrawAlgorithm& CityMap::drawAlgorithm()
{
    return *_drawAlgo;
}

HeightsAlgorithm& CityMap::heightsAlgorithm()
{
    return *_heightsAlgo;
}

MapElementsAlgorithm& CityMap::mapElementsAlgorithm()
{
    return *_mapElementsAlgo;
}

float CityMap::heightAt(const cellar::Vec2f& pos) const
{
    // Check side of square for determining dx and dy
    // *-------*
    // | \   + |
    // |   \   |
    // | -   \ |
    // *-------*


    Vec2ui base(floor(pos.x()), floor(pos.y()));
    Vec2f bias(pos.x() - base.x(), pos.y() - base.y());

    if(bias == Vec2f(0, 0)) return _junctions.get(pos.x(), pos.y())->height();

    float dx, dy;
    const Vec2f n(0.707106781f, 0.707106781f); // sqrt(2)/2 = 0.707106781f
    Vec2f p(1.0f - bias.x(), bias.y());

    if(p * n < 0) // Which side of square
    {
        dx = _junctions.get(base + Vec2ui(1, 0))->height() - _junctions.get(base)->height();
        dy = _junctions.get(base + Vec2ui(0, 1))->height() - _junctions.get(base)->height();
    }
    else
    {
        base += Vec2ui(1, 1);
        bias -= Vec2f(1.0f, 1.0f);
        dx = _junctions.get(base)->height() - _junctions.get(base - Vec2ui(1, 0))->height();
        dy = _junctions.get(base)->height() - _junctions.get(base - Vec2ui(0, 1))->height();
    }

    return _junctions.get(base)->height() + bias.x()*dx + bias.y()*dy;
}

void CityMap::reset()
{
    resetJunctions();
}

void CityMap::setup()
{
    if( _heightsAlgo )
        _heightsAlgo->setup( *this );

    if( _mapElementsAlgo )
        _mapElementsAlgo->setup( *this );

    if( _drawAlgo )
        _drawAlgo->setup( *this );
}

bool CityMap::load(const string &)
{
    return false;
}

bool CityMap::save(const string &)
{
    return false;
}

void CityMap::resetJunctions()
{
    for(unsigned int j=0; j<_junctions.height(); ++j)
    {
        for(unsigned int i=0; i<_junctions.width(); ++i)
        {
            Junction* junc = _junctions.get(i, j);
            if(junc != 0x0)
                delete junc;
            _junctions.set(i, j, new Junction());
        }
    }
}
