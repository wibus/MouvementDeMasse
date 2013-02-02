#include <iostream>
using namespace std;

#include <Misc/CellarUtils.h>
#include <Misc/Log.h>
#include <DateAndTime/Timer.h>
using namespace cellar;

#include <PropTeam/AbstractPropTeam.h>
using namespace prop2;

#include <Stage/Event/StageTime.h>
#include <Stage/Event/SynchronousKeyboard.h>
#include <Stage/Event/SynchronousMouse.h>
#include <Stage/AbstractStage.h>
using namespace scaena;

#include "MdMCharacter.h"
#include "City/City.h"
#include "Rendering/DrawCityModule.h"
#include "Algorithm/CitizensAlgorithm/CitizensDistribByIsland.h"
#include "Algorithm/CitizensAlgorithm/CitizensMoveHomeWork.h"
#include "Algorithm/HeightsAlgorithm/HeightsByNoiseAlgo.h"
#include "Algorithm/MapElementsAlgorithm/MapElementsDepthFirst.h"
#include "Algorithm/MapElementsAlgorithm/MapElementsByIsland.h"

MdMCharacter::MdMCharacter(AbstractStage& stage) :
    AbstractCharacter(stage, "MdMCharacter"),
    _city( new City(64, 60) ),
    _drawCityModule( new DrawCityModule() ),
    _heightsAlgo(    new HeightByNoiseAlgo() ),
    _mapElemAlgo(    new MapElementsByIsland() ),
    _citizensDistribAlgo( new CitizensDistribByIsland() ),
    _citizenMoveAlgo(     new CitizensMoveHomeWork() ),
    _camMan( stage.camera() ),
    _dateText(),
    _fpsText(),
    _upsText()
{
    stage.camera().setTripod(Vec3f(_city->size().x() / 2, 0, _city->ground().maxHeight()),
                             Vec3f(_city->size().x() / 2, _city->size().y() / 2, 0),
                             Vec3f(0, 0 ,1));
    stage.camera().registerObserver( *this );
}

MdMCharacter::~MdMCharacter()
{
    delete _city;
    delete _drawCityModule;
    delete _citizensDistribAlgo;
    delete _heightsAlgo;
    delete _mapElemAlgo;
}

void MdMCharacter::enterStage()
{
    setAlgorithms();
    stage().camera().refresh();

    _dateText = stage().propTeam().createTextHud();
    _dateText->setHandlePosition(Vec2r(10, -30));
    _dateText->setHorizontalAnchor(HorizontalAnchor::LEFT);
    _dateText->setVerticalAnchor(VerticalAnchor::TOP);

    _fpsText = stage().propTeam().createTextHud();
    _fpsText->setHandlePosition(Vec2r(5, 5));
    _fpsText->setHorizontalAnchor(HorizontalAnchor::LEFT);
    _fpsText->setVerticalAnchor(VerticalAnchor::BOTTOM);

    _upsText = stage().propTeam().createTextHud();
    _upsText->setHandlePosition(Vec2r(5, 25));
    _upsText->setHorizontalAnchor(HorizontalAnchor::LEFT);
    _upsText->setVerticalAnchor(VerticalAnchor::BOTTOM);
}

void MdMCharacter::beginStep(const StageTime &time)
{
    updateCamera( time.elapsedTime() );

    _city->update();
    _citizensDistribAlgo->update();
    _citizenMoveAlgo->update();
    _drawCityModule->update();

    _dateText->setText(_city->dateAndTime().toString());
    _upsText->setText( string("UPS : ") + toString(ceil(1.0f / time.elapsedTime())) );
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
    _drawCityModule->draw();

    _fpsText->setText( string("FPS : ") + toString(ceil(1.0f / time.elapsedTime())) );
}

void MdMCharacter::exitStage()
{
    stage().propTeam().deleteTextHud(_dateText);
    stage().propTeam().deleteTextHud(_fpsText);
    stage().propTeam().deleteTextHud(_upsText);
}

void MdMCharacter::notify(cellar::CameraMsg &msg)
{
    if(msg.change == CameraMsg::PROJECTION)
    {
        _drawCityModule->updateProjectionMatrix( msg.camera.projectionMatrix() );
    }
    else if(msg.change == CameraMsg::VIEW)
    {
        _drawCityModule->updateModelViewMatrix( msg.camera.viewMatrix() );
    }
}

City& MdMCharacter::city()
{
    return *_city;
}

void MdMCharacter::setCity(City* city)
{
    delete _city;
    _city = city;
}

bool MdMCharacter::saveCity(const std::string& fileName)
{
    return _city->save( fileName );
}

bool MdMCharacter::loadCity(const std::string& fileName)
{
    return _city->load( fileName );
}

void MdMCharacter::setAlgorithms()
{
    Timer timer;
    timer.start();
    getLog().postMessage(new Message('I', false, "Start city algorithms", "MdMCharacter"));

    _city->reset();
    _city->ground().setMinHeight(-3.5f);
    _city->ground().setMaxHeight( 4.0f);
    _city->ground().setWaterHeight(0.0f);

    // Height algorithm
    _heightsAlgo->setup( *_city );
    getLog().postMessage(new Message('I', false, toString(timer.counter()) + " -> Height algo finished.", "MdMCharacter"));

    // Map elements
    _mapElemAlgo->setup( *_city );
    getLog().postMessage(new Message('I', false, toString(timer.counter()) + " -> Map Elements algo finished.", "MdMCharacter"));

    // Citizens
    _citizensDistribAlgo->setup( *_city );
    _citizenMoveAlgo->setup( *_city );
    getLog().postMessage(new Message('I', false, toString(timer.counter()) + " -> Citizens algo finished.", "MdMCharacter"));

    // Rendering
    _drawCityModule->setup( *_city );
    getLog().postMessage(new Message('I', false, toString(timer.counter()) + " -> Draw module finished.", "MdMCharacter"));
}
