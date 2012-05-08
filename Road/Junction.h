#ifndef JUNCTION_H
#define JUNCTION_H

#include<memory>

#include "MdMTypes.h"

class Street;

class Junction
{
public:
    Junction();
    virtual ~Junction();

    void attach(const std::shared_ptr<Street>& street, CardinalDirection direction);
    std::shared_ptr<Street> getStreet(CardinalDirection direction);

    void setHeight(float height);
    float height() const;

private:
    std::shared_ptr<Street> _streets[NB_DIRECTIONS];
    float _height;
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

#endif // JUNCTION_H
