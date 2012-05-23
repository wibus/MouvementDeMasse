#ifndef STREET_H
#define STREET_H

#include <cassert>
#include <MathsAndPhysics/Vector.h>

class Street
{
public:
    Street(const cellar::Vec2ui& posA, const cellar::Vec2ui& posB);

    cellar::Vec2ui junctionA() const;
    cellar::Vec2ui junctionB() const;
    cellar::Vec2ui oppositeJunction(const cellar::Vec2ui& junction);

private:
    cellar::Vec2ui _junctions[2];
};



// IMPLEMENTATION //
inline cellar::Vec2ui Street::junctionA() const
{
    return _junctions[0];
}

inline cellar::Vec2ui Street::junctionB() const
{
    return _junctions[1];
}

inline cellar::Vec2ui Street::oppositeJunction(const cellar::Vec2ui& junction)
{
    if (junction != _junctions[0])
    {
        if (junction != _junctions[1])
            assert(false/* Not a junction connected to that street */);
        else
            return _junctions[0];
    }

    return _junctions[1];
}

#endif // STREET_H
