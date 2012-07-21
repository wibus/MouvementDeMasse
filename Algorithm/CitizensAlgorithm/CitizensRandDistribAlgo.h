#ifndef CITIZENSRANDDISTRIBALGO_H
#define CITIZENSRANDDISTRIBALGO_H

#include "CitizensAlgorithm.h"


class CitizensRandDistribAlgo : public CitizensAlgorithm
{
public:
    CitizensRandDistribAlgo();

    virtual void setup(City &city);
    virtual void update();

protected:
    cellar::Vec2i findRandomAccessibleLand(Land::Type type);
    void advancePos(cellar::Vec2i& pos);
    bool isAccessible(const cellar::Vec2i& pos);
    cellar::Vec2i randomAccessPointTo(const cellar::Vec2i& pos);
    bool homeToWorkPathByDijkstra(Path& path, const cellar::Vec2i& src, const cellar::Vec2i& dst);
};

#endif // CITIZENSRANDDISTRIBALGO_H
