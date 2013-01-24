#ifndef BRIDGESCOMPONENT_H
#define BRIDGESCOMPONENT_H

#include <vector>
#include <DataStructure/Vector.h>

#include "AbstractComponent.h"


class BridgesComponent : public AbstractComponent
{
public:
    BridgesComponent(City& city, cellar::GlProgram& shader);
    ~BridgesComponent();

    void setup();
    void draw();

protected:
    static const int _BRIDGE_NB_BUFFS = 3;
    unsigned int _bridgeBuffs[_BRIDGE_NB_BUFFS];
    unsigned int _bridgeVao;    
    unsigned int _bridgeTex;
    int _bridgeNbElems;
};

#endif // BRIDGESCOMPONENT_H
