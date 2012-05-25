#include <iostream>
using namespace std;

#include <Misc/CellarUtils.h>
using namespace cellar;

#include <StageEvents/StageTime.h>
#include <StageEvents/SynchronousKeyboard.h>
#include <StageEvents/SynchronousMouse.h>
#include <Stages/AbstractStage.h>
using namespace scaena;

#include "MdMCharacter.h"
#include "CityMap.h"

#include "Algorithm/HeightsAlgorithm/HeightsByNoiseAlgo.h"
#include "Algorithm/DrawAlgorithm/DrawNudeHills.h"


MdMCharacter::MdMCharacter(AbstractStage& stage) :
    AbstractCharacter(stage, "MdMCharacter"),
    _cityMap( new CityMap(200, 200)),
    _fps(),
    _camMan( stage.camera() )
{
    _fps.setPosition(5, 5);

    stage.camera().setTripod(Vec3f(-10, -10, 10),
                             Vec3f(0, 0, 0),
                             Vec3f(0, 0 ,1));
}

void MdMCharacter::enterStage()
{
    cout << "I'm here" << endl;

    setAlgorithms();
    _cityMap->setup();

    stage().camera().refresh();
}

void MdMCharacter::beginStep(const StageTime &time)
{
    float speed = 10.0f;

    if(stage().synchronousKeyboard().isAsciiPressed('w'))
    {
        _camMan.forward(time.elapsedTime() * speed);
    }
    if(stage().synchronousKeyboard().isAsciiPressed('s'))
    {
        _camMan.forward(-time.elapsedTime() * speed);
    }
    if(stage().synchronousKeyboard().isAsciiPressed('a'))
    {
        _camMan.sideward(-time.elapsedTime() * speed);
    }
    if(stage().synchronousKeyboard().isAsciiPressed('d'))
    {
        _camMan.sideward(time.elapsedTime() * speed);
    }

    if(stage().synchronousMouse().displacement() != Vec2i(0, 0) &&
       stage().synchronousMouse().buttonIsPressed(Mouse::LEFT))
    {
        _camMan.turnHorizontaly(stage().synchronousMouse().displacement().x() * 0.004f);
        _camMan.turnVerticaly(stage().synchronousMouse().displacement().y() * 0.004f);
    }
}

void MdMCharacter::endStep(const StageTime &time)
{
}

void MdMCharacter::draw(const scaena::StageTime &time)
{
    _cityMap->drawAlgorithm().draw();

    _fps.setText( toString(1.0f / time.elapsedTime()) );
    _fps.draw();
}

void MdMCharacter::exitStage()
{
    cout << "Goodbye" << endl;
}

CityMap& MdMCharacter::cityMap()
{
    return *_cityMap;
}

void MdMCharacter::setCityMap(CityMap* map)
{
    _cityMap.reset( map );
}

void MdMCharacter::setAlgorithms()
{
    // Height algorithm
    HeightByNoiseAlgo* heightAlgo = new HeightByNoiseAlgo();
    heightAlgo->setNbNoises( cellar::min(_cityMap->size().x(), _cityMap->size().y()) / 2 );
    heightAlgo->setWeightedNoisesRange(1, heightAlgo->nbNoises());
    heightAlgo->setMinHeight(-14.0);
    heightAlgo->setMaxHeight(20.0);
    _cityMap->setHeightsAlgorithm( heightAlgo );

    // Draw algorithm
    DrawNudeHills* drawAlgo = new DrawNudeHills();
    stage().camera().registerObserver( *drawAlgo );
    _cityMap->setDrawAlgorithm( drawAlgo );
}
