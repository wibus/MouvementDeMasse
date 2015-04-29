#include "CitizensDistribByIsland.h"

#include <queue>
#include <vector>
#include <stack>
#include <map>
#include <algorithm>
using namespace std;

#include <CellarWorkbench/Misc/CellarUtils.h>
using namespace cellar;



class ivec2Less
{
public:
    inline bool operator ()(const glm::ivec2& p1, const glm::ivec2& p2)
    {
        if(p1.y < p2.y)
            return true;

        if(p1.y > p2.y)
            return false;

        if(p1.x < p2.x)
            return true;

        return false;
    }
};

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
    bool operator<(const AStarNode& node)
        {return (distToDst + distToSrc) > (node.distToDst + distToSrc);}
};

struct CitizensDistribByIsland::DataStructures
{
    map< glm::ivec2, glm::ivec2, ivec2Less > _bridgeEnds;
    Grid2D< AStarNode > _aStarGrid;
    Grid2D< std::vector<Path::Node> > _nodesToBeConsidered;
};


AStarNode::AStarNode() :
    status(NOT_VISITED),
    distToDst(0.0f),
    distToSrc(0.0f),
    node(Path::Node(Path::JUNCTION, glm::ivec2(0, 0))),
    last(Path::Node(Path::JUNCTION, glm::ivec2(0, 0)))
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


CitizensDistribByIsland::CitizensDistribByIsland() :
    _data(new DataStructures())
{
}

void CitizensDistribByIsland::setup(City &city)
{
    CitizensAlgorithm::setup( city );


    vector<glm::ivec2> residential;
    int residentialRooms = 0;

    vector<glm::ivec2> commercial;
    int commercialRooms = 0;

    Land* land;
    for(int j=0; j<_city->lands().getHeight(); ++j)
    {
        for(int i=0; i<_city->lands().getWidth(); ++i)
        {
            glm::ivec2 pos(i, j);
            land = _city->lands().get(i, j);
            if(land->type() == Land::RESIDENTIAL && isAccessible(pos))
            {
                residential.push_back( pos );
                residentialRooms += land->capacity();
            }
            else if(land->type() == Land::COMMERCIAL && isAccessible(pos))
            {
                commercial.push_back( pos );
                commercialRooms += land->capacity();
            }
        }
    }
    if(residentialRooms == 0  ||  commercialRooms ==0)
        return;


    initializeAStarStructures();
    int nbCitizens = minVal(residentialRooms, commercialRooms) / 16;
    float normWalkSp = _description->normalWalkingSpeed;


    for(int c=0; c<nbCitizens; ++c)
    {
        Citizen ctz;

        // Randomly find a home that have a junction nearby
        int homeIdx = randomRange(0, static_cast<int>(residential.size()));
        glm::ivec2 homePos = residential[ homeIdx ];
        glm::ivec2 homeAccessPoint = randomAccessPointTo(homePos);
        Land* home = _city->lands().get( homePos );
        home->allocateRoom( ctz.id() );
        if(home->isFull())
        {
            residential[homeIdx] = residential[residential.size()-1];
            residential.pop_back();
        }

        // Randomly find a work that have a junction nearby
        int workIdx = randomRange(0, static_cast<int>(commercial.size()));
        glm::ivec2 workPos = commercial[ workIdx ];
        glm::ivec2 workAccessPoint = randomAccessPointTo(workPos);
        Land* work = _city->lands().get( workPos );
        work->allocateRoom( ctz.id() );
        if(work->isFull())
        {
            commercial[workIdx] = commercial[commercial.size()-1];
            commercial.pop_back();
        }


        // Construct the path between the house and the work
        Path homeToWorkPath(homePos, workPos);
        homeToWorkPathByAStar(homeToWorkPath, homeAccessPoint, workAccessPoint);
        homeToWorkPath.gotoBegin();


        // Build The citizen
        ctz.curState = CITIZEN_AT_HOME; // Will be set in Move algorithm setup()
        ctz.position = glm::vec3(0, 0, 0);          // Will be set in Move algorithm setup()
        ctz.direction = glm::vec3(0.0f, 0.0f, 0.0f);
        ctz.homePos = homePos;
        ctz.workPos = workPos;
        ctz.homeToWorkPath = homeToWorkPath;
        ctz.walkingSpeed = normWalkSp + randomRange(-normWalkSp/3.0f, normWalkSp/3.0f);

        // Shifts : [0, 4] = Day; [5, 7] = Afternoon; [8, 9] = Night
        int shift = randomRange(0, 10);
        Time toGoWorking = Time().fromSeconds(ctz.homeToWorkPath.lenght / ctz.walkingSpeed);
             if(shift < 5) ctz.schedule.setDayShift(       toGoWorking );
        else if(shift < 8) ctz.schedule.setAfternoonShift( toGoWorking );
        else               ctz.schedule.setNightShift(     toGoWorking );

        _city->citizens().insert(make_pair(ctz.id(), ctz));
    }

    cleanAStarStructures();
}

void CitizensDistribByIsland::update()
{
}

bool CitizensDistribByIsland::isAccessible(const glm::ivec2& pos)
{
    if(_city->junctions().get(pos+glm::ivec2(0,0))->type() != Junction::GRASS) return true;
    if(_city->junctions().get(pos+glm::ivec2(1,0))->type() != Junction::GRASS) return true;
    if(_city->junctions().get(pos+glm::ivec2(1,1))->type() != Junction::GRASS) return true;
    if(_city->junctions().get(pos+glm::ivec2(0,1))->type() != Junction::GRASS) return true;
    return false;
}

