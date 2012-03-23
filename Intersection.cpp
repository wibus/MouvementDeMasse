#include "Intersection.h"

Intersection::Intersection()
{
    for(int i=0; i<NB_DIRECTIONS; ++i)
        _neightbors[i] = nullptr;
}
