#ifndef JUNCTIONSCOMPONENT_H
#define JUNCTIONSCOMPONENT_H

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
    DrawCityCommonData& _common;
    unsigned int _junctionsVao;
    unsigned int _junctionsTex;
    int          _junctionsNbElems;
};

#endif // JUNCTIONSCOMPONENT_H
