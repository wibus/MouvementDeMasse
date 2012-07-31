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

void Sun::setTime(const Calendar::Time& time)
{
    float angle = 2.0f * PI * (time.hour + (time.minute + time.second/60.0f)/60.0f)/24.0f;

    _rotationMatrix.loadIdentity();
    _rotationMatrix.rotate(
        _rotationAxis.x(),
        _rotationAxis.y(),
        _rotationAxis.z(),
        angle
    );

    _direction = _rotationMatrix * _midnightDirection;
}
