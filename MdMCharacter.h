#ifndef MDMCHARACTER_H
#define MDMCHARACTER_H

#include <memory>

#include <DateAndTime/Calendar.h>
#include <Graphics/Camera/Camera.h>
#include <Graphics/Camera/CameraManFree.h>
#include <DesignPatterns/SpecificObserver.h>

#include <Text/2D/Text.h>

#include <Characters/AbstractCharacter.h>

#include "Algorithm/DrawAlgorithm/DrawCityModule.h"
#include "Algorithm/DrawAlgorithm/DrawHighPrecisionGround.h"

class CityMap;


class MdMCharacter : public scaena::AbstractCharacter,
                     public cellar::SpecificObserver<cellar::CameraMsg>
{
public:
    MdMCharacter(scaena::AbstractStage& stage);

    // Character interface
    virtual void enterStage();
    virtual void beginStep(const scaena::StageTime &time);
    virtual void endStep(const scaena::StageTime &time);
    virtual void draw(const scaena::StageTime &time);
    virtual void exitStage();

    // Specific observer interface
    virtual void notify(cellar::CameraMsg &msg);

    // CityMap manip
    CityMap& cityMap();
    void setCityMap(CityMap* map);

private:
    void setAlgorithms();
    void updateCamera(float elapsedtime);

    std::shared_ptr<CityMap> _cityMap;
    DrawCityModule _drawCityAlgorithm;
    cellar::CameraManFree _camMan;
    prop::twoD::Text _dateText;
    prop::twoD::Text _fpsText;
    prop::twoD::Text _upsText;
};

#endif // MDMCHARACTER_H
