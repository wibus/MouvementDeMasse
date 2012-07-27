#include "Junction.h"


const std::string Junction::TYPE_STRINGS[Junction::NB_TYPES] = {
    "GRASS", "ASPHALT"
};

Junction::Junction() :
    _type(GRASS)
{
    for(int i=0; i<4; ++i)
        _streets[i].reset();
}

Junction::~Junction()
{
}

std::vector< std::shared_ptr<Street> > Junction::getOtherStreets(CardinalDirection direction)
{
    std::vector< std::shared_ptr<Street> > streets;

    for(int s=0; s<NB_DIRECTIONS; ++s)
    {
        if(s != direction && _streets[s].get() != 0x0)
            streets.push_back(_streets[s]);
    }

    return streets;
}
