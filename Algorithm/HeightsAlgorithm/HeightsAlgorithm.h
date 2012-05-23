#ifndef HEIGHTSALGORITHM_H
#define HEIGHTSALGORITHM_H

#include "Algorithm/Algorithm.h"

class HeightsAlgorithm : public Algorithm
{
protected:
    HeightsAlgorithm(); 

public:
    virtual ~HeightsAlgorithm();

    void setMinHeight(float minHeight);
    void setMaxHeight(float maxHeight);

    virtual void setup(CityMap& cityMap);

protected:
    float _minHeight;
    float _maxHeight;
};

#endif // HEIGHTSALGORITHM_H
