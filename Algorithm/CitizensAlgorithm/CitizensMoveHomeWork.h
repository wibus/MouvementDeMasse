#ifndef CITIZENSMOVEHOMEWORK_H
#define CITIZENSMOVEHOMEWORK_H

#include "CitizensAlgorithm.h"


class CitizensMoveHomeWork : public CitizensAlgorithm
{
public:
    CitizensMoveHomeWork();

    virtual void setup(City &city);
    virtual void update();

protected:
    float gotoWork(Citizen& ctz, float timeToMove);
    float gotoHome(Citizen& ctz, float timeToMove);
    void placeCitizen(Citizen& ctz);

    float _streetLength;
    float _juncLengthBeg;
    float _juncLengthEnd;
};

#endif // CITIZENSMOVEHOMEWORK_H
