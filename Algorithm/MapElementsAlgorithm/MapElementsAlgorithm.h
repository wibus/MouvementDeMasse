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

    virtual void setup(City& city);

protected:
    Land::Type _typesOfLand[Land::NB_TYPES];

    CardinalDirection toCardinal(const cellar::Vec2i& dir);
    cellar::Vec2i    getNeighbor(const cellar::Vec2i& pos, const cellar::Vec2i& dir);
};

#endif // MAPELEMENTSALGORITHM_H
