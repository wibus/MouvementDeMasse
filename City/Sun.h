#ifndef SUN_H
#define SUN_H

#include <MathsAndPhysics/Vector.h>
#include <MathsAndPhysics/Matrix4x4.h>

class Sun
{
public:
    Sun(const cellar::Vec4f& midnightDirection, const cellar::Vec3f& rotationAxis);

    virtual void setTime(unsigned int hour, unsigned int minutes);
    cellar::Vec4f direction() const;

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

#endif // SUN_H
