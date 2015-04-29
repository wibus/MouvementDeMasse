#include "Sky.h"

#include <CellarWorkbench/Misc/SimplexNoise.h>
#include <CellarWorkbench/Misc/CellarUtils.h>
using namespace cellar;


Sky::Sky(int width, int height) :
    _colorsGrid(width, height, 0.0f),
    _cloudsGrid(width, height, 0.0f),
    _cloudsImage(new unsigned char[width*height*4], width, height),
    _cloudyness(0.5f),
    _compactness(0.6f)
{
    float nsx = randomRange(-10.0f, 10.0f);
    float nsy = randomRange(-10.0f, 10.0f);

    for(int j=0; j<height; ++j)
    {
        for(int i=0; i<width; ++i)
        {
            float xc = i/(float)width;
            float yc = j/(float)height;

            _colorsGrid.set(i, j,
               (SimplexNoise::noiseTile2d(xc + nsx + 0.0f, yc + nsy + 0.0f, 0.6f) * 0.40f + 0.60f) * 256);

            _cloudsGrid.set(i, j,
                SimplexNoise::noiseTile2d(xc + nsx + 1.0f, yc + nsy + 1.0f, 1.0f) * 0.75f +
                SimplexNoise::noiseTile2d(xc + nsx + 2.0f, yc + nsy + 2.0f, 3.0f) * 0.25f);

            float norm = 1.0f / (_cloudyness);
            float depth = (_cloudsGrid.get(i, j) + 1.0f) * 0.5f;
            float density = pow(clamp(norm*(_cloudyness - depth), 0.0f, 1.0f),
                                1.0f-_compactness);

            _cloudsImage.setColor(i, j,
                                  _colorsGrid.get(i, j),
                                  _colorsGrid.get(i, j),
                                  _colorsGrid.get(i, j),
                                  density * 255);
        }
    }
}

void Sky::update()
{
}
