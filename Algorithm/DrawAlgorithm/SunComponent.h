#ifndef SUNCOMPONENT_H
#define SUNCOMPONENT_H

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
    unsigned int _sunVao;
    const int _sunNbElems;
};

#endif // SUNCOMPONENT_H
