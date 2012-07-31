#ifndef CITIZEN_H
#define CITIZEN_H

#include <vector>

#include <DataStructure/Vector.h>


class Path
{
public:
    enum NodeType {LAND, JUNCTION, BRIDGE_END, METRO, BUS_STOP, NB_NODE_TYPES};
    struct Node
    {
        Node(NodeType type, const cellar::Vec2i& pos) : type(type), pos(pos) {}
        NodeType type;
        cellar::Vec2i pos;
    };
    typedef std::vector< Node >  NodeVector;
    typedef NodeVector::iterator NodeIterator;

    Path();
    Path(const cellar::Vec2i& source, const cellar::Vec2i& destination);

    cellar::Vec2i source;
    cellar::Vec2i destination;    
    NodeVector    nodes;

    static const std::string NODE_TYPE_STRINGS[NB_NODE_TYPES];
};


class Citizen
{
public:
    enum State {AT_HOME, WORKING, SHOPING, MOVING, NB_STATES};
    typedef int Id;
    static const Id NO_ID = -1;

    Citizen();

    Id            id()          const {return _id;}
    State         state;
    float         walkingSpeed;
    cellar::Vec3f position;
    cellar::Vec3f direction;
    cellar::Vec2i homePos;      //Vec3i(MapX,  MapY)
    cellar::Vec2i workPos;      //Vec3i(MapX,  MapY)
    Path          homeToWorkPath;


    const static std::string STATE_STRINGS[NB_STATES];

private:
    Id _id;
    static Id __assigneId();
    static Id __nextId;
};

#endif // CITIZEN_H
