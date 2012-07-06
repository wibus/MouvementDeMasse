#ifndef HEIGHTSBYNOISEALGO_H
#define HEIGHTSBYNOISEALGO_H

#include "HeightsAlgorithm.h"

class HeightByNoiseAlgo : public HeightsAlgorithm
{
public:
    HeightByNoiseAlgo();

    virtual void setup(City& city);

    int minFrequency() const;
    int maxFrequency() const;
    void setWeightedFrequenciesRange(int min, int max);

private:
    int _minFreq;
    int _maxFreq;
};

#endif // HEIGHTSBYNOISEALGO_H
