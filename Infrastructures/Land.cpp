#include "Land.h"

Land::Land() :
    _lands(),
    _hStreets(),
    _vStreets()
{
    for(int i=0; i<NB_DIRECTIONS; ++i)
        _lands[i] = nullptr;

    for(int i=0; i<NB_V_SIDES; ++i)
        _hStreets[i] = nullptr;
    for(int i=0; i<NB_H_SIDES; ++i)
        _vStreets[i] = nullptr;
}
