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

    CardinalDirection toCardinal(const glm::ivec2& dir);
    glm::ivec2    getNeighbor(const glm::ivec2& pos, const glm::ivec2& dir);
};

#endif // MAPELEMENTSALGORITHM_H
