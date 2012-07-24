#ifndef STREETSCOMPONENT_H
#define STREETSCOMPONENT_H

#include "AbstractComponent.h"


class StreetsComponent : public AbstractComponent
{
public:
    StreetsComponent(City& city, cellar::GLShaderProgram& shader);
    ~StreetsComponent();

    void setup();
    void draw();

protected:
    static const int _STREETS_NB_BUFFS = 3;
    unsigned int _streetsBuffs[_STREETS_NB_BUFFS];
    unsigned int _streetsVao;
    unsigned int _streetsTex;
    int          _streetsNbElems;
};

#endif // STREETSCOMPONENT_H
