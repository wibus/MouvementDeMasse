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
#include "City/CityMap.h"
#include "Algorithm/HeightsAlgorithm/HeightsByNoiseAlgo.h"
#include "Algorithm/MapElementsAlgorithm/MapElementsDepthFirst.h"


MdMCharacter::MdMCharacter(AbstractStage& stage) :
    AbstractCharacter(stage, "MdMCharacter"),
    _cityMap( new CityMap(100, 100)),
    _drawCityAlgorithm(*_cityMap),
    _camMan( stage.camera() ),
    _fpsText()
{
    _fpsText.setPosition(5, 5);

    stage.camera().setTripod(Vec3f(_cityMap->size().x() / 2, 0, _cityMap->ground().maxHeight()),
                             Vec3f(_cityMap->size().x() / 2, _cityMap->size().y() / 2, 0),
                             Vec3f(0, 0 ,1));
    stage.camera().registerObserver( *this );

    _calendar.setDate(Calendar::Date(2000, Calendar::Date::JANUARY, 1, 0, 0));
}

void MdMCharacter::enterStage()
{
    _calendar.setClock(Calendar::Clock(Calendar::Clock::MINUTE));
    _calendar.start();
    updateCalendar();

    stage().camera().refresh();

    setAlgorithms();
}

void MdMCharacter::beginStep(const StageTime &time)
{
    updateCalendar();
    updateCamera( time.elapsedTime() );
    _drawCityAlgorithm.update();
}

void MdMCharacter::updateCalendar()
{
    _calendar.tic();

    _dateText.setText(_calendar.date().toString(true, true));

    _cityMap->sun().setTime(_calendar.date().hour, _calendar.date().minute);
}

void MdMCharacter::updateCamera(float elapsedtime)
{
    const float speed = 10.0f;

    if(stage().synchronousKeyboard().isAsciiPressed('w'))
    {
        _camMan.forward(elapsedtime * speed);
    }
    if(stage().synchronousKeyboard().isAsciiPressed('s'))
    {
        _camMan.forward(-elapsedtime * speed);
    }
    if(stage().synchronousKeyboard().isAsciiPressed('a'))
    {
        _camMan.sideward(-elapsedtime * speed);
    }
    if(stage().synchronousKeyboard().isAsciiPressed('d'))
    {
        _camMan.sideward(elapsedtime * speed);
    }

    if(stage().synchronousMouse().displacement() != Vec2i(0, 0) &&
       stage().synchronousMouse().buttonIsPressed(Mouse::LEFT))
    {
        _camMan.turnHorizontaly(stage().synchronousMouse().displacement().x() * 0.004f);
        _camMan.turnVerticaly(stage().synchronousMouse().displacement().y() * 0.004f);
    }
}

void MdMCharacter::endStep(const StageTime &)
{
}

void MdMCharacter::draw(const scaena::StageTime &time)
{
    _drawCityAlgorithm.draw();

    _dateText.draw();
    _fpsText.setText( toString(1.0f / time.elapsedTime()) );
    _fpsText.draw();
}

void MdMCharacter::exitStage()
{
    _calendar.stop();
}

void MdMCharacter::notify(cellar::CameraMsg &msg)
{
    _dateText.setPosition(10, stage().height() - 20);

    if(msg.change == CameraMsg::PROJECTION)
    {
        _drawCityAlgorithm.updateProjectionMatrix( msg.camera.projectionMatrix() );
    }
    else if(msg.change == CameraMsg::VIEW)
    {
        _drawCityAlgorithm.updateModelViewMatrix( msg.camera.viewMatrix() );
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
    _cityMap->ground().setMinHeight(-3.5f);
    _cityMap->ground().setMaxHeight( 4.0f);
    _cityMap->ground().setWaterHeight(0.0f);

    // Height algorithm
    HeightByNoiseAlgo heightAlgo;
    heightAlgo.setNbNoises( cellar::min(_cityMap->size().x(), _cityMap->size().y()) / 2 );
    heightAlgo.setWeightedNoisesRange(1, heightAlgo.nbNoises());
    heightAlgo.setup( *_cityMap );

    MapElementsDepthFirst mapElemAlgo;
    mapElemAlgo.setup(*_cityMap);

    _drawCityAlgorithm.setup();
}
