#ifndef CITIZEN_H
#define CITIZEN_H

#include <vector>

#include <DataStructure/Vector.h>


class Path
{
public:
    enum NodeType {LAND, JUNCTION, METRO, BUS_STOP, NB_NODES};
    typedef std::vector< std::pair<NodeType, cellar::Vec2i> > NodeVector;

    Path();
    Path(const cellar::Vec2i& source, const cellar::Vec2i& destination);

    cellar::Vec2i source;
    cellar::Vec2i destination;    
    NodeVector    nodes;
};


class Citizen
{
public:
    enum State {AT_HOME, WORKING, SHOPING, MOVING, NB_STATES};

    Citizen();

    int           cid;
    State         state;
    cellar::Vec3f position;
    cellar::Vec3f direction;
    cellar::Vec2i homePos;      //Vec3i(MapX,  MapY)
    cellar::Vec3i homeRoom;     //Vec3i(BuildingX, BuildingY, Floor)
    cellar::Vec2i workPos;      //Vec3i(MapX,  MapY)
    cellar::Vec3i workRoom;     //Vec3i(BuildingX, BuildingY, Floor)
    Path          homeToWorkPath;
    float         walkingSpeed;

private:
    static int __assigneId();
    static int __nextId;
};

#endif // CITIZEN_H
