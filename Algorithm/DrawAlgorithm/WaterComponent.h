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
    unsigned int _waterVao;
    const int _waterNbElems;
};

#endif // WATERCOMPONENT_H

