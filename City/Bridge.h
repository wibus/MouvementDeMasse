#ifndef BRIDGE_H
#define BRIDGE_H

#include <MathsAndPhysics/Vector.h>

class Bridge
{
public:
    Bridge(const cellar::Vec2i& endA, const cellar::Vec2i& endB);
private:
    cellar::Vec2i _endA;
    cellar::Vec2i _endB;
};

#endif // BRIDGE_H
