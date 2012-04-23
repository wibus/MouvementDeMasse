#include "MdMPlay.h"
#include "MdMCharacter.h"

using namespace std;
using namespace scaena;


MdMPlay::MdMPlay() :
    SingleActPlay("MdMPlay")
{
}

void MdMPlay::loadExternalRessources()
{
}

void MdMPlay::setUpPersistentCharacters()
{
    addPersistentCharacter(
        shared_ptr<AbstractCharacter>(new MdMCharacter( stage() ))
    );
}
