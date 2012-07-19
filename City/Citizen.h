#ifndef CITIZEN_H
#define CITIZEN_H

#include <vector>

#include <MathsAndPhysics/Vector.h>


class Path
{
public:
    enum NodeType {NODE_JUNCTION, NODE_METRO, NODE_BUS_STOP, NB_NODES};
    typedef std::vector< std::pair<NodeType, std::string> > NodeVector;

    Path();
    Path(const cellar::Vec2i& destination, const cellar::Vec2i& source);


    cellar::Vec2i destination;
    cellar::Vec2i source;
    NodeVector    nodes;
};


class Citizen
{
public:
    enum State {HOME, WORKING, SHOPING, MOVING, NB_STATES};

    Citizen(const cellar::Vec3f& pos);

    State         state;
    cellar::Vec3f position;
    cellar::Vec3f direction;
    cellar::Vec2i housePos;
    cellar::Vec2i workPos;
    Path          currentPath;
    float         walkSpeed;
};

#endif // CITIZEN_H
