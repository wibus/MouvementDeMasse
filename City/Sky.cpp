#include "Sky.h"

#include <Algorithm/Noise.h>
using namespace cellar;


Sky::Sky(int width, int height) :
    _cloudsGrid(width, height, 0.0f),
    _cloudsImage(new unsigned char[width*height*4], width, height, Image::RGBA),
    _cloudyness(0.5f),
    _cloudCompactness(0.4f)
{
    float nsx = randomRange(-10.0f, 10.0f);
    float nsy = randomRange(-10.0f, 10.0f);
    SimplexNoise noisegen;

    for(int j=0; j<height; ++j)
    {
        for(int i=0; i<width; ++i)
        {
            float xc = i/(float)width;
            float yc = j/(float)height;

            _cloudsGrid.set(i, j,
                noisegen.noiseTile2d(xc + nsx, yc + nsy, 2.0f));

            float depth = (_cloudsGrid.get(i, j) + 1.0f) * 0.5f;

            float compact =
                    pow(clamp(_cloudyness - depth,
                              0.0f,
                              1.0f),
                        _cloudCompactness);

            _cloudsImage.setColorAt(i, j,
                RGBAColor(255, 255, 255, compact * 255));
        }
    }
}

void Sky::update()
{
}
