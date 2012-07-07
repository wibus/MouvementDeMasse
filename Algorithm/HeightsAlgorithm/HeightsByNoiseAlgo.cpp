#include "HeightsByNoiseAlgo.h"

#include <iostream>
using namespace std;

#include <Misc/CellarUtils.h>
#include <MathsAndPhysics/Algorithms.h>
using namespace cellar;

HeightByNoiseAlgo::HeightByNoiseAlgo()
{
}

void HeightByNoiseAlgo::setup(City &city)
{
    HeightsAlgorithm::setup( city );

    float middleHeight = (_ground->maxHeight() + _ground->minHeight()) / 2.0f;
    float amplitude    = (_ground->maxHeight() - _ground->minHeight()) / 2.0f;
    int maxFreq = minVal(_mapSize.x(), _mapSize.y()) / 4;

    Grid<float> perlin(_mapSize.x(), _mapSize.y());
    perlinNoise(EqualWeighter(1, maxFreq), perlin);

    for(int j=0; j< _mapSize.y(); ++j)
        for(int i=0; i< _mapSize.x(); ++i)
            _ground->setHeightAt(i, j,
                perlin.get(i, j) * amplitude + middleHeight
            );
}
