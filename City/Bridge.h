#ifndef BRIDGE_H
#define BRIDGE_H

#include <DataStructure/Vector.h>

class Bridge
{
public:
    Bridge(const cellar::Vec2i& endA, const cellar::Vec2i& endB);

    cellar::Vec2i& endA();
    cellar::Vec2i& endB();
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

#endif // BRIDGE_H
