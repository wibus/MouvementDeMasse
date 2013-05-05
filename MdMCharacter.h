#ifndef MDMCHARACTER_H
#define MDMCHARACTER_H

#include <memory>

#include <DateAndTime/Calendar.h>
#include <DesignPattern/SpecificObserver.h>
#include <Camera/Camera.h>
#include <Camera/CameraManFree.h>
#include <Character/AbstractCharacter.h>

#include <Hud/TextHud.h>

class City;
class DrawCityModule;
class HeightsAlgorithm;
class MapElementsAlgorithm;
class CitizensAlgorithm;
class SaveAndLoadAlgorithm;


class MdMCharacter : public scaena::AbstractCharacter,
                     public cellar::SpecificObserver<media::CameraMsg>
{
public:
    MdMCharacter(scaena::AbstractStage& stage);
    ~MdMCharacter();

    // Character interface
    virtual void enterStage();
    virtual void beginStep(const scaena::StageTime &time);
    virtual void endStep(const scaena::StageTime &time);
    virtual void draw(const scaena::StageTime &time);
    virtual void exitStage();

    // Specific observer interface
    virtual void notify(media::CameraMsg &msg);

    // City manip
    City& city();
    void setCity(City* city);
    bool saveCity(const std::string& fileName);
    bool loadCity(const std::string& fileName);

private:
    void setAlgorithms();
    void updateCamera(float elapsedtime);

    City* _city;
    DrawCityModule* _drawCityModule;
    HeightsAlgorithm*     _heightsAlgo;
    MapElementsAlgorithm* _mapElemAlgo;
    CitizensAlgorithm*    _citizensDistribAlgo;
    CitizensAlgorithm*    _citizenMoveAlgo;

    media::CameraManFree _camMan;
    std::shared_ptr<prop2::TextHud> _dateText;
    std::shared_ptr<prop2::TextHud> _fpsText;
    std::shared_ptr<prop2::TextHud> _upsText;


    // Deleted methods //
private:
    MdMCharacter(const MdMCharacter&);
    MdMCharacter& operator= (const MdMCharacter&);
};

#endif // MDMCHARACTER_H
