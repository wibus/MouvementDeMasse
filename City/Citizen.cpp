#include "Citizen.h"

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

Citizen::Citizen() :
    cid(__assigneId()),
    state(Citizen::AT_HOME),
    position(0.0f, 0.0f, 0.0f),
    direction(1.0f, 0.0f, 0.0f),
    homePos(0, 0),
    workPos(0, 0),
    homeToWorkPath(),
    walkingSpeed(1.0f)
{
}
