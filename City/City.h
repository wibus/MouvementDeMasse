#ifndef CITY_H
#define CITY_H

#include <memory>
#include <vector>

#include <DateAndTime/Calendar.h>
#include <DataStructures/PGrid.h>
#include <MathsAndPhysics/Vector.h>

#include "Citizen.h"
#include "Sky.h"
#include "Sun.h"
#include "Ground.h"
#include "Bridge.h"
#include "Land.h"
#include "Junction.h"
#include "Street.h"
#include "Visual.h"


class City
{
public:
    City(int width, int height);

    cellar::Vec2i  size() const;
    std::vector<Citizen>& citizens();
    Sky& sky();
    Sun& sun();
    Ground& ground();
    std::vector<Bridge>& bridges();
    cellar::PGrid<Land>& lands();
    cellar::PGrid<Junction>& junctions();    
    cellar::Calendar& calendar();
    Visual& visual();

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
    std::vector<Citizen> _citizens;
    Sky _sky;
    Sun _sun;
    Ground _ground;
    std::vector<Bridge> _bridges;
    cellar::PGrid<Land> _lands;
    cellar::PGrid<Junction> _junctions;    
    cellar::Calendar _calendar;
    Visual _visual;
};



// Implementation //
inline cellar::Vec2i City::size() const
{
    return _size;
}

inline std::vector<Citizen>& City::citizens()
{
    return _citizens;
}

inline Sky& City::sky()
{
    return _sky;
}

inline Sun& City::sun()
{
    return _sun;
}

inline Ground& City::ground()
{
    return _ground;
}

inline std::vector<Bridge>& City::bridges()
{
    return _bridges;
}

inline cellar::PGrid<Land>& City::lands()
{
    return _lands;
}

inline cellar::PGrid<Junction>& City::junctions()
{
    return _junctions;
}

inline cellar::Calendar& City::calendar()
{
    return _calendar;
}

inline Visual& City::visual()
{
    return _visual;
}

#endif // CITY_H
