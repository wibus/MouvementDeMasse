#ifndef STREET_H
#define STREET_H

#include <cassert>

#include <GLM/glm.hpp>


class Street
{
public:
    Street(const glm::ivec2& posA, const glm::ivec2& posB);

    glm::ivec2 junctionA() const;
    glm::ivec2 junctionB() const;
    glm::ivec2 oppositeJunction(const glm::ivec2& junction);

private:
    glm::ivec2 _junctions[2];
};



// IMPLEMENTATION //
inline glm::ivec2 Street::junctionA() const
{
    return _junctions[0];
}

inline glm::ivec2 Street::junctionB() const
{
    return _junctions[1];
}

inline glm::ivec2 Street::oppositeJunction(const glm::ivec2& junction)
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
