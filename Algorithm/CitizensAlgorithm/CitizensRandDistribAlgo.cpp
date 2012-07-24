#include "CitizensRandDistribAlgo.h"

#include <queue>
using namespace std;

#include <Misc/CellarUtils.h>
using namespace cellar;


CitizensRandDistribAlgo::CitizensRandDistribAlgo()
{
}

void CitizensRandDistribAlgo::setup(City &city)
{
    CitizensAlgorithm::setup( city );

    int nbCitizens = _mapSize.x()*10;

    for(int c=0; c<nbCitizens; ++c)
    {
        // Randomly find a home that have a junction nearby
        Vec2i homePos = findRandomAccessibleLand(Land::RESIDENTIAL);
        Vec2i homeAccessPoint = randomAccessPointTo(homePos);


        // Randomly find a work that have a junction nearby
        Vec2i workPos = findRandomAccessibleLand(Land::COMMERCIAL);
        Vec2i workAccessPoint = randomAccessPointTo(workPos);


        // Construct the path between the house and the work
        Path homeToWorkPath(workPos, homePos);
        homeToWorkPathByDijkstra( homeToWorkPath, homeAccessPoint, workAccessPoint);

        // Set the citizen walking speed
        Calendar::Clock::TimeJump timeJump = _city->calendar().clock().timeJump;
        float normWalkSp = _description->normalWalkingSpeed *
                           (timeJump == Calendar::Clock::MINUTE ? 60.0f : 1.0f);
        float walkingSpeed = normWalkSp + randomRange(-normWalkSp/3.0f, normWalkSp/3.0f);


        // Build The citizen
        Citizen ctz;
        ctz.state = Citizen::AT_HOME;
        ctz.position = vec3<float>(homePos, _ground->heightAt(homePos) + randomRange(1.0f, 2.0f)) + Vec3f(0.5f, 0.5f, 0.0f);
        ctz.direction(0.0f, 0.0f, 0.0f);
        ctz.homePos = homePos;
        ctz.workPos = workPos;
        ctz.homeToWorkPath = homeToWorkPath;
        ctz.walkingSpeed = walkingSpeed;

        _city->citizens().push_back( ctz );
    }
}

void CitizensRandDistribAlgo::update()
{
}

Vec2i CitizensRandDistribAlgo::findRandomAccessibleLand(Land::Type type)
{
    bool landFound = false;
    Vec2i landPos(randomRange(0, _city->lands().width()),
                  randomRange(0, _city->lands().height()));

    while(!landFound)
    {
        while(_city->lands().get(landPos)->type() != type)
            advancePos(landPos);

        if(isAccessible( landPos ))
            landFound = true;
        else
            advancePos(landPos);
    }

    return landPos;
}

void CitizensRandDistribAlgo::advancePos(Vec2i& pos)
{
    ++pos[0];
    if(pos[0] >= _city->lands().width())
    {
        pos[0] = 0;

        ++pos[1];
        if(pos[1] >= _city->lands().height())
            pos[1] = 0;
    }
}

bool CitizensRandDistribAlgo::isAccessible(const Vec2i& pos)
{
    if(_city->junctions().get(pos+Vec2i(0,0))->type() != Junction::GRASS) return true;
    if(_city->junctions().get(pos+Vec2i(1,0))->type() != Junction::GRASS) return true;
    if(_city->junctions().get(pos+Vec2i(1,1))->type() != Junction::GRASS) return true;
    if(_city->junctions().get(pos+Vec2i(0,1))->type() != Junction::GRASS) return true;
    return false;
}

Vec2i CitizensRandDistribAlgo::randomAccessPointTo(const Vec2i& pos)
{
    Vec2f landPos(pos.x() + 0.5f, pos.y() + 0.5f);
    Vec2f streetDir(0.5f, 0.0f);
    Vec2f cornerDir;
    Vec2f corner;
    Vec2i juncPos;

    int turnOffset = randomRange(0, 4);
    for(int i=0; i<turnOffset; ++i) streetDir.rotateQuarterCCW();

    cornerDir = streetDir;
    cornerDir.rotateQuarterCW();
    corner = landPos + streetDir + cornerDir;
    juncPos(round(corner.x()), round(corner.y()));

    while(_city->junctions().get(juncPos)->type() == Junction::GRASS)
    {
        streetDir.rotateQuarterCCW();
        cornerDir.rotateQuarterCCW();
        corner = landPos + streetDir + cornerDir;
        juncPos(round(corner.x()), round(corner.y()));
    }

    return juncPos;
}

bool CitizensRandDistribAlgo::homeToWorkPathByDijkstra(Path& path, const Vec2i& src, const Vec2i& dst)
{
    // Helper grid //
    // 0 : Not visited
    // 1 : Visited
    // 2 : Destination
    const int UKN_TKN = 0;
    const int KNW_TKN = 1;
    const int DST_TKN = 2;
    Grid<int> juncs(_mapSize.x()+1, _mapSize.y()+1, UKN_TKN);
    juncs.set(src, KNW_TKN);
    juncs.set(dst, DST_TKN);

    queue< pair<Vec2i, vector<Vec2i> > > nodes; // pair<current node, previous nodes>
    nodes.push( make_pair(src, vector<Vec2i>()) );

    while(!nodes.empty())
    {
        // Add the current node to the list of know nodes
        nodes.front().second.push_back(nodes.front().first);

        // Take out the node position and known nodes list
        Vec2i&         curNode = nodes.front().first;
        vector<Vec2i>& knowns  = nodes.front().second;

        // For all cardinal directions
        for(int d=0; d<NB_DIRECTIONS; ++d)
        {
            // If there is a street connecting to a junction in that direction
            if(_city->junctions().get(curNode)->getStreet(_cardinalDirections[d]) != 0x0)
            {
                Vec2i next = curNode + toVec(_cardinalDirections[d]);

                // If the next node is unknown
                if(juncs.get(next) == UKN_TKN)
                {
                    // Copy the current node and mark the next has known
                    nodes.push(make_pair(next, knowns));
                    juncs.set(next, KNW_TKN);
                }

                // If the next node is the destination node
                else if(juncs.get(next) == DST_TKN)
                {
                    knowns.push_back( next );

                    // Build path from previously visited nodes
                    for(size_t n=0; n<knowns.size(); ++n)
                        path.nodes.push_back(make_pair(Path::JUNCTION, knowns[n]));

                    // Get out of the algorithm
                    // and show that it has found the destination
                    return true;
                }

                // Else if the node is already known, so already in the paths queue
                else if(juncs.get(next) == KNW_TKN) continue;
            }
        }

        // Pop the curNode out of the queue
        nodes.pop();
    }

    // If the algorithm reach this point,
    // it has not found the destination from the source given in parameters
    return false;
}
