#ifndef HEIGHTSBYNOISEALGO_H
#define HEIGHTSBYNOISEALGO_H

#include "HeightsAlgorithm.h"

class HeightByNoiseAlgo : public HeightsAlgorithm
{
public:
    HeightByNoiseAlgo();

    virtual void setup(CityMap& cityMap);

    unsigned int nbNoises() const;
    unsigned int minWeightedNoise() const;
    unsigned int maxWeightedNoise() const;

    void setNbNoises(unsigned int nb);
    void setWeightedNoisesRange(unsigned int min, unsigned int max);

private:
    float noiseWeight(unsigned int idx);

    unsigned int _nbNoises;
    unsigned int _minNoise;
    unsigned int _maxNoise;
};

#endif // HEIGHTSBYNOISEALGO_H
