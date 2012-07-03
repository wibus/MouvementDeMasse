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
#include "Algorithm/MapElementsAlgorithm/MapElementsByIsland.h"

MdMCharacter::MdMCharacter(AbstractStage& stage) :
    AbstractCharacter(stage, "MdMCharacter"),
    _cityMap( new CityMap(64, 48)),
    _drawCityAlgorithm(*_cityMap),
    _camMan( stage.camera() ),
    _fpsText(),
    _upsText()
{
    _fpsText.setPosition(5, 5);
    _upsText.setPosition(5, 25);

    stage.camera().setTripod(Vec3f(_cityMap->size().x() / 2, 0, _cityMap->ground().maxHeight()),
                             Vec3f(_cityMap->size().x() / 2, _cityMap->size().y() / 2, 0),
                             Vec3f(0, 0 ,1));
    stage.camera().registerObserver( *this );

    _cityMap->calendar().setClock(Calendar::Clock(Calendar::Clock::MINUTE));
    _cityMap->calendar().setDate(Calendar::Date(2000, Calendar::Date::JANUARY, 1, 8, 0, 0));
}

void MdMCharacter::enterStage()
{
    setAlgorithms();
    stage().camera().refresh();
    _cityMap->calendar().start();
}

void MdMCharacter::beginStep(const StageTime &time)
{
    updateCamera( time.elapsedTime() );

    _cityMap->update();
    _drawCityAlgorithm.update();

    _dateText.setText(_cityMap->calendar().date().toString(true, true));
    _upsText.setText( string("UPS : ") + toString(ceil(1.0f / time.elapsedTime())) );
}

void MdMCharacter::updateCamera(float elapsedtime)
{
    float velocity  = 10.0f * elapsedtime;
    float turnSpeed = 0.004;

    if(stage().synchronousKeyboard().isAsciiPressed('w'))
    {
        _camMan.forward(velocity);
    }
    if(stage().synchronousKeyboard().isAsciiPressed('s'))
    {
        _camMan.forward(-velocity);
    }
    if(stage().synchronousKeyboard().isAsciiPressed('a'))
    {
        _camMan.sideward(-velocity);
    }
    if(stage().synchronousKeyboard().isAsciiPressed('d'))
    {
        _camMan.sideward(velocity);
    }

    if(stage().synchronousMouse().displacement() != Vec2i(0, 0) &&
       stage().synchronousMouse().buttonIsPressed(Mouse::LEFT))
    {
        _camMan.turnHorizontaly(stage().synchronousMouse().displacement().x() * turnSpeed);
        _camMan.turnVerticaly(  stage().synchronousMouse().displacement().y() * turnSpeed);
    }
}

void MdMCharacter::endStep(const StageTime &)
{
}

void MdMCharacter::draw(const scaena::StageTime &time)
{
    _fpsText.setText( string("FPS : ") + toString(ceil(1.0f / time.elapsedTime())) );

    _drawCityAlgorithm.draw();    
    _fpsText.draw();
    _upsText.draw();
    _dateText.draw();
}

void MdMCharacter::exitStage()
{
    _cityMap->calendar().stop();
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
    heightAlgo.setWeightedFrequenciesRange(1,
        cellar::min(_cityMap->size().x(), _cityMap->size().y()) / 2 );
    heightAlgo.setup( *_cityMap );

    MapElementsDepthFirst mapElemAlgo;
    //MapElementsByIsland mapElemAlgo;
    mapElemAlgo.setup(*_cityMap);

    _drawCityAlgorithm.setup();
}
