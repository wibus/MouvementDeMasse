#include "HeightsByNoiseAlgo.h"

#include <iostream>
using namespace std;

#include <Misc/CellarUtils.h>
#include <MathsAndPhysics/Noise.h>
using namespace cellar;

HeightByNoiseAlgo::HeightByNoiseAlgo()
{
}

void HeightByNoiseAlgo::setup(City &city)
{
    HeightsAlgorithm::setup( city );

    float middleHeight = (_ground->maxHeight() + _ground->minHeight()) / 2.0f;
    float amplitude    = (_ground->maxHeight() - _ground->minHeight()) / 2.0f;
    SimplexNoise noisegen;
    float nsx = randomRange(-10.0f, 10.0f);
    float nsy = randomRange(-10.0f, 10.0f);

    for(int j=0; j< _mapSize.y(); ++j)
        for(int i=0; i< _mapSize.x(); ++i)
        {
            float xc = i/(float)_mapSize.x();
            float yc = j/(float)_mapSize.y();

            _ground->setHeightAt(i, j,
                noisegen.noiseTile2d(xc + nsx, yc + nsy, 0.5f) * amplitude + middleHeight
            );
        }
}
