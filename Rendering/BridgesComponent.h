#ifndef BRIDGESCOMPONENT_H
#define BRIDGESCOMPONENT_H

#include <vector>
#include <MathsAndPhysics/Vector.h>

#include "AbstractComponent.h"


class BridgesComponent : public AbstractComponent
{
public:
    BridgesComponent(City& city, cellar::GLShaderProgram& shader);
    ~BridgesComponent();

    void setup();
    void draw();

protected:
    unsigned int _bridgeVao;
    unsigned int _bridgeTex;
    int _bridgeNbElems;
};

#endif // BRIDGESCOMPONENT_H
