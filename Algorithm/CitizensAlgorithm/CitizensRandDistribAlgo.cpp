#include "CitizensRandDistribAlgo.h"

#include <queue>
#include <vector>
#include <stack>
#include <algorithm>
using namespace std;

#include <Misc/CellarUtils.h>
using namespace cellar;


CitizensRandDistribAlgo::CitizensRandDistribAlgo()
{
}

void CitizensRandDistribAlgo::setup(City &city)
{
    CitizensAlgorithm::setup( city );


    vector<Vec2i> residential;
    vector<Vec2i> commercial;
    Land* land;
    for(int j=0; j<_city->lands().height(); ++j)
    {
        for(int i=0; i<_city->lands().width(); ++i)
        {
            Vec2i pos(i, j);
            land = _city->lands().get(i, j);
            if(land->type() == Land::RESIDENTIAL && isAccessible(pos))
                residential.push_back( pos );
            else if(land->type() == Land::COMMERCIAL && isAccessible(pos))
                commercial.push_back( pos );
        }
    }


    int nbCitizens = 0;
    if(residential.size() != 0  &&  commercial.size() !=0)
        nbCitizens = (residential.size() + commercial.size());
    else
        return;


    initializeAStarStructures();

    for(int c=0; c<nbCitizens; ++c)
    {
        // Randomly find a home that have a junction nearby        
        Vec2i homePos = residential[ randomRange((size_t)0, residential.size()) ];
        Vec2i homeAccessPoint = randomAccessPointTo(homePos);

        // Randomly find a work that have a junction nearby
        Vec2i workPos = commercial[ randomRange((size_t)0, commercial.size()) ];
        Vec2i workAccessPoint = randomAccessPointTo(workPos);


        // Construct the path between the house and the work
        Path homeToWorkPath(homePos, workPos);
        homeToWorkPathByAStar(homeToWorkPath, homeAccessPoint, workAccessPoint);


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

        _city->citizens().insert(make_pair(ctz.id(), ctz));
    }

    cleanAStarStructures();
}


void CitizensRandDistribAlgo::update()
{
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
    Vec2i offsets[] = {
        Vec2i(0, 0),
        Vec2i(1, 0),
        Vec2i(1, 1),
        Vec2i(0, 1)
    };

    int turn = randomRange(0, 4);

    while(_city->junctions().get(pos + offsets[turn])->type() == Junction::GRASS)
        turn = (turn + 1) % 4;

    return pos + offsets[turn];
}


struct DijkstraNode
{
    DijkstraNode(const Path::Node& cur, const Path::NodeVector& lasts) :
        curNode(cur), lastNodes(lasts) {}

    Path::Node       curNode;
    Path::NodeVector lastNodes;
};

bool CitizensRandDistribAlgo::homeToWorkPathByDijkstra(Path& path, const Vec2i& src, const Vec2i& dst)
{
    // Helper grid //
    // 0 : Not visited
    // 1 : Visited
    // 2 : Destination
    const int UKN_TKN = 0;
    const int KNW_TKN = 1;
    const int DST_TKN = 2;
    Grid2D<int> juncs(_mapSize.x()+1, _mapSize.y()+1, UKN_TKN);
    juncs.set(src, KNW_TKN);
    juncs.set(dst, DST_TKN);

    queue< DijkstraNode > nodes;
    nodes.push( DijkstraNode(Path::Node(Path::JUNCTION, src), Path::NodeVector()) );


    while(!nodes.empty())
    {
        // Add the current node to the list of know nodes
        nodes.front().lastNodes.push_back(nodes.front().curNode);

        // Take out the node position and known nodes list
        Path::Node&       cNode    = nodes.front().curNode;
        Path::NodeVector& lNodes  = nodes.front().lastNodes;


        // For all cardinal directions
        for(int d=0; d<NB_DIRECTIONS; ++d)
        {
            // If there is a street connecting to a junction in that direction
            if(_city->junctions().get(cNode.pos)->getStreet(_cardinalDirections[d]) != 0x0)
            {
                Path::Node next(Path::JUNCTION, cNode.pos + toVec(_cardinalDirections[d]));

                // If the next node is unknown
                if(juncs.get(next.pos) == UKN_TKN)
                {
                    // Copy the current node and mark the next has known
                    nodes.push(DijkstraNode(next, lNodes));
                    juncs.set(next.pos, KNW_TKN);
                }

                // If the next node is the destination node
                else if(juncs.get(next.pos) == DST_TKN)
                {
                    lNodes.push_back( next );

                    // Set this lastNodes vector to be the chosen path
                    path.nodes = lNodes;

                    // Get out of the algorithm
                    // and show that it has found the destination
                    return true;
                }

                // Else if the node is already known, so already in the paths queue
                else if(juncs.get(next.pos) == KNW_TKN) continue;
            }
        }

        if(_bridgeEnds.find(cNode.pos) != _bridgeEnds.end())
        {
            BridgeIterator bIt = _city->bridges().begin();
            for(;bIt != _city->bridges().end(); ++bIt)
            {
                if(bIt->isAnEnd(cNode.pos))
                {
                    lNodes.back().type = Path::BRIDGE_END;
                    Path::Node next(Path::BRIDGE_END, bIt->otherEnd(cNode.pos));

                    // If the next node is unknown
                    if(juncs.get(next.pos) == UKN_TKN)
                    {
                        // Copy the current node and mark the next has known
                        nodes.push(DijkstraNode(next, lNodes));
                        juncs.set(next.pos, KNW_TKN);
                    }

                    // If the next node is the destination node
                    else if(juncs.get(next.pos) == DST_TKN)
                    {
                        lNodes.push_back( next );

                        // Set this lastNodes vector to be the chosen path
                        path.nodes = lNodes;

                        // Get out of the algorithm
                        // and show that it has found the destination
                        return true;
                    }

                    // Else if the node is already known, so already in the paths queue
                    else if(juncs.get(next.pos) == KNW_TKN) continue;
                }
            }
        }


        // Pop the curNode out of the queue
        nodes.pop();
    }

    // If the algorithm reach this point,
    // it has not found the destination from the source given in parameters
    return false;
}


