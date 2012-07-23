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

    State         state;
    cellar::Vec3f position;
    cellar::Vec3f direction;
    cellar::Vec2i homePos;
    cellar::Vec2i workPos;
    Path          homeToWorkPath;
    float         walkingSpeed;
};

#endif // CITIZEN_H
