#include "Land.h"
using namespace cellar;


const std::string Land::TYPE_STRINGS[Land::NB_TYPES] = {
    "GRASS", "RESIDENTIAL", "COMMERCIAL"
};

const int Land::NB_ROOMS_BY_STORY = 9;
int Land::_maxNbStories = 8;


Land::Land():
    _type(GRASS),
    _nbStories(0),
    _capacity(0),
    _islandIdentifier(-1),
    _residents()
{
}

Land::~Land()
{
}
