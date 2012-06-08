#include "Street.h"

using namespace cellar;


Street::Street(const Vec2i &posA, const Vec2i &posB)
{
    _junctions[0] = posA;
    _junctions[1] = posB;
}
