#ifndef BRIDGE_H
#define BRIDGE_H

#include <GLM/glm.hpp>


class Bridge
{
public:
    Bridge(const glm::ivec2& endA, const glm::ivec2& endB);

    glm::ivec2& endA();
    glm::ivec2& endB();
    glm::ivec2& otherEnd(const glm::ivec2 end);
    bool           isAnEnd(const glm::ivec2 end);

private:
    glm::ivec2 _endA;
    glm::ivec2 _endB;
};

// Implementation //
inline glm::ivec2& Bridge::endA()
{
    return _endA;
}

inline glm::ivec2& Bridge::endB()
{
    return _endB;
}

inline glm::ivec2& Bridge::otherEnd(const glm::ivec2 end)
{
    if(end == _endA)
        return _endB;
    return _endA;
}

inline bool Bridge::isAnEnd(const glm::ivec2 end)
{
    return (end == _endA || end == _endB);
}

#endif // BRIDGE_H
