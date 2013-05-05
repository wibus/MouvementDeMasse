#ifndef WATERCOMPONENT_H
#define WATERCOMPONENT_H

#include "AbstractComponent.h"


class WaterComponent : public AbstractComponent
{
public:
    WaterComponent(City& city, media::GlProgram& shader);
    ~WaterComponent();

    void setup();
    void draw();

protected:
    static const int _WATER_NB_BUFFS = 1;
    static const int _WATER_NB_ELEMS = 4;
    unsigned int _waterBuffs[_WATER_NB_BUFFS];
    unsigned int _waterVao;
};

#endif // WATERCOMPONENT_H

