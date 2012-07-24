#ifndef WATERCOMPONENT_H
#define WATERCOMPONENT_H

#include "AbstractComponent.h"


class WaterComponent : public AbstractComponent
{
public:
    WaterComponent(City& city, cellar::GLShaderProgram& shader);
    ~WaterComponent();

    void setup();
    void draw();

protected:
    static const int _WATER_NB_BUFFS = 1;
    unsigned int _waterBuffs[_WATER_NB_BUFFS];
    unsigned int _waterVao;
    const int _WATER_NB_ELEMS;
};

#endif // WATERCOMPONENT_H