glm::ivec2 CitizensDistribByIsland::randomAccessPointTo(const glm::ivec2& pos)
{
    const glm::ivec2 offsets[] = {
        glm::ivec2(0, 0),
        glm::ivec2(1, 0),
        glm::ivec2(1, 1),
        glm::ivec2(0, 1)
    };

    int turn = randomRange(0, 4);

    while(_city->junctions().get(pos + offsets[turn])->type() == Junction::GRASS)
        turn = (turn + 1) % 4;

    return pos + offsets[turn];
}

bool CitizensDistribByIsland::homeToWorkPathByAStar(Path& path, const glm::ivec2& src, const glm::ivec2& dst)
{
    Grid2D< AStarNode >& aStarGrid = _data->_aStarGrid;
    Grid2D< std::vector<Path::Node> >& nodesToBeConsidered = _data->_nodesToBeConsidered;

    float inf = _city->size().x * _city->size().y;

    for(int j=0; j<aStarGrid.getHeight(); ++j)
        for(int i=0; i<aStarGrid.getWidth(); ++i)
            aStarGrid.set(i, j, AStarNode(AStarNode::NOT_VISITED, dist(glm::ivec2(i, j), dst), inf,
                                     Path::Node(Path::BUS_STOP, glm::ivec2(i, j)),
                                     Path::Node(Path::BUS_STOP, glm::ivec2(-1, -1))));

    vector< AStarNode > priority;
    aStarGrid.get(src).distToSrc = 0.0f;
    priority.push_back( aStarGrid.get(src) );

    while( !priority.empty() )
    {
        // Pop the nearest node
        make_heap(priority.begin(), priority.end());
        pop_heap( priority.begin(), priority.end());
        AStarNode curAsNode = priority.back();
        glm::ivec2     curPos    = curAsNode.node.pos;
        priority.pop_back();


        aStarGrid.get(curPos).status = AStarNode::VISITED;

        if(curPos == dst)
        {
            path.lenght = curAsNode.distToSrc;
            stack< AStarNode > revPath;
            revPath.push( curAsNode );
            while(revPath.top().node.pos != src)
            {
                glm::ivec2 prevNode = revPath.top().last.pos;
                revPath.push( aStarGrid.get(prevNode) );
            }

            while( !revPath.empty() )
            {
                path.nodes.push_back( revPath.top().node );
                revPath.pop();
            }

            return true;
        }


        vector<Path::Node>& candidates = nodesToBeConsidered.get(curPos);

        for(size_t c=0; c<candidates.size(); ++c)
        {
            AStarNode& nextAsNode = aStarGrid.get(candidates[c].pos);
            nextAsNode.node.type  = candidates[c].type;
            glm::ivec2 nextPos         = candidates[c].pos;
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
                AStarNode& newNode = aStarGrid.get( nextPos );
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

void CitizensDistribByIsland::initializeAStarStructures()
{
    map< glm::ivec2, glm::ivec2, ivec2Less >& bridgeEnds = _data->_bridgeEnds;
    Grid2D< AStarNode >& aStarGrid = _data->_aStarGrid;
    Grid2D< std::vector<Path::Node> >& nodesToBeConsidered = _data->_nodesToBeConsidered;

    for(size_t b=0; b<_city->bridges().size(); ++b)
    {
        bridgeEnds.insert(make_pair(_city->bridges()[b].endA(), _city->bridges()[b].endB()));
        bridgeEnds.insert(make_pair(_city->bridges()[b].endB(), _city->bridges()[b].endA()));
    }

    aStarGrid = Grid2D< AStarNode >(_city->junctions().getWidth(),
                                   _city->junctions().getHeight());


    nodesToBeConsidered = Grid2D< vector<Path::Node> >(_city->junctions().getWidth(),
                                                       _city->junctions().getHeight());
    int ntbcWidth = nodesToBeConsidered.getWidth();
    int ntbcHeight = nodesToBeConsidered.getHeight();
    vector< std::shared_ptr<Street> > streets;
    for(int j=0; j<ntbcHeight; ++j)
    {
        for(int i=0; i<ntbcWidth; ++i)
        {
            glm::ivec2 pos(i, j);

            // Junctions
            streets.clear();
            streets = _city->junctions().get(i, j)->getOtherStreets(NB_DIRECTIONS);

            for(size_t s=0; s<streets.size(); ++s)
                nodesToBeConsidered.get(i, j).push_back(
                    Path::Node(Path::JUNCTION, streets[s]->oppositeJunction(pos))
                );


            // Bridge ends
            map< glm::ivec2, glm::ivec2, ivec2Less >::iterator bridge = bridgeEnds.find(pos);
            if(bridge != bridgeEnds.end())
            {
                nodesToBeConsidered.get(i, j).push_back(
                    Path::Node(Path::BRIDGE_END, bridge->second)
                );
            }
        }
    }
}

void CitizensDistribByIsland::cleanAStarStructures()
{
    _data->_bridgeEnds.clear();
    _data->_aStarGrid = Grid2D< AStarNode >(0, 0);
    _data->_nodesToBeConsidered = Grid2D< std::vector<Path::Node> >(0, 0);
}
