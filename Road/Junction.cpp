#include "Junction.h"

Junction::Junction() :
    _type(GRASS)
{
    for(int i=0; i<4; ++i)
        _streets[i].reset();
}

Junction::~Junction()
{
}
