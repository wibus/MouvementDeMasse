#include "Junction.h"

Junction::Junction()
{
    for (uint i = 0; i < NB_DIRECTIONS; i++)
    {
        _streets[i].reset(0x0);
    }
}

Junction::~Junction()
{
}
