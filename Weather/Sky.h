#ifndef SKY_H
#define SKY_H

#include <DataStructures/Grid.h>
#include <Graphics/Image.h>
#include <MathsAndPhysics/Vector.h>


class Sky
{
public:
    Sky(int width, int height);

    void update();

    cellar::Grid<float>& cloudsGrid();
    cellar::Image& cloudsImage();
    float cloudyness() const;

    void setCloudyness(float zeroTOone);

private:
    cellar::Grid<float>  _cloudsGrid;
    cellar::Image        _cloudsImage;
    float _cloudyness;
};



// IMPLEMENTATION //
inline cellar::Grid<float>& Sky::cloudsGrid()
{
    return _cloudsGrid;
}

inline cellar::Image& Sky::cloudsImage()
{
    return _cloudsImage;
}

inline float Sky::cloudyness() const
{
    return _cloudyness;
}

inline void Sky::setCloudyness(float zeroTOone)
{
    _cloudyness = zeroTOone;
}

#endif // SKY_H
