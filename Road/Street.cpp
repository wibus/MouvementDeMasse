#include "Street.h"

using namespace cellar;


Street::Street(const Vec2ui &posA, const Vec2ui &posB)
{
    _junctions[0] = posA;
    _junctions[1] = posB;
}
