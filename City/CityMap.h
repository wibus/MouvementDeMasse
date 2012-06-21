#ifndef CITYMAP_H
#define CITYMAP_H

#include <memory>

#include <DataStructures/PGrid.h>
#include <MathsAndPhysics/Vector.h>

#include "Sun.h"
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
    Ground& ground();
    cellar::PGrid<Land> &lands();
    cellar::PGrid<Junction>& junctions();

    void reset();
    void setup();
    bool load(const std::string& fileName);
    bool save(const std::string& fileName);

protected:
    void resetJunctions();
    void resetLands();

private:
    cellar::Vec2i _size;
    Sun _sun;
    Ground _ground;
    cellar::PGrid<Land> _lands;
    cellar::PGrid<Junction> _junctions;
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

#endif // CITYMAP_H
