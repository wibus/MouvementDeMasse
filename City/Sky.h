#ifndef SKY_H
#define SKY_H

#include <CellarWorkbench/DataStructure/Grid2D.h>
#include <CellarWorkbench/Image/Image.h>


class Sky
{
public:
    Sky(int width, int height);

    void update();

    cellar::Grid2D<float>& cloudsGrid();
    cellar::Image& cloudsImage();
    float cloudyness() const;
    float compactness() const;

    void setCloudyness(float zeroTOone);
    void setCompactness(float compact);

private:
    cellar::Grid2D<float>  _colorsGrid;
    cellar::Grid2D<float>  _cloudsGrid;
    cellar::Image          _cloudsImage;
    float _cloudyness;
    float _compactness;
};



// IMPLEMENTATION //
inline cellar::Grid2D<float>& Sky::cloudsGrid()
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

inline float Sky::compactness() const
{
    return _compactness;
}

inline void Sky::setCloudyness(float zeroTOone)
{
    _cloudyness = zeroTOone;
}

inline void Sky::setCompactness(float compact)
{
    _compactness = compact;
}

#endif // SKY_H
