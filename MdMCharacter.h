#ifndef MDMCHARACTER_H
#define MDMCHARACTER_H

#include <memory>

#include <CellarWorkbench/Camera/Camera.h>
#include <CellarWorkbench/Camera/CameraManFree.h>
#include <CellarWorkbench/DateAndTime/Calendar.h>
#include <CellarWorkbench/DesignPattern/SpecificObserver.h>

#include <PropRoom2D/Prop/Hud/TextHud.h>

#include <Scaena/Play/Character.h>


class City;
class DrawCityModule;
class HeightsAlgorithm;
class MapElementsAlgorithm;
class CitizensAlgorithm;
class SaveAndLoadAlgorithm;


class MdMCharacter : public scaena::Character,
                     public cellar::SpecificObserver<cellar::CameraMsg>
{
public:
    MdMCharacter();
    ~MdMCharacter();

    // Character interface
    virtual void enterStage() override;
    virtual void beginStep(const scaena::StageTime &time) override;
    virtual void endStep(const scaena::StageTime &time) override;
    virtual void draw(const std::shared_ptr<scaena::View> &view,
                      const scaena::StageTime &time) override;
    virtual void exitStage() override;

    // Specific observer interface
    virtual void notify(cellar::CameraMsg &msg);

    // City manip
    City& city();
    void setCity(City* city);
    bool saveCity(const std::string& fileName);
    bool loadCity(const std::string& fileName);

private:
    void setupAlgorithms();
    void updateCamera(float elapsedtime);

    City* _city;
    DrawCityModule* _drawCityModule;
    HeightsAlgorithm*     _heightsAlgo;
    MapElementsAlgorithm* _mapElemAlgo;
    CitizensAlgorithm*    _citizensDistribAlgo;
    CitizensAlgorithm*    _citizenMoveAlgo;

    std::shared_ptr<cellar::CameraManFree> _camMan;
    std::shared_ptr<prop2::TextHud> _dateText;
    std::shared_ptr<prop2::TextHud> _fpsText;
    std::shared_ptr<prop2::TextHud> _upsText;


    // Deleted methods //
private:
    MdMCharacter(const MdMCharacter&);
    MdMCharacter& operator= (const MdMCharacter&);
};

#endif // MDMCHARACTER_H
