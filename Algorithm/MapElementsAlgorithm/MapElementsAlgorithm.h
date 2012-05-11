#ifndef MAPELEMENTSALGORITHM_H
#define MAPELEMENTSALGORITHM_H

#include "Algorithm/Algorithm.h"


class MapElementsAlgorithm : public Algorithm
{
protected:
    MapElementsAlgorithm();

public:
    virtual ~MapElementsAlgorithm();

    virtual void setup(CityMap& cityMap);
    virtual void process() = 0;

protected:
};

#endif // MAPELEMENTSALGORITHM_H
