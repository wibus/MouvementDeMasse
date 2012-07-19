#include "Citizen.h"

Path::Path() :
    destination(),
    source(),
    nodes()
{
}

Path::Path(const cellar::Vec2i& destination, const cellar::Vec2i& source) :
    destination(destination),
    source(source),
    nodes()
{
}

Citizen::Citizen(const cellar::Vec3f& pos) :
    state(Citizen::HOME),
    position( pos ),
    direction(1.0f, 0.0f, 0.0f),
    housePos(0, 0),
    workPos(0, 0),
    currentPath(),
    walkSpeed(1.0f)
{
}
