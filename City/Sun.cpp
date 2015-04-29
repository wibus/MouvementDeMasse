#include "Sun.h"

#include <GLM/gtc/matrix_transform.hpp>

#include <CellarWorkbench/Misc/CellarUtils.h>
using namespace cellar;

Sun::Sun(const glm::vec4& midnightDirection, const glm::vec3& rotationAxis) :
    _direction(midnightDirection),
    _midnightDirection(midnightDirection),
    _rotationAxis(rotationAxis),
    _rotationMatrix()
{
}

void Sun::setTime(const Time& time)
{
    float angle = 2.0f * glm::pi<float>() * time.dayCompleteness();

    _rotationMatrix = glm::rotate(glm::mat4(), angle, glm::vec3(
        _rotationAxis.x,
        _rotationAxis.y,
        _rotationAxis.z));

    _direction = _rotationMatrix * _midnightDirection;
}
