#include "HeightsByNoiseAlgo.h"

#include <Misc/CellarUtils.h>

#include "CityMap.h"
#include "Road/Junction.h"

using namespace cellar;

HeightByNoiseAlgo::HeightByNoiseAlgo()
{
}

void HeightByNoiseAlgo::setup(CityMap &cityMap)
{
    HeightsAlgorithm::setup( cityMap );

    unsigned char prev=0, next=1;
    float maxHeight = 0.0f;
    float weight = noiseWeight(0);
    bool usedNoise = inBounds((unsigned int)0, _minNoise, _maxNoise);
    bool isLastPass = false;

    Grid<float> result(_mapSize.x(), _mapSize.y());
    Grid<float> noises[2] = {
        Grid<float>(_mapSize.x(), _mapSize.y()),
        Grid<float>(_mapSize.x(), _mapSize.y())
    };


    for(unsigned int j=0; j< _mapSize.y(); ++j)
    {
        for(unsigned int i=0; i< _mapSize.x(); ++i)
        {
            float height = random(-1.0, 1.0);
            noises[prev].set(i, j, height);
            if(usedNoise) result.set(i, j, height);
        }
    }

    for(unsigned int n=1; n<_nbNoises; ++n)
    {
        // This is truly an assignation
        if( (usedNoise = inBounds(n, _minNoise, _maxNoise)) )
            weight = noiseWeight(n);

        isLastPass = (n == _nbNoises-1) ? true : false;


        for(unsigned int j=0; j< _mapSize.y(); ++j)
        {
            for(unsigned int i=0; i< _mapSize.x(); ++i)
            {
                int nb = 0;
                float moy = 0;

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

                float h = moy / nb;
                noises[next].set(i, j, h);


                if(usedNoise)
                    result.get(i, j) += h * weight;

                if(isLastPass)
                    maxHeight = max(maxHeight, abs(result.get(i, j)));
            }
        }

        prev ^= 0x1;
        next ^= 0x1;
    }

    Junction* junction;
    for(unsigned int j=0; j< _mapSize.y(); ++j)
    {
        for(unsigned int i=0; i< _mapSize.x(); ++i)
        {
            // This is truly an assignation
            if( (junction = _cityMap->junctions().get(i, j)) )
                junction->setHeight( result.get(i, j)/maxHeight );
        }
    }
}

void HeightByNoiseAlgo::process()
{
}

float HeightByNoiseAlgo::noiseWeight(unsigned int idx)
{
    return sqrt(idx / static_cast<float>(_nbNoises));
}

unsigned int HeightByNoiseAlgo::nbNoises() const
{
    return _nbNoises;
}

unsigned int HeightByNoiseAlgo::minWeightedNoise() const
{
    return _minNoise;
}

unsigned int HeightByNoiseAlgo::maxWeightedNoise() const
{
    return _maxNoise;
}

void HeightByNoiseAlgo::setNbNoises(unsigned int nb)
{
    _nbNoises = nb;
}

void HeightByNoiseAlgo::setWeightedNoisesRange(unsigned int min, unsigned int max)
{
    _minNoise = min;
    _maxNoise = max;
}
