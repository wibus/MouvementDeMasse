#include "City.h"

#include <iostream>

using namespace std;
//using namespace cellar;
//using namespace prop;
using namespace scaena;



City::City(AbstractStage &stage) :
    AbstractCharacter(stage, "City")
{
}

City::~City()
{
}

void City::enterStage()
{
}

void City::draw(const scaena::StageTime &time)
{
}

void City::beginStep(const scaena::StageTime &time)
{
}

void City::exitStage()
{
}


bool City::keyPressEvent(const scaena::KeyboardEvent &event)
{
    cout << "Hello!" << endl;
}

bool City::mousePressEvent(const scaena::MouseEvent &event)
{
}

bool City::mouseMoveEvent(const scaena::MouseEvent &event)
{
}
