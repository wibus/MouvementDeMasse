#include "Sky.h"

#include <MathsAndPhysics/Algorithms.h>
using namespace cellar;


Sky::Sky(int width, int height) :
    _cloudsGrid(width, height, 0.0f),
    _cloudsImage(new unsigned char[width*height*4], width, height, Image::RGBA),
    _cloudyness(0.0f)
{
    perlinNoise(LinearWeighter(0, 100), _cloudsGrid);

    for(int j=0; j<height; ++j)
        for(int i=0; i<width; ++i)
            _cloudsImage.setColorAt(i, j,
                RGBAColor(255, 255, 255, 255*(_cloudsGrid.get(i, j)+1.0f)*0.5f));
}

void Sky::update()
{
}
