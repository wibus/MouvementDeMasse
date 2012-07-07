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
    unsigned int _streetsVao;
    unsigned int _streetsTex;
    int          _streetsNbElems;
};

#endif // STREETSCOMPONENT_H
