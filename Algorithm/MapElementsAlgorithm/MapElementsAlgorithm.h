#ifndef MAPELEMENTSALGORITHM_H
#define MAPELEMENTSALGORITHM_H

#include "MdMTypes.h"
#include "Algorithm/Algorithm.h"


class MapElementsAlgorithm : public Algorithm
{
protected:
    MapElementsAlgorithm();

public:
    virtual ~MapElementsAlgorithm();

    virtual void setup(CityMap& cityMap);

protected:
    CardinalDirection toCardinal(const cellar::Vec2i& dir);
    cellar::Vec2ui    getNeighbor(const cellar::Vec2ui& pos, const cellar::Vec2i& dir);
};

#endif // MAPELEMENTSALGORITHM_H