AStarNode::AStarNode() :
    status(NOT_VISITED),
    distToDst(0.0f),
    distToSrc(0.0f),
    node(Path::Node(Path::JUNCTION, cellar::Vec2i(0, 0))),
    last(Path::Node(Path::JUNCTION, cellar::Vec2i(0, 0)))
{
}

AStarNode::AStarNode(Status status, float distToDst, float distToSrc,
                     Path::Node node, Path::Node last) :
    status(status),
    distToDst(distToDst),
    distToSrc(distToSrc),
    node(node),
    last(last)
{
}

bool CitizensRandDistribAlgo::homeToWorkPathByAStar(Path& path, const Vec2i& src, const Vec2i& dst)
{
    float inf = _city->size().x() * _city->size().y();

    for(int j=0; j<_aStarGrid.height(); ++j)
        for(int i=0; i<_aStarGrid.width(); ++i)
            _aStarGrid.set(i, j, AStarNode(AStarNode::NOT_VISITED, dist(Vec2i(i, j), dst), inf,
                                     Path::Node(Path::BUS_STOP, Vec2i(i, j)),
                                     Path::Node(Path::BUS_STOP, Vec2i(-1, -1))));

    vector< AStarNode > priority;
    priority.push_back( _aStarGrid.get(src) );

    while( !priority.empty() )
    {
        // Pop the nearest node
        make_heap(priority.begin(), priority.end());
        pop_heap( priority.begin(), priority.end());
        AStarNode curAsNode = priority.back();
        Vec2i     curPos    = curAsNode.node.pos;
        priority.pop_back();


        _aStarGrid.get(curPos).status = AStarNode::VISITED;
        if(curPos == dst)
        {
            stack< AStarNode > revPath;
            revPath.push( curAsNode );
            while(revPath.top().node.pos != src)
                revPath.push( _aStarGrid.get(revPath.top().last.pos) );

            while( !revPath.empty() )
            {
                path.nodes.push_back( revPath.top().node );
                revPath.pop();
            }

            return true;
        }


        vector<Path::Node>& candidates = _nodesToBeConsidered.get(curPos);

        for(size_t c=0; c<candidates.size(); ++c)
        {
            AStarNode& nextAsNode = _aStarGrid.get(candidates[c].pos);
            nextAsNode.node.type  = candidates[c].type;
            Vec2i nextPos         = candidates[c].pos;
            float distToSrc       = curAsNode.distToSrc + dist(curPos, nextPos);

            if(nextAsNode.status == AStarNode::VISITED)
                continue;

            else if(nextAsNode.status == AStarNode::IN_QUEUE)
            {
                if(distToSrc < nextAsNode.distToSrc)
                {
                    nextAsNode.distToSrc = distToSrc;
                    nextAsNode.last      = curAsNode.node;

                    for(size_t i=0; i<priority.size(); ++i)
                        if(priority[i].node.pos == nextPos)
                        {
                            priority[i].distToSrc = distToSrc;
                            priority[i].node.type = nextAsNode.node.type;
                            priority[i].last      = curAsNode.node;
                            break;
                        }
                }
            }

            else if(nextAsNode.status == AStarNode::NOT_VISITED)
            {
                AStarNode& newNode = _aStarGrid.get( nextPos );
                newNode.status    = AStarNode::IN_QUEUE;
                newNode.distToSrc = distToSrc;
                newNode.node      = nextAsNode.node;
                newNode.last      = curAsNode.node;
                priority.push_back( newNode );
            }
        }

        // End of that node
    }

    return false;
}

void CitizensRandDistribAlgo::initializeAStarStructures()
{
    for(size_t b=0; b<_city->bridges().size(); ++b)
    {
        _bridgeEnds.insert(_city->bridges()[b].endA());
        _bridgeEnds.insert(_city->bridges()[b].endB());
    }

    _aStarGrid = Grid2D< AStarNode >(_city->junctions().width(),
                                   _city->junctions().height());

    _nodesToBeConsidered = Grid2D< vector<Path::Node> >(_city->junctions().width(),
                                                           _city->junctions().height());
    for(int j=0; j<_nodesToBeConsidered.height(); ++j)
    {
        for(int i=0; i<_nodesToBeConsidered.width(); ++i)
        {
            Vec2i pos(i, j);

            // Junctions
            std::vector< std::shared_ptr<Street> > streets =
                _city->junctions().get(i, j)->getOtherStreets(NB_DIRECTIONS);

            for(size_t s=0; s<streets.size(); ++s)
                _nodesToBeConsidered.get(i, j).push_back(
                    Path::Node(Path::JUNCTION, streets[s]->oppositeJunction(pos))
                );


            // Bridge ends
            if(_bridgeEnds.find(pos) != _bridgeEnds.end())
                for(BridgeIterator bit = _city->bridges().begin(); bit != _city->bridges().end(); ++bit)
                    if(bit->isAnEnd(pos))
                    {

                        _nodesToBeConsidered.get(i, j).push_back(
                            Path::Node(Path::BRIDGE_END, bit->otherEnd(pos))
                        );
                        break;
                    }
        }
    }
}

void CitizensRandDistribAlgo::cleanAStarStructures()
{
    _bridgeEnds.clear();
    _aStarGrid = Grid2D< AStarNode >(0, 0);
    _nodesToBeConsidered = Grid2D< std::vector<Path::Node> >(0, 0);
}
