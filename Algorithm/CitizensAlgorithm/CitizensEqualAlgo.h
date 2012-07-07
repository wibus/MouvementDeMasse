#ifndef CITIZENSEQUALALGO_H
#define CITIZENSEQUALALGO_H

#include "CitizensAlgorithm.h"


class CitizensEqualAlgo : public CitizensAlgorithm
{
public:
    CitizensEqualAlgo();

    virtual void setup(City &city);
    virtual void update();
};

#endif // CITIZENSEQUALALGO_H
