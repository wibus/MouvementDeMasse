#ifndef JUNCTIONSCOMPONENT_H
#define JUNCTIONSCOMPONENT_H

#include "AbstractComponent.h"


class JunctionsComponent : public AbstractComponent
{
public:
    JunctionsComponent(City& city, media::GlProgram& shader);
    ~JunctionsComponent();

    void setup();
    void draw();

protected:
    static const int _JUNCTIONS_NB_BUFFS = 2;
    unsigned int _junctionsBuffs[_JUNCTIONS_NB_BUFFS];
    unsigned int _junctionsVao;
    unsigned int _junctionsTex;
    int          _junctionsNbElems;
};

#endif // JUNCTIONSCOMPONENT_H
