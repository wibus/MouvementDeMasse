#ifndef HEIGHTSBYNOISEALGO_H
#define HEIGHTSBYNOISEALGO_H

#include "HeightsAlgorithm.h"

class HeightByNoiseAlgo : public HeightsAlgorithm
{
public:
    HeightByNoiseAlgo();

    virtual void setup(CityMap& cityMap);

    int nbNoises() const;
    int minWeightedNoise() const;
    int maxWeightedNoise() const;

    void setNbNoises(int nb);
    void setWeightedNoisesRange(int min, int max);

private:
    float noiseWeight(int idx);

    int _nbNoises;
    int _minNoise;
    int _maxNoise;
};

#endif // HEIGHTSBYNOISEALGO_H
