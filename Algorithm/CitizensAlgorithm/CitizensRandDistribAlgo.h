#ifndef CITIZENSRANDDISTRIBALGO_H
#define CITIZENSRANDDISTRIBALGO_H

#include <set>

#include "CitizensAlgorithm.h"


struct AStarNode
{
    enum Status {NOT_VISITED, IN_QUEUE, VISITED, NB_STATUS};
    Status status;
    float  distToDst;
    float  distToSrc;
    Path::Node node;
    Path::Node last;

    AStarNode();
    AStarNode(Status status, float distToDst, float distToSrc,
              Path::Node node, Path::Node last);
    bool operator<(const AStarNode& node);
};


class CitizensRandDistribAlgo : public CitizensAlgorithm
{
public:
    CitizensRandDistribAlgo();

    virtual void setup(City &city);
    virtual void update();

protected:
    bool isAccessible(const cellar::Vec2i& pos);
    cellar::Vec2i randomAccessPointTo(const cellar::Vec2i& pos);
    bool homeToWorkPathByDijkstra(Path& path, const cellar::Vec2i& src, const cellar::Vec2i& dst);


    bool homeToWorkPathByAStar(Path& path, const cellar::Vec2i& src, const cellar::Vec2i& dst);
    void initializeAStarStructures();
    void cleanAStarStructures();
    float dist(const cellar::Vec2i& p1, const cellar::Vec2i& p2);

    std::set< cellar::Vec2i > _bridgeEnds;
    cellar::Grid2D< AStarNode > _aStarGrid;
    cellar::Grid2D< std::vector<Path::Node> > _nodesToBeConsidered;
};



// IMPLEMENTATION //
inline float CitizensRandDistribAlgo::dist(const cellar::Vec2i& p1, const cellar::Vec2i &p2)
{
    return cellar::Vec2f(p1).distanceTo(p2);
}

inline bool AStarNode::operator<(const AStarNode& node)
{
    return (distToDst + distToSrc) > (node.distToDst + distToSrc);
}

#endif // CITIZENSRANDDISTRIBALGO_H
