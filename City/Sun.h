#ifndef SUN_H
#define SUN_H

#include <DateAndTime/Calendar.h>
#include <DataStructure/Vector.h>
#include <DataStructure/Matrix.h>

class Sun
{
public:
    Sun(const cellar::Vec4f& midnightDirection, const cellar::Vec3f& rotationAxis);

    virtual void setTime(const cellar::Time& time);
    cellar::Vec4f direction() const;
    cellar::Vec4f midnightDirection() const;
    cellar::Vec3f rotationAxis() const;
    const cellar::Mat4f& rotationMatrix() const;

protected:
    cellar::Vec4f _direction;
    cellar::Vec4f _midnightDirection;
    cellar::Vec3f _rotationAxis;
    cellar::Mat4f _rotationMatrix;
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

inline const cellar::Mat4f& Sun::rotationMatrix() const
{
    return _rotationMatrix;
}

#endif // SUN_H
