#ifndef DRAWALGORITHM_H
#define DRAWALGORITHM_H

#include "Algorithm/Algorithm.h"


class DrawAlgorithm : public Algorithm
{
protected:
    DrawAlgorithm();

public:
    virtual ~DrawAlgorithm();

    virtual void setup(CityMap& cityMap);
    virtual void draw() = 0;

protected:
};

#endif // DRAWALGORITHM_H
