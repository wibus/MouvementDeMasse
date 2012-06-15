#ifndef SUNCOMPONENT_H
#define SUNCOMPONENT_H

#include "Graphics/GL/GLShaderProgram.h"

struct DrawCityCommonData;


class SunComponent
{
public:
    SunComponent(DrawCityCommonData& common);

    void setup();
    void draw();
    void update();

    void updateProjectionMatrix();
    void updateModelViewMatrix();

protected:
    DrawCityCommonData& _common;
    cellar::GLShaderProgram _sunShader;
    unsigned int _sunVao;
    const int _sunNbElems;
};

#endif // SUNCOMPONENT_H
