#ifndef CITIZENSALGORITHM_H
#define CITIZENSALGORITHM_H

#include <CellarWorkbench/DateAndTime/Calendar.h>

#include "Algorithm/Algorithm.h"


class CitizensAlgorithm : public Algorithm
{
protected:
    CitizensAlgorithm();

public :
    ~CitizensAlgorithm();

    virtual void setup(City& city);
    virtual void update() = 0;
};

#endif // CITIZENSALGORITHM_H
