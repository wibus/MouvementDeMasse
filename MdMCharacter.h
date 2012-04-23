#ifndef MDMCHARACTER_H
#define MDMCHARACTER_H

#include <Characters/AbstractCharacter.h>


class MdMCharacter : public scaena::AbstractCharacter
{
public:
    MdMCharacter(scaena::AbstractStage& stage);

    virtual void enterStage();
    virtual void beginStep(const scaena::StageTime &time);
    virtual void endStep(const scaena::StageTime &time);
    virtual void exitStage();
};

#endif // MDMCHARACTER_H
