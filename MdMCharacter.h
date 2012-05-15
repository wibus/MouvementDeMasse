#ifndef MDMCHARACTER_H
#define MDMCHARACTER_H

#include <memory>

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
    virtual void exitStage();

    // CityMap manip
    CityMap& cityMap();
    void setCityMap(CityMap* map);

private:
    void chooseAlgorithms();

    std::shared_ptr<CityMap> _cityMap;
};

#endif // MDMCHARACTER_H
