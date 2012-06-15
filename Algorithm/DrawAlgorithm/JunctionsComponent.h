#ifndef JUNCTIONSCOMPONENT_H
#define JUNCTIONSCOMPONENT_H

#include <Graphics/GL/GLShaderProgram.h>

struct DrawCityCommonData;


class JunctionsComponent
{
public:
    JunctionsComponent(DrawCityCommonData& common);

    void setup();
    void draw();
    void update();

    void updateProjectionMatrix();
    void updateModelViewMatrix();

protected:
    float junctionHeight(int x, int y);

    DrawCityCommonData& _common;
    cellar::GLShaderProgram _junctionsShader;
    unsigned int _junctionsVao;
    unsigned int _junctionsTex;
    int          _junctionsNbElems;
};

#endif // JUNCTIONSCOMPONENT_H
