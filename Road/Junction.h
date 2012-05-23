#ifndef JUNCTION_H
#define JUNCTION_H

#include<memory>

#include "MdMTypes.h"

class Street;

class Junction
{
public:
    enum Type{GROUND, ASPHALT, GRAVEL, NB_TYPES};
    Junction();
    virtual ~Junction();

    void attach(const std::shared_ptr<Street>& street, CardinalDirection direction);
    std::shared_ptr<Street> getStreet(CardinalDirection direction);

    void setHeight(float height);
    float height() const;

    void setType(Type type);
    Type type() const;

private:
    std::shared_ptr<Street> _streets[NB_DIRECTIONS];
    float _height;
    Type _type;
};



// Implementation //
inline void Junction::attach(const std::shared_ptr<Street> &street, CardinalDirection direction)
{
    _streets[direction] = street;
}

inline std::shared_ptr<Street> Junction::getStreet(CardinalDirection direction)
{
    return _streets[direction];
}

inline void Junction::setHeight(float height)
{
    _height = height;
}

inline float Junction::height() const
{
    return _height;
}

inline void Junction::setType(Type type)
{
    _type = type;
}

inline Junction::Type Junction::type() const
{
    return _type;
}

#endif // JUNCTION_H
