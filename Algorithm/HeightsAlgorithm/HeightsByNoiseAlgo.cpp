#include "HeightsByNoiseAlgo.h"

#include <iostream>
using namespace std;

#include <CellarWorkbench/Misc/CellarUtils.h>
#include <CellarWorkbench/Misc/SimplexNoise.h>
using namespace cellar;

HeightByNoiseAlgo::HeightByNoiseAlgo()
{
}

void HeightByNoiseAlgo::setup(City &city)
{
    HeightsAlgorithm::setup( city );

    const int NB_FREQ = 4;
    const float FREQ[NB_FREQ] = {
        1.0f,
        2.0f,
        4.0f,
        8.0f
    };
    glm::vec2 ORIG[NB_FREQ];
    for(int i=0; i<NB_FREQ; ++i)
        ORIG[i] = glm::vec2(randomRange(-10.0f, 10.0f), randomRange(-10.0f, 10.0f));

    float middleHeight = (_ground->maxHeight() + _ground->minHeight()) / 2.0f;
    float amplitude    = (_ground->maxHeight() - _ground->minHeight()) / 2.0f;

    for(int j=0; j< _mapSize.y; ++j)
    {
        for(int i=0; i< _mapSize.x; ++i)
        {
            float xc = i/(float)_mapSize.x;
            float yc = j/(float)_mapSize.y;

            float height = 0.0f;
            for(int k=0; k<NB_FREQ; ++k)
                height += amplitude/(FREQ[k]*FREQ[k]) * SimplexNoise::noiseTile2d(
                    ORIG[k].x + xc*FREQ[k],
                    ORIG[k].x + yc*FREQ[k],
                    0.5f);

            _ground->setHeightAt(i, j, middleHeight + height);
        }
    }
}
