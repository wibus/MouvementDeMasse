#include "Citizen.h"


const std::string Path::NODE_TYPE_STRINGS[Path::NB_NODE_TYPES] = {
    "LAND", "JUNCTION", "BRIDGE_END", "METRO", "BUS_STOP"
};

Path::Path() :
    source(),
    destination(),
    nodes()
{
}

Path::Path(const cellar::Vec2i& source, const cellar::Vec2i& destination) :
    source(source),
    destination(destination),
    nodes()
{
}


int Citizen::__nextId = 0;
int Citizen::__assigneId() {return __nextId++;}

const std::string Citizen::STATE_STRINGS[Citizen::NB_STATES] = {
    "AT_HOME", "WORKING", "SHOPING", "MOVING"
};

Citizen::Citizen() :    
    state(Citizen::AT_HOME),
    walkingSpeed(1.0f),
    position(0.0f, 0.0f, 0.0f),
    direction(1.0f, 0.0f, 0.0f),
    homePos(0, 0),
    workPos(0, 0),
    homeToWorkPath(),
    _id(__assigneId())
{
}
