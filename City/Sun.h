#ifndef SUN_H
#define SUN_H

#include <GLM/glm.hpp>

#include <CellarWorkbench/DateAndTime/Calendar.h>

class Sun
{
public:
    Sun(const glm::vec4& midnightDirection, const glm::vec3& rotationAxis);

    virtual void setTime(const cellar::Time& time);
    glm::vec4 direction() const;
    glm::vec4 midnightDirection() const;
    glm::vec3 rotationAxis() const;
    const glm::mat4& rotationMatrix() const;

protected:
    glm::vec4 _direction;
    glm::vec4 _midnightDirection;
    glm::vec3 _rotationAxis;
    glm::mat4 _rotationMatrix;
};



// IMPLEMENTATION //
inline glm::vec4 Sun::direction() const
{
    return _direction;
}

inline glm::vec4 Sun::midnightDirection() const
{
    return _midnightDirection;
}

inline glm::vec3 Sun::rotationAxis() const
{
    return _rotationAxis;
}

inline const glm::mat4& Sun::rotationMatrix() const
{
    return _rotationMatrix;
}

#endif // SUN_H
