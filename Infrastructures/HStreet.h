#ifndef HSTREET_H
#define HSTREET_H

#include "MdMTypes.h"


class HStreet
{
public:
    HStreet();

    void attachIntersection(HSide side, Intersection* intersecton);
    void attachLand(VSide side, Land* land);

    Intersection* getAttachedIntersection(HSide side) const;
    Land* getAttachedLand(VSide side) const;

private:
    Intersection* _intersections[NB_H_SIDES];
    Land* _lands[NB_V_SIDES];
};



// IMPLEMENTATION //
inline void HStreet::attachIntersection(HSide side, Intersection *intersecton)
    {_intersections[side] = intersecton;}
inline void HStreet::attachLand(VSide side, Land* land)
    {_lands[side] = land;}
inline Intersection* HStreet::getAttachedIntersection(HSide side) const
    {return _intersections[side];}
inline Land* HStreet::getAttachedLand(VSide side) const
    {return _lands[side];}

#endif // STREET_H
