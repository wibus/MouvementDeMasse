#include "Sun.h"

#include <Misc/CellarUtils.h>
using namespace cellar;

Sun::Sun(const cellar::Vec4f& midnightDirection, const cellar::Vec3f& rotationAxis) :
    _direction(midnightDirection),
    _midnightDirection(midnightDirection),
    _rotationAxis(rotationAxis),
    _rotationMatrix()
{
}

void Sun::setTime(unsigned int hour, unsigned int minutes, unsigned int seconds)
{
    float angle = 2.0f * PI * (hour + (minutes + seconds/60.0f)/60.0f)/24.0f;

    _rotationMatrix.loadIdentity();
    _rotationMatrix.rotate(
        _rotationAxis.x(),
        _rotationAxis.y(),
        _rotationAxis.z(),
        angle
    );

    _direction = _rotationMatrix * _midnightDirection;
}
