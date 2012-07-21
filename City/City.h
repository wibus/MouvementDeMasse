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
#include "Land.h"
#include "Junction.h"
#include "Street.h"
#include "Bridge.h"
#include "Description.h"


class City
{
public:
    City(int width, int height);

    cellar::Vec2i  size() const;
    std::vector<Citizen>& citizens();
    Sky& sky();
    Sun& sun();
    Ground& ground();    
    cellar::PGrid<Land>& lands();
    cellar::PGrid<Junction>& junctions();
    std::vector<Bridge>& bridges();
    cellar::Calendar& calendar();
    Description& description();

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
    cellar::PGrid<Land> _lands;
    cellar::PGrid<Junction> _junctions;
    std::vector<Bridge> _bridges;
    cellar::Calendar _calendar;
    Description _description;
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

inline cellar::PGrid<Land>& City::lands()
{
    return _lands;
}

inline cellar::PGrid<Junction>& City::junctions()
{
    return _junctions;
}

inline std::vector<Bridge>& City::bridges()
{
    return _bridges;
}

inline cellar::Calendar& City::calendar()
{
    return _calendar;
}

inline Description& City::description()
{
    return _description;
}

#endif // CITY_H
