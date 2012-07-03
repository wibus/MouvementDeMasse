#ifndef JUNCTIONSCOMPONENT_H
#define JUNCTIONSCOMPONENT_H

#include "AbstractComponent.h"


class JunctionsComponent : public AbstractComponent
{
public:
    JunctionsComponent(DrawCityCommonData& common);
    ~JunctionsComponent();

    void setup();
    void draw();

protected:
    unsigned int _junctionsVao;
    unsigned int _junctionsTex;
    int          _junctionsNbElems;
};

#endif // JUNCTIONSCOMPONENT_H
