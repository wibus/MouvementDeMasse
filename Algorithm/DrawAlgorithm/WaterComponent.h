#ifndef WATERCOMPONENT_H
#define WATERCOMPONENT_H

#include "Graphics/GL/GLShaderProgram.h"

struct DrawCityCommonData;


class WaterComponent
{
public:
    WaterComponent(DrawCityCommonData& common);

    void setup();
    void draw();
    void update();

    void updateProjectionMatrix();
    void updateModelViewMatrix();

protected:
    DrawCityCommonData& _common;
    cellar::GLShaderProgram _waterShader;
    unsigned int _waterVao;
    const int _waterNbElems;
};

#endif // WATERCOMPONENT_H
