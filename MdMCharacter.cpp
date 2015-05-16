#include "MdMCharacter.h"

#include <iostream>

#include <GLM/gtc/matrix_transform.hpp>

#include <CellarWorkbench/DateAndTime/Clock.h>
#include <CellarWorkbench/Misc/StringUtils.h>
#include <CellarWorkbench/Camera/Camera.h>
#include <CellarWorkbench/Misc/Log.h>

#include <PropRoom2D/Team/AbstractTeam.h>

#include <Scaena/Play/Play.h>
#include <Scaena/Play/View.h>
#include <Scaena/StageManagement/Event/StageTime.h>
#include <Scaena/StageManagement/Event/SynchronousKeyboard.h>
#include <Scaena/StageManagement/Event/SynchronousMouse.h>

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


MdMCharacter::MdMCharacter() :
    Character("MdMCharacter"),
    _dateText(),
    _fpsText(),
    _upsText()
{
}

MdMCharacter::~MdMCharacter()
{
}

void MdMCharacter::enterStage()
{
    _city = new City(70, 63);
    _drawCityModule = new DrawCityModule();
    _heightsAlgo = new HeightByNoiseAlgo();
    _mapElemAlgo = new MapElementsByIsland();
    _citizensDistribAlgo = new CitizensDistribByIsland();
    _citizenMoveAlgo = new CitizensMoveHomeWork();
    setupAlgorithms();

    _dateText = play().propTeam2D()->createTextHud();
    _dateText->setHandlePosition(glm::dvec2(10, -30));
    _dateText->setHorizontalAnchor(EHorizontalAnchor::LEFT);
    _dateText->setVerticalAnchor(EVerticalAnchor::TOP);
    _dateText->setHeight(16);

    _fpsText = play().propTeam2D()->createTextHud();
    _fpsText->setHandlePosition(glm::dvec2(5, 5));
    _fpsText->setHorizontalAnchor(EHorizontalAnchor::LEFT);
    _fpsText->setVerticalAnchor(EVerticalAnchor::BOTTOM);
    _fpsText->setHeight(16);

    _upsText = play().propTeam2D()->createTextHud();
    _upsText->setHandlePosition(glm::dvec2(5, 25));
    _upsText->setHorizontalAnchor(EHorizontalAnchor::LEFT);
    _upsText->setVerticalAnchor(EVerticalAnchor::BOTTOM);
    _upsText->setHeight(16);

    std::shared_ptr<Camera> camera = play().view()->camera3D();
    _camMan.reset(new CameraManFree(camera));
    camera->registerObserver( *this );
    _camMan->setPosition(glm::vec3(_city->size().x / 2.0f, 0.0f,
                                  _city->ground().maxHeight()));
    camera->refresh();
}

void MdMCharacter::beginStep(const StageTime &time)
{
    _city->update();
    _citizensDistribAlgo->update();
    _citizenMoveAlgo->update();

    _dateText->setText(_city->dateAndTime().toString());
    _upsText->setText( string("UPS : ") + toString( floor(time.framesPerSecond()) ) );
}

void MdMCharacter::endStep(const StageTime &)
{
}

void MdMCharacter::draw(const std::shared_ptr<scaena::View> &,
                        const scaena::StageTime &time)
{
    updateCamera( time.elapsedTime() );

    _drawCityModule->update();
    _drawCityModule->draw();

    _fpsText->setText( string("FPS : ") + toString( floor(time.framesPerSecond()) ) );
}

void MdMCharacter::exitStage()
{
    play().propTeam2D()->deleteTextHud(_dateText);
    play().propTeam2D()->deleteTextHud(_fpsText);
    play().propTeam2D()->deleteTextHud(_upsText);

    delete _city;
    delete _drawCityModule;
    delete _citizensDistribAlgo;
    delete _heightsAlgo;
    delete _mapElemAlgo;

    _city = nullptr;
    _drawCityModule = nullptr;
    _citizensDistribAlgo = nullptr;
    _heightsAlgo = nullptr;
    _mapElemAlgo = nullptr;
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
    getLog().postMessage(new Message('I', false,
        "Start city algorithms", "MdMCharacter"));

    _city->reset();
    _city->ground().setMinHeight(-3.5f);
    _city->ground().setMaxHeight( 4.0f);
    _city->ground().setWaterHeight(0.0f);

    // Height algorithm
    _heightsAlgo->setup( *_city );
    timer.tick();
    getLog().postMessage(new Message('I', false,
        toString(timer.totalSeconds()) + " -> Height algo finished.",
        "MdMCharacter"));

    // Map elements
    _mapElemAlgo->setup( *_city );
    timer.tick();
    getLog().postMessage(new Message('I', false,
        toString(timer.totalSeconds()) + " -> Map Elements algo finished.",
        "MdMCharacter"));

    // Citizens
    _citizensDistribAlgo->setup( *_city );
    _citizenMoveAlgo->setup( *_city );
    timer.tick();
    getLog().postMessage(new Message('I', false,
        toString(timer.totalSeconds()) + " -> Citizens algo finished.",
        "MdMCharacter"));

    // Rendering
    _drawCityModule->setup( *_city );
    timer.tick();
    getLog().postMessage(new Message('I', false,
        toString(timer.totalSeconds()) + " -> Draw module finished.",
        "MdMCharacter"));
}

void MdMCharacter::updateCamera(float elapsedtime)
{
    float velocity  = 2.0f * elapsedtime;
    float turnSpeed = 0.004f;

    SynchronousMouse& syncMouse = *play().synchronousMouse();
    SynchronousKeyboard& syncKeyboard = *play().synchronousKeyboard();

    if(syncKeyboard.isAsciiPressed('w'))
    {
        _camMan->forward(velocity);
    }
    if(syncKeyboard.isAsciiPressed('s'))
    {
        _camMan->forward(-velocity);
    }
    if(syncKeyboard.isAsciiPressed('a'))
    {
        _camMan->sideward(-velocity);
    }
    if(syncKeyboard.isAsciiPressed('d'))
    {
        _camMan->sideward(velocity);
    }

    if(syncMouse.displacement() != glm::ivec2(0, 0) &&
       syncMouse.buttonIsPressed(EMouseButton::LEFT))
    {
        _camMan->pan( syncMouse.displacement().x * -turnSpeed);
        _camMan->tilt(syncMouse.displacement().y * -turnSpeed);
    }
}
