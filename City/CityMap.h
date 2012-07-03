#ifndef CITYMAP_H
#define CITYMAP_H

#include <memory>

#include <DateAndTime/Calendar.h>
#include <DataStructures/PGrid.h>
#include <MathsAndPhysics/Vector.h>

#include "Weather/Sun.h"
#include "Weather/Sky.h"
#include "Ground.h"
#include "Land/Land.h"
#include "Road/Junction.h"
#include "Road/Street.h"

class DrawAlgorithm;
class HeightsAlgorithm;
class MapElementsAlgorithm;


class CityMap
{
public:
    CityMap(int width, int height);

    cellar::Vec2i  size() const;
    Sun& sun();
    Sky& sky();
    Ground& ground();
    cellar::PGrid<Land> &lands();
    cellar::PGrid<Junction>& junctions();
    cellar::Calendar& calendar();

    void reset();
    void setup();
    void update();
    bool load(const std::string& fileName);
    bool save(const std::string& fileName);

protected:
    void resetJunctions();
    void resetLands();

private:
    cellar::Vec2i _size;
    Sun _sun;
    Sky _sky;
    Ground _ground;
    cellar::PGrid<Land> _lands;
    cellar::PGrid<Junction> _junctions;
    cellar::Calendar _calendar;
};



// Implementation //
inline cellar::Vec2i CityMap::size() const
{
    return _size;
}

inline Sun& CityMap::sun()
{
    return _sun;
}

inline Sky& CityMap::sky()
{
    return _sky;
}

inline Ground& CityMap::ground()
{
    return _ground;
}

inline cellar::PGrid<Land>& CityMap::lands()
{
    return _lands;
}

inline cellar::PGrid<Junction>& CityMap::junctions()
{
    return _junctions;
}

inline cellar::Calendar& CityMap::calendar()
{
    return _calendar;
}

#endif // CITYMAP_H
