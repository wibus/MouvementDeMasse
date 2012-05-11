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

    float amplitude = (_maxHeight - _minHeight) / 2.0f;
    int nbNoises = 100;
    Grid<float>** noises = new Grid<float>*[nbNoises];
    for(int i=0; i<nbNoises; ++i)
        noises[i] = new Grid<float>(_mapSize.x(),
                                   _mapSize.y());


    for(unsigned int j=0; j< _mapSize.y(); ++j)
        for(unsigned int i=0; i< _mapSize.x(); ++i)
            noises[0]->set(i, j, random(-amplitude, amplitude));

    for(int n=1; n<nbNoises; ++n)
    {
        for(unsigned int j=0; j< _mapSize.y(); ++j)
        {
            for(unsigned int i=0; i< _mapSize.x(); ++i)
            {
                int nb = 0;
                float moy = 0;

                if(i > 0)
                {
                    ++nb;
                    moy += noises[n-1]->get(i-1, j);
                }
                if(i < _mapSize.x() - 1)
                {
                    ++nb;
                    moy += noises[n-1]->get(i+1, j);
                }
                if(j > 0)
                {
                    ++nb;
                    moy += noises[n-1]->get(i, j-1);
                }
                if(j < _mapSize.y() - 1)
                {
                    ++nb;
                    moy += noises[n-1]->get(i, j+1);
                }

                if(i > 0 && j > 0)
                {
                    ++nb;
                    moy += noises[n-1]->get(i-1, j-1);
                }

                if(i < _mapSize.x() - 1 && j > 0)
                {
                    ++nb;
                    moy += noises[n-1]->get(i+1, j-1);
                }

                if(i < _mapSize.x() - 1 && j < _mapSize.y() - 1)
                {
                    ++nb;
                    moy += noises[n-1]->get(i+1, j+1);
                }

                if(i > 0 && j < _mapSize.y() - 1)
                {
                    ++nb;
                    moy += noises[n-1]->get(i-1, j+1);
                }

                noises[n]->set(i, j, moy / nb);
            }
        }
    }

    for(unsigned int j=0; j< _mapSize.y(); ++j)
    {
        for(unsigned int i=0; i< _mapSize.x(); ++i)
        {
            float height = 0;

            for(int n=6; n<nbNoises; ++n)
            {
                height += noises[n]->get(i, j) / sqrt(nbNoises - n);
            }

            _cityMap->junctions().get(i, j)->setHeight(height);
        }
    }
    // END
    for(int i=0; i<nbNoises; ++i)
        delete noises[i];
    delete [] noises;
}
