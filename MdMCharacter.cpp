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
#include "Algorithm/MapElementsAlgorithm/MapElementsDepthFirst.h"


MdMCharacter::MdMCharacter(AbstractStage& stage) :
    AbstractCharacter(stage, "MdMCharacter"),
    _sun(Vec4f(-1, -1, 2, 0), Vec3f(-1.0, -1.0, -0.5)),
    _cityMap( new CityMap(100, 100, Vec2f(-20.0, 20.0))),
    _drawAlgorithm(),
    _camMan( stage.camera() ),
    _fpsText()
{
    _fpsText.setPosition(5, 5);

    stage.camera().setTripod(Vec3f(_cityMap->size().x() / 2, 0, _cityMap->heightsRange()[1]),
                             Vec3f(_cityMap->size().x() / 2, _cityMap->size().y() / 2, 0),
                             Vec3f(0, 0 ,1));
    stage.camera().registerObserver( *this );

    _calendar.setDate(Calendar::Date(2000, Calendar::Date::JANUARY, 1, 0, 0));
}

void MdMCharacter::enterStage()
{
    setAlgorithms();
    stage().camera().refresh();

    _calendar.setClock(Calendar::Clock(Calendar::Clock::MINUTE));
    _calendar.start();
    updateCalendar();
}

void MdMCharacter::beginStep(const StageTime &time)
{
    updateCalendar();
    updateCamera( time.elapsedTime() );
}

void MdMCharacter::updateCalendar()
{
    _calendar.tic();

    _dateText.setPosition(10, stage().height() - 20);
    _dateText.setText(_calendar.date().toString(true, true));

    _sun.setTime(_calendar.date().hour, _calendar.date().minute);
    _drawAlgorithm.updateSunDirection( _sun.direction() );
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
    //_cityMap->drawAlgorithm().draw();
    _drawAlgorithm.draw();
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
    if(msg.change == CameraMsg::PROJECTION)
    {
        _drawAlgorithm.updateProjectionMatrix( msg.camera.projectionMatrix() );
    }
    else if(msg.change == CameraMsg::VIEW)
    {
        _drawAlgorithm.updateViewMatrix( msg.camera.viewMatrix() );
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
    HeightByNoiseAlgo heightAlgo;
    heightAlgo.setNbNoises( cellar::min(_cityMap->size().x(), _cityMap->size().y()) / 2 );
    heightAlgo.setWeightedNoisesRange(1, heightAlgo.nbNoises());
    heightAlgo.setup( *_cityMap );



    MapElementsDepthFirst mapElemAlgo;
    mapElemAlgo.setup(*_cityMap);
    /*
    // Draw algorithm
    Vec2ui pos(_cityMap->size() / 2);
    Vec2ui next;
    Vec2i  dir(0, 0);
    while(pos.x() != 1 &&
          pos.x() != _cityMap->size().x()-1 &&
          pos.y() != 1 &&
          pos.y() != _cityMap->size().y()-1 )
    {
        _cityMap->junctions().get( pos )->setType(Junction::ASPHALT);

        dir(0, 0);
        if(rand() % 2)
        {
            if(rand() % 2)
                dir.setX(-1);
            else
                dir.setX(1);
        }
        else
        {
            if(rand() % 2)
                dir.setY(-1);
            else
                dir.setY(1);
        }

        next( pos.x() + dir.x(), pos.y() + dir.y());
        shared_ptr<Street> street( new Street(pos, next) );

        if(dir == Vec2i(-1, 0))
        {
            _cityMap->junctions().get(pos)->attach(street, WEST);
            _cityMap->junctions().get(next)->attach(street, EAST);
        }
        else if(dir == Vec2i(1, 0))
        {
            _cityMap->junctions().get(pos)->attach(street, EAST);
            _cityMap->junctions().get(next)->attach(street, WEST);
        }
        else if(dir == Vec2i(0, -1))
        {
            _cityMap->junctions().get(pos)->attach(street, SOUTH);
            _cityMap->junctions().get(next)->attach(street, NORTH);
        }
        else if(dir == Vec2i(0, 1))
        {
            _cityMap->junctions().get(pos)->attach(street, NORTH);
            _cityMap->junctions().get(next)->attach(street, SOUTH);
        }


        pos = next;
    }*/

    _drawAlgorithm.setup( *_cityMap );

    _cityMap->setup();
}
