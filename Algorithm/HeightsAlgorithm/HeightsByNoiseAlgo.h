#ifndef HEIGHTSBYNOISEALGO_H
#define HEIGHTSBYNOISEALGO_H

#include "HeightsAlgorithm.h"

class HeightByNoiseAlgo : public HeightsAlgorithm
{
public:
    HeightByNoiseAlgo();

    virtual void setup(City& city);
};

#endif // HEIGHTSBYNOISEALGO_H
