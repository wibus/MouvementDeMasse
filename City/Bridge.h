#ifndef BRIDGE_H
#define BRIDGE_H

#include <DataStructure/Vector.h>

class Bridge
{
public:
    Bridge(const cellar::Vec2i& endA, const cellar::Vec2i& endB);

    cellar::Vec2i& endA();
    cellar::Vec2i& endB();
    cellar::Vec2i& otherEnd(const cellar::Vec2i end);
    bool           isAnEnd(const cellar::Vec2i end);

private:
    cellar::Vec2i _endA;
    cellar::Vec2i _endB;
};

// Implementation //
inline cellar::Vec2i& Bridge::endA()
{
    return _endA;
}

inline cellar::Vec2i& Bridge::endB()
{
    return _endB;
}

inline cellar::Vec2i& Bridge::otherEnd(const cellar::Vec2i end)
{
    if(end == _endA)
        return _endB;
    return _endA;
}

inline bool Bridge::isAnEnd(const cellar::Vec2i end)
{
    return (end == _endA || end == _endB);
}

#endif // BRIDGE_H
