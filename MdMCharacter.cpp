#include "MdMCharacter.h"

#include <iostream>

#include <GLM/gtc/matrix_transform.hpp>

#include <CellarWorkbench/DateAndTime/Clock.h>
#include <CellarWorkbench/Misc/StringUtils.h>
#include <CellarWorkbench/Misc/Log.h>

#include <PropRoom2D/PropTeam/AbstractPropTeam.h>

#include <Scaena/Stage/Event/StageTime.h>
#include <Scaena/Stage/Event/SynchronousKeyboard.h>
#include <Scaena/Stage/Event/SynchronousMouse.h>
#include <Scaena/Stage/AbstractStage.h>

#include "City/City.h"
#include "Rendering/DrawCityModule.h"
#include "Algorithm/CitizensAlgorithm/CitizensDistribByIsland.h"
#include "Algorithm/CitizensAlgorithm/CitizensMoveHomeWork.h"
#include "Algorithm/HeightsAlgorithm/HeightsByNoiseAlgo.h"
#include "Algorithm/MapElementsAlgorithm/MapElementsDepthFirst.h"
#include "Algorithm/MapElementsAlgorithm/MapElementsByIsland.h"

using namespace std;
using namespace cellar;
using namespace prop2;
using namespace scaena;


MdMCharacter::MdMCharacter(AbstractStage& stage) :
    AbstractCharacter(stage, "MdMCharacter"),
    _city( new City(70, 63) ),
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
    _camMan.setPosition(glm::vec3(_city->size().x / 2.0f, 0.0f, _city->ground().maxHeight()));
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
    setupAlgorithms();
    stage().camera().refresh();

    _dateText = stage().propTeam2D().createTextHud();
    _dateText->setHandlePosition(glm::dvec2(10, -30));
    _dateText->setHorizontalAnchor(EHorizontalAnchor::LEFT);
    _dateText->setVerticalAnchor(EVerticalAnchor::TOP);
    _dateText->setHeight(16);

    _fpsText = stage().propTeam2D().createTextHud();
    _fpsText->setHandlePosition(glm::dvec2(5, 5));
    _fpsText->setHorizontalAnchor(EHorizontalAnchor::LEFT);
    _fpsText->setVerticalAnchor(EVerticalAnchor::BOTTOM);
    _fpsText->setHeight(16);

    _upsText = stage().propTeam2D().createTextHud();
    _upsText->setHandlePosition(glm::dvec2(5, 25));
    _upsText->setHorizontalAnchor(EHorizontalAnchor::LEFT);
    _upsText->setVerticalAnchor(EVerticalAnchor::BOTTOM);
    _upsText->setHeight(16);
}

void MdMCharacter::beginStep(const StageTime &time)
{
    updateCamera( time.elapsedTime() );

    _city->update();
    _citizensDistribAlgo->update();
    _citizenMoveAlgo->update();
    _drawCityModule->update();

    _dateText->setText(_city->dateAndTime().toString());
    _upsText->setText( string("UPS : ") + toString( floor(time.framesPerSecond()) ) );
}

void MdMCharacter::updateCamera(float elapsedtime)
{
    float velocity  = 2.0f * elapsedtime;
    float turnSpeed = 0.004f;

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

    if(stage().synchronousMouse().displacement() != glm::ivec2(0, 0) &&
       stage().synchronousMouse().buttonIsPressed(EMouseButton::LEFT))
    {
        _camMan.pan( stage().synchronousMouse().displacement().x * -turnSpeed);
        _camMan.tilt(stage().synchronousMouse().displacement().y * -turnSpeed);
    }
}

void MdMCharacter::endStep(const StageTime &)
{
}

void MdMCharacter::draw(const scaena::StageTime &time)
{
    _drawCityModule->draw();

    _fpsText->setText( string("FPS : ") + toString( floor(time.framesPerSecond()) ) );
}

void MdMCharacter::exitStage()
{
    stage().propTeam2D().deleteTextHud(_dateText);
    stage().propTeam2D().deleteTextHud(_fpsText);
    stage().propTeam2D().deleteTextHud(_upsText);
}

void MdMCharacter::notify(CameraMsg &msg)
{
    if(msg.change == CameraMsg::EChange::PROJECTION)
    {
        _drawCityModule->updateProjectionMatrix( msg.camera.projectionMatrix() );
    }
    else if(msg.change == CameraMsg::EChange::VIEW)
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

void MdMCharacter::setupAlgorithms()
{
    Clock timer;
    timer.start();
    getLog().postMessage(new Message('I', false, "Start city algorithms", "MdMCharacter"));

    _city->reset();
    _city->ground().setMinHeight(-3.5f);
    _city->ground().setMaxHeight( 4.0f);
    _city->ground().setWaterHeight(0.0f);

    // Height algorithm
    _heightsAlgo->setup( *_city );
    timer.tick();
    getLog().postMessage(new Message('I', false, toString(timer.totalSeconds()) + " -> Height algo finished.", "MdMCharacter"));

    // Map elements
    _mapElemAlgo->setup( *_city );
    timer.tick();
    getLog().postMessage(new Message('I', false, toString(timer.totalSeconds()) + " -> Map Elements algo finished.", "MdMCharacter"));

    // Citizens
    _citizensDistribAlgo->setup( *_city );
    _citizenMoveAlgo->setup( *_city );
    timer.tick();
    getLog().postMessage(new Message('I', false, toString(timer.totalSeconds()) + " -> Citizens algo finished.", "MdMCharacter"));

    // Rendering
    _drawCityModule->setup( *_city );
    timer.tick();
    getLog().postMessage(new Message('I', false, toString(timer.totalSeconds()) + " -> Draw module finished.", "MdMCharacter"));
}
