#include <iostream>
using namespace std;

#include "MdMCharacter.h"

using namespace scaena;


MdMCharacter::MdMCharacter(AbstractStage& stage) :
    AbstractCharacter(stage, "MdMCharacter")
{
}

void MdMCharacter::enterStage()
{
    cout << "I'm here" << endl;
}

void MdMCharacter::beginStep(const StageTime &time)
{
}

void MdMCharacter::endStep(const StageTime &time)
{
}

void MdMCharacter::exitStage()
{
    cout << "Goodbye" << endl;
}
