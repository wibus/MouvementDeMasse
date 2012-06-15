#ifndef SKYCOMPONENT_H
#define SKYCOMPONENT_H

struct DrawCityCommonData;


class SkyComponent
{
public:
    SkyComponent(DrawCityCommonData& common);

    void setup();
    void draw();
    void update();

    void updateProjectionMatrix();
    void updateModelViewMatrix();

protected:
    DrawCityCommonData& _common;
    const double _skyCoefCorrection;
};

#endif // SKYCOMPONENT_H
