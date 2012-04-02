#ifndef LAND_H
#define LAND_H

#include "MdMTypes.h"


class Land
{
public:
    Land();

    void attachLand(CardinalDirection direction, Land* land);
    void attachHStreet(VSide side, HStreet* street);
    void attachVStreet(HSide side, VStreet* street);

    Land* getAttachedLand(CardinalDirection direction) const;
    HStreet* getAttachedHStreet(VSide side) const;
    VStreet* getAttachedVStreet(HSide side) const;

private:
    Land* _lands[NB_DIRECTIONS];
    HStreet* _hStreets[NB_V_SIDES];
    VStreet* _vStreets[NB_H_SIDES];
};



// IMPLEMENTATION //
inline void Land::attachLand(CardinalDirection direction, Land* land)
    {_lands[direction] = land;}
inline void Land::attachHStreet(VSide side, HStreet* street)
    {_hStreets[side] = street;}
inline void Land::attachVStreet(HSide side, VStreet* street)
    {_vStreets[side] = street;}
inline Land* Land::getAttachedLand(CardinalDirection direction) const
    {return _lands[direction];}
inline HStreet *Land::getAttachedHStreet(VSide side) const
    {return _hStreets[side];}
inline VStreet *Land::getAttachedVStreet(HSide side) const
    {return _vStreets[side];}

#endif // LOT_H
