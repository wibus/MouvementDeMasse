#include "HeightsByNoiseAlgo.h"

#include <iostream>
using namespace std;

#include <Misc/CellarUtils.h>

#include "City/CityMap.h"
#include "Road/Junction.h"

using namespace cellar;

HeightByNoiseAlgo::HeightByNoiseAlgo()
{
}

void HeightByNoiseAlgo::setup(CityMap &cityMap)
{
    HeightsAlgorithm::setup( cityMap );

    float middleHeight = (_maxHeight + _minHeight) / 2.0f;
    float amplitude = (_maxHeight - _minHeight) / 2.0f;

    unsigned char prev=0, next=1;
    float maxHeight = 0.0f;
    float weight = noiseWeight(0);
    bool usedNoise = inBounds(0, _minNoise, _maxNoise);
    bool isLastPass = false;

    Grid<float> result(_mapSize.x(), _mapSize.y(), 0);
    Grid<float> noises[2] = {
        Grid<float>(_mapSize.x(), _mapSize.y()),
        Grid<float>(_mapSize.x(), _mapSize.y())
    };


    for(int j=0; j< _mapSize.y(); ++j)
    {
        for(int i=0; i< _mapSize.x(); ++i)
        {
            float height = random(-1.0, 1.0);
            noises[prev].set(i, j, height);
            if(usedNoise) result.set(i, j, height);
        }
    }

    for(int n=1; n<_nbNoises; ++n)
    {
        // This is truly an assignation
        if( (usedNoise = inBounds(n, _minNoise, _maxNoise)) )
            weight = noiseWeight(n);

        isLastPass = (n == _nbNoises-1);


        for(int j=0; j< _mapSize.y(); ++j)
        {
            for(int i=0; i< _mapSize.x(); ++i)
            {
                const int nb = 8;
                float moy = 0;

                Vec2i sides[nb] = {
                    Vec2i(i-1, j-1),
                    Vec2i(i,   j-1),
                    Vec2i(i+1, j-1),
                    Vec2i(i+1, j),
                    Vec2i(i+1, j+1),
                    Vec2i(i,   j+1),
                    Vec2i(i-1, j+1),
                    Vec2i(i-1, j),
                };

                for(int k=0; k<nb; ++k)
                {
                    sides[k][Vec2i::AXIS_X] %= _mapSize.x();
                    if(sides[k].x() < 0)
                        sides[k][Vec2i::AXIS_X] += _mapSize.x();

                    sides[k][Vec2i::AXIS_Y] %= _mapSize.y();
                    if(sides[k].y() < 0)
                        sides[k][Vec2i::AXIS_Y] += _mapSize.y();

                    moy += noises[prev].get( sides[k] );
                }


                /*
                if(i > 0)
                {
                    ++nb;
                    moy += noises[prev].get(i-1, j);

                    if(j > 0)
                    {
                        ++nb;
                        moy += noises[prev].get(i-1, j-1);
                    }
                    if(j < _mapSize.y() - 1)
                    {
                        ++nb;
                        moy += noises[prev].get(i-1, j+1);
                    }
                }
                if(i < _mapSize.x() - 1)
                {
                    ++nb;
                    moy += noises[prev].get(i+1, j);

                    if(j > 0)
                    {
                        ++nb;
                        moy += noises[prev].get(i+1, j-1);
                    }

                    if(j < _mapSize.y() - 1)
                    {
                        ++nb;
                        moy += noises[prev].get(i+1, j+1);
                    }
                }

                if(j > 0)
                {
                    ++nb;
                    moy += noises[prev].get(i, j-1);
                }

                if(j < _mapSize.y() - 1)
                {
                    ++nb;
                    moy += noises[prev].get(i, j+1);
                }
                */

                float h = moy / nb;
                noises[next].set(i, j, h);


                if(usedNoise)
                    result.get(i, j) += h * weight;

                if(isLastPass)
                    maxHeight = cellar::max(maxHeight, abs(result.get(i, j)));
            }
        }

        prev ^= 0x1;
        next ^= 0x1;
    }

    float amplitudeCorection = amplitude / maxHeight;

    for(int j=0; j< _mapSize.y(); ++j)
        for(int i=0; i< _mapSize.x(); ++i)
            _ground->setHeightAt(i, j,
                result.get(i, j) * amplitudeCorection + middleHeight
            );
}

float HeightByNoiseAlgo::noiseWeight(int idx)
{
    return sqrt(idx / static_cast<float>(_nbNoises));
}

int HeightByNoiseAlgo::nbNoises() const
{
    return _nbNoises;
}

int HeightByNoiseAlgo::minWeightedNoise() const
{
    return _minNoise;
}

int HeightByNoiseAlgo::maxWeightedNoise() const
{
    return _maxNoise;
}

void HeightByNoiseAlgo::setNbNoises(int nb)
{
    _nbNoises = nb;
}

void HeightByNoiseAlgo::setWeightedNoisesRange(int min, int max)
{
    _minNoise = min;
    _maxNoise = max;
}
