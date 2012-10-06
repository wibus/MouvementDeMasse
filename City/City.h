#ifndef CITY_H
#define CITY_H

#include <memory>
#include <vector>
#include <map>

#include <DateAndTime/Calendar.h>
#include <DataStructure/PGrid2D.h>
#include <DataStructure/Vector.h>

#include "Citizen.h"
#include "Sky.h"
#include "Sun.h"
#include "Ground.h"
#include "Land.h"
#include "Junction.h"
#include "Street.h"
#include "Bridge.h"
#include "Description.h"

typedef std::vector<Bridge>        BridgeContainer;
typedef BridgeContainer::iterator  BridgeIterator;

typedef std::map<int, Citizen>     CitizenContainer;
typedef CitizenContainer::iterator CitizenIterator;


class City
{
public:
    City(int width, int height);

    cellar::Vec2i  size() const;
    cellar::DateAndTime& dateAndTime();
    cellar::DateAndTime& timeJump();
    Sky& sky();
    Sun& sun();
    Ground& ground();    
    cellar::PGrid2D<Land>& lands();
    cellar::PGrid2D<Junction>& junctions();
    BridgeContainer&  bridges();
    CitizenContainer& citizens();    
    Description& description();

    void reset();
    void setup();
    void update();
    bool load(const std::string& fileName);
    bool save(const std::string& fileName);

protected:
    void resetJunctions();
    void resetLands();

    bool saveDescription(const std::string& fileName);
    bool saveHeightMap(const std::string& fileName);
    bool saveSkyMap(const std::string& fileName);

private:
    cellar::Vec2i _size;
    cellar::DateAndTime _dateAndTime;
    cellar::DateAndTime _timeJump;
    Sky _sky;
    Sun _sun;
    Ground _ground;
    cellar::PGrid2D<Land> _lands;
    cellar::PGrid2D<Junction> _junctions;
    BridgeContainer  _bridges;
    CitizenContainer _citizens;    
    Description _description;
};



// Implementation //
inline cellar::Vec2i City::size() const
{
    return _size;
}

inline cellar::DateAndTime& City::dateAndTime()
{
    return _dateAndTime;
}

inline cellar::DateAndTime& City::timeJump()
{
    return _timeJump;
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

inline cellar::PGrid2D<Land>& City::lands()
{
    return _lands;
}

inline cellar::PGrid2D<Junction>& City::junctions()
{
    return _junctions;
}

inline BridgeContainer &City::bridges()
{
    return _bridges;
}

inline CitizenContainer& City::citizens()
{
    return _citizens;
}

inline Description& City::description()
{
    return _description;
}

#endif // CITY_H
