#ifndef CITIZENSDISTRIBBYISLAND_H
#define CITIZENSDISTRIBBYISLAND_H

#include "CitizensAlgorithm.h"


class CitizensDistribByIsland : public CitizensAlgorithm
{
public:
    CitizensDistribByIsland();

    virtual void setup(City &city);
    virtual void update();

protected:
    float dist(const cellar::Vec2i& p1, const cellar::Vec2i& p2);
    bool isAccessible(const cellar::Vec2i& pos);
    cellar::Vec2i randomAccessPointTo(const cellar::Vec2i& pos);

    bool homeToWorkPathByAStar(Path& path, const cellar::Vec2i& src, const cellar::Vec2i& dst);
    void initializeAStarStructures();
    void cleanAStarStructures();


    class DataStructures;
    DataStructures* _data;
};



// IMPLEMENTATION //
inline float CitizensDistribByIsland::dist(const cellar::Vec2i& p1, const cellar::Vec2i &p2)
{
    return cellar::Vec2f(p1).distanceTo(p2);
}

#endif // CITIZENSDISTRIBBYISLAND_H
