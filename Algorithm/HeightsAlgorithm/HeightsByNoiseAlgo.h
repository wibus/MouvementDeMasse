#ifndef HEIGHTSBYNOISEALGO_H
#define HEIGHTSBYNOISEALGO_H

#include "HeightsAlgorithm.h"

class HeightByNoiseAlgo : public HeightsAlgorithm
{
public:
    HeightByNoiseAlgo();

    virtual void setup(CityMap& cityMap);
    virtual void process() = 0;
};

#endif // HEIGHTSBYNOISEALGO_H
