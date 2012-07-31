#include "Land.h"


const std::string Land::TYPE_STRINGS[Land::NB_TYPES] = {
    "GRASS", "RESIDENTIAL", "COMMERCIAL"
};

int Land::_maxNbStories = 8;

Land::Land():
    _type(GRASS),
    _islandIdentifier(-1),
    _nbStories(1),
    _capacity(1),
    _nbResidents(0)
{
}

Land::~Land()
{
}
