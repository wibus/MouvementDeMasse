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

void Sun::setTime(const Time& time)
{
    float angle = 2.0f * PI * time.dayCompleteness();

    _rotationMatrix.loadIdentity();
    _rotationMatrix.rotate(
        _rotationAxis.x(),
        _rotationAxis.y(),
        _rotationAxis.z(),
        angle
    );

    _direction = _rotationMatrix * _midnightDirection;
}
