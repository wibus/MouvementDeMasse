#ifndef CITYMAP_H
#define CITYMAP_H

#include <memory>

#include <DataStructures/PGrid.h>
#include <MathsAndPhysics/Vector.h>

#include "Land/Land.h"
#include "Road/Junction.h"

class DrawAlgorithm;
class HeightsAlgorithm;
class MapElementsAlgorithm;


class CityMap
{
public:
    CityMap(uint width, uint height, const cellar::Vec2f& heightsRange);

    void setDrawAlgorithm(DrawAlgorithm* algorithm);
    void setHeightsAlgorithm(HeightsAlgorithm* algorithm);
    void setMapElementsAlgorthm(MapElementsAlgorithm* algorithm);

    DrawAlgorithm& drawAlgorithm();
    HeightsAlgorithm& heightsAlgorithm();
    MapElementsAlgorithm& mapElementsAlgorithm();

    cellar::Vec2ui size() const;
    cellar::Vec2f  heightsRange() const;
    cellar::PGrid<Land> &lands();
    cellar::PGrid<Junction>& junctions();

    void reset();
    void setup();
    bool load(const std::string& fileName);
    bool save(const std::string& fileName);

protected:
    void resetJunctions();

private:
    cellar::Vec2ui _size;
    cellar::Vec2f  _heightsRange;
    cellar::PGrid<Land> _lands;
    cellar::PGrid<Junction> _junctions;

    std::shared_ptr<DrawAlgorithm> _drawAlgo;
    std::shared_ptr<HeightsAlgorithm> _heightsAlgo;
    std::shared_ptr<MapElementsAlgorithm> _mapElementsAlgo;
};



// Implementation //
inline cellar::Vec2ui CityMap::size() const
{
    return _size;
}

inline cellar::Vec2f  CityMap::heightsRange() const
{
    return _heightsRange;
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
