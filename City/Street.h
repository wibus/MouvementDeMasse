#ifndef STREET_H
#define STREET_H

#include <cassert>
#include <DataStructure/Vector.h>

class Street
{
public:
    Street(const cellar::Vec2i& posA, const cellar::Vec2i& posB);

    cellar::Vec2i junctionA() const;
    cellar::Vec2i junctionB() const;
    cellar::Vec2i oppositeJunction(const cellar::Vec2i& junction);

private:
    cellar::Vec2i _junctions[2];
};



// IMPLEMENTATION //
inline cellar::Vec2i Street::junctionA() const
{
    return _junctions[0];
}

inline cellar::Vec2i Street::junctionB() const
{
    return _junctions[1];
}

inline cellar::Vec2i Street::oppositeJunction(const cellar::Vec2i& junction)
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
