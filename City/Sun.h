#ifndef SUN_H
#define SUN_H

#include <DataStructure/Vector.h>
#include <DataStructure/Matrix.h>

class Sun
{
public:
    Sun(const cellar::Vec4f& midnightDirection, const cellar::Vec3f& rotationAxis);

    virtual void setTime(unsigned int hour, unsigned int minutes, unsigned int seconds);
    cellar::Vec4f direction() const;
    cellar::Vec4f midnightDirection() const;
    cellar::Vec3f rotationAxis() const;

protected:
    cellar::Vec4f _direction;
    cellar::Vec4f _midnightDirection;
    cellar::Vec3f _rotationAxis;
    cellar::Matrix4x4<float> _rotationMatrix;
};



// IMPLEMENTATION //
inline cellar::Vec4f Sun::direction() const
{
    return _direction;
}

inline cellar::Vec4f Sun::midnightDirection() const
{
    return _midnightDirection;
}

inline cellar::Vec3f Sun::rotationAxis() const
{
    return _rotationAxis;
}

#endif // SUN_H
