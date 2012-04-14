#ifndef MDMPLAY_H
#define MDMPLAY_H

#include <Plays/SingleActPlay.h>


class MdMPlay : public scaena::SingleActPlay
{
public:
    MdMPlay();

    virtual void loadExternalRessources();
    virtual void setUpPersistentCharacters();
};

#endif // MDMPLAY_H
