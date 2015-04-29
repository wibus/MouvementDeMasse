#include "Street.h"


Street::Street(const glm::ivec2 &posA, const glm::ivec2 &posB)
{
    _junctions[0] = posA;
    _junctions[1] = posB;
}
