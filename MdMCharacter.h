#ifndef MDMCHARACTER_H
#define MDMCHARACTER_H

#include <memory>

#include <DateAndTime/Calendar.h>
#include <Graphics/Camera/Camera.h>
#include <Graphics/Camera/CameraManFree.h>
#include <DesignPatterns/SpecificObserver.h>
#include <Text/2D/Text.h>
#include <Characters/AbstractCharacter.h>

class City;
class DrawCityModule;
class HeightsAlgorithm;
class MapElementsAlgorithm;
class CitizensAlgorithm;


class MdMCharacter : public scaena::AbstractCharacter,
                     public cellar::SpecificObserver<cellar::CameraMsg>
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
    virtual void notify(cellar::CameraMsg &msg);

    // City manip
    City& city();
    void setCity(City* city);

private:
    void setAlgorithms();
    void updateCamera(float elapsedtime);

    City* _city;
    DrawCityModule* _drawCityModule;
    HeightsAlgorithm*     _heightsAlgo;
    MapElementsAlgorithm* _mapElemAlgo;
    CitizensAlgorithm*    _citizensAlgo;

    cellar::CameraManFree _camMan;
    prop::twoD::Text _dateText;
    prop::twoD::Text _fpsText;
    prop::twoD::Text _upsText;


    // Deleted methods //
private:
    MdMCharacter(const MdMCharacter&);
    MdMCharacter& operator= (const MdMCharacter&);
};

#endif // MDMCHARACTER_H
