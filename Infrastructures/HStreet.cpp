#include "HStreet.h"

HStreet::HStreet() :
    _intersections(),
    _lands()
{
    for(int i=0; i<NB_H_SIDES; ++i)
        _intersections[i] = nullptr;

    for(int i=0; i<NB_V_SIDES; ++i)
        _lands[i] = nullptr;
}
