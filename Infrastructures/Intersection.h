#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "MdMTypes.h"


class Intersection
{
public:
    Intersection();

    void attachHStreet(HSide side, HStreet* street);
    void attachVStreet(VSide side, VStreet* street);
    HStreet* getAttahedHStreet(HSide side) const;
    VStreet* getAttahedVStreet(VSide side) const;

private:
    HStreet* _hStreets[NB_H_SIDES];
    VStreet* _vStreets[NB_V_SIDES];
};



// IMPLEMENTATION //
inline void Intersection::attachHStreet(HSide side, HStreet* street)
    {_hStreets[side] = street;}
inline void Intersection::attachVStreet(VSide side, VStreet* street)
    {_vStreets[side] = street;}
inline HStreet* Intersection::getAttahedHStreet(HSide side) const
    {return _hStreets[side];}
inline VStreet* Intersection::getAttahedVStreet(VSide side) const
    {return _vStreets[side];}

#endif // INTERSECTION_H
