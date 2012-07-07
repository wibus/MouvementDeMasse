#ifndef CITIZEN_H
#define CITIZEN_H

#include <MathsAndPhysics/Vector.h>


class Citizen
{
public:
    enum State {HOUSING, WORKING, SHOPING, MOVING, NB_STATES};

    Citizen();

    void init();

    State state() const;
    cellar::Vec2f position() const;
    cellar::Vec2i housePos() const;
    cellar::Vec2i workPos() const;
    cellar::Vec2i location() const;
    cellar::Vec2i desitnation() const;

protected:
    State _state;
    cellar::Vec2f _position;
    cellar::Vec2i _housePos;
    cellar::Vec2i _workPos;
    cellar::Vec2i _location;
    cellar::Vec2i _destination;
};

#endif // CITIZEN_H
