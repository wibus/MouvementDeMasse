#ifndef CITY_H
#define CITY_H

#include <memory>
#include <vector>
#include <map>

#include <CellarWorkbench/DateAndTime/Calendar.h>
#include <CellarWorkbench/DataStructure/PGrid2D.h>

#include "Citizen.h"
#include "Sky.h"
#include "Sun.h"
#include "Ground.h"
#include "Land.h"
#include "Junction.h"
#include "Street.h"
#include "Bridge.h"
#include "Description.h"

class QString;

typedef std::vector<Bridge>        BridgeContainer;
typedef BridgeContainer::iterator  BridgeIterator;

typedef std::map<int, Citizen>     CitizenContainer;
typedef CitizenContainer::iterator CitizenIterator;


class City
{
public:
    City(int width, int height);

    glm::ivec2  size() const;
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

    bool saveHeightMap(const std::string& fileName);
    bool saveSkyMap(const std::string& fileName);

    void constructFileNames(
            const std::string& fileName,
            QString& filePath,
            QString& xmlFileName,
            QString& descriptFileName ,
            QString& heightMapFileName,
            QString& skyMapFileName);

private:
    glm::ivec2 _size;
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
inline glm::ivec2 City::size() const
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
