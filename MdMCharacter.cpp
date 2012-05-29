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
#include "Algorithm/DrawAlgorithm/DrawSunAndGround.h"


MdMCharacter::MdMCharacter(AbstractStage& stage) :
    AbstractCharacter(stage, "MdMCharacter"),
    _cityMap( new CityMap(200, 200, Vec2f(-20.0, 20.0))),    
    _camMan( stage.camera() ),
    _fpsText()
{
    _fpsText.setPosition(5, 5);

    stage.camera().setTripod(Vec3f(-10, -10, 10),
                             Vec3f(0, 0, 0),
                             Vec3f(0, 0 ,1));
    stage.camera().registerObserver( *this );
}

void MdMCharacter::enterStage()
{
    cout << "I'm here" << endl;

    setAlgorithms();
    _calendar.start();
    stage().camera().refresh();
}

void MdMCharacter::beginStep(const StageTime &time)
{
    _calendar.tic();
    _dateText.setPosition(10, stage().height() - 20);
    _dateText.setText(_calendar.dateToString());

    const float speed = 10.0f;

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
    //_cityMap->drawAlgorithm().draw();
    _drawAlgorithm->draw();
    _dateText.draw();
    _fpsText.setText( toString(1.0f / time.elapsedTime()) );
    _fpsText.draw();
}

void MdMCharacter::exitStage()
{
    cout << "Goodbye" << endl;
    _calendar.stop();
}

void MdMCharacter::notify(cellar::CameraMsg &msg)
{
    if(msg.change == CameraMsg::PROJECTION)
    {
        _drawAlgorithm->updateProjectionMatrix( msg.camera.projectionMatrix() );
    }
    else if(msg.change == CameraMsg::VIEW)
    {
        _drawAlgorithm->updateViewMatrix( msg.camera.viewMatrix() );
    }
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
    _cityMap->reset();

    // Height algorithm
    HeightByNoiseAlgo* heightAlgo = new HeightByNoiseAlgo();
    heightAlgo->setNbNoises( cellar::min(_cityMap->size().x(), _cityMap->size().y()) / 2 );
    heightAlgo->setWeightedNoisesRange(1, heightAlgo->nbNoises());
    heightAlgo->setup( *_cityMap );

    // Draw algorithm
    DrawSun* drawSun = new  DrawSunAndGround();
    drawSun->setup( *_cityMap );
    _drawAlgorithm.reset( drawSun );

    _cityMap->setup();
}
