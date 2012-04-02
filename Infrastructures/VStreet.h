#ifndef VSTREET_H
#define VSTREET_H

#include "MdMTypes.h"


class VStreet
{
public:
    VStreet();

    void attachIntersection(VSide side, Intersection* intersecton);
    void attachLand(HSide side, Land* land);

    Intersection* getAttachedIntersection(VSide side) const;
    Land* getAttachedLand(HSide side) const;

private:
    Intersection* _intersections[NB_V_SIDES];
    Land* _lands[NB_H_SIDES];
};



// IMPLEMENTATION //
inline void VStreet::attachIntersection(VSide side, Intersection *intersecton)
    {_intersections[side] = intersecton;}
inline void VStreet::attachLand(HSide side, Land* land)
    {_lands[side] = land;}
inline Intersection* VStreet::getAttachedIntersection(VSide side) const
    {return _intersections[side];}
inline Land* VStreet::getAttachedLand(HSide side) const
    {return _lands[side];}

#endif // VSTREET_H
