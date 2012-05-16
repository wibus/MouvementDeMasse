#ifndef MDMCHARACTER_H
#define MDMCHARACTER_H

#include <memory>

#include <Graphics/Camera/CameraManFree.h>

#include <Text/2D/Text.h>

#include <Characters/AbstractCharacter.h>

class CityMap;


class MdMCharacter : public scaena::AbstractCharacter
{
public:
    MdMCharacter(scaena::AbstractStage& stage);

    // Character interface
    virtual void enterStage();
    virtual void beginStep(const scaena::StageTime &time);
    virtual void endStep(const scaena::StageTime &time);
    virtual void draw(const scaena::StageTime &time);
    virtual void exitStage();

    // CityMap manip
    CityMap& cityMap();
    void setCityMap(CityMap* map);

private:
    void setAlgorithms();

    std::shared_ptr<CityMap> _cityMap;
    prop::twoD::Text _fps;
    cellar::CameraManFree _camMan;
};

#endif // MDMCHARACTER_H
