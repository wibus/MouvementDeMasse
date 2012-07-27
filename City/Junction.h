#ifndef JUNCTION_H
#define JUNCTION_H

#include <memory>
#include <vector>

#include "MdMTypes.h"

class Street;

class Junction
{
public:
    enum Type{GRASS, ASPHALT, NB_TYPES};

    Junction();
    virtual ~Junction();

    Type type() const;
    std::shared_ptr<Street> getStreet(CardinalDirection direction);
    std::vector< std::shared_ptr<Street> > getOtherStreets(CardinalDirection direction);

    void setType(Type type);
    void attach(const std::shared_ptr<Street>& street, CardinalDirection direction);


    static const std::string TYPE_STRINGS[NB_TYPES];

private:
    Type _type;
    std::shared_ptr<Street> _streets[NB_DIRECTIONS];    
};



// Implementation //
inline Junction::Type Junction::type() const
{
    return _type;
}

inline std::shared_ptr<Street> Junction::getStreet(CardinalDirection direction)
{
    return _streets[direction];
}

inline void Junction::setType(Type type)
{
    _type = type;
}

inline void Junction::attach(const std::shared_ptr<Street> &street, CardinalDirection direction)
{
    _streets[direction] = street;
}

#endif // JUNCTION_H
