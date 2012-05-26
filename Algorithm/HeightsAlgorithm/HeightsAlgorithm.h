#ifndef HEIGHTSALGORITHM_H
#define HEIGHTSALGORITHM_H

#include "Algorithm/Algorithm.h"

class HeightsAlgorithm : public Algorithm
{
protected:
    HeightsAlgorithm(); 

public:
    virtual ~HeightsAlgorithm();

    virtual void setup(CityMap& cityMap);
};

#endif // HEIGHTSALGORITHM_H
