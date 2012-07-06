#include "HeightsByNoiseAlgo.h"

#include <iostream>
using namespace std;

#include <Misc/CellarUtils.h>
#include <MathsAndPhysics/Algorithms.h>

#include "City/City.h"
#include "Road/Junction.h"

using namespace cellar;

HeightByNoiseAlgo::HeightByNoiseAlgo()
{
}

void HeightByNoiseAlgo::setup(City &city)
{
    HeightsAlgorithm::setup( city );

    float middleHeight = (_ground->maxHeight() + _ground->minHeight()) / 2.0f;
    float amplitude    = (_ground->maxHeight() - _ground->minHeight()) / 2.0f;

    Grid<float> perlin(_mapSize.x(), _mapSize.y());
    perlinNoise(EqualWeighter(_minFreq, _maxFreq), perlin);

    for(int j=0; j< _mapSize.y(); ++j)
        for(int i=0; i< _mapSize.x(); ++i)
            _ground->setHeightAt(i, j,
                perlin.get(i, j) * amplitude + middleHeight
            );
}

int HeightByNoiseAlgo::minFrequency() const
{
    return _minFreq;
}

int HeightByNoiseAlgo::maxFrequency() const
{
    return _maxFreq;
}

void HeightByNoiseAlgo::setWeightedFrequenciesRange(int min, int max)
{
    _minFreq = min;
    _maxFreq = max;
}
