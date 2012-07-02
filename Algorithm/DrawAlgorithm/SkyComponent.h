#ifndef SKYCOMPONENT_H
#define SKYCOMPONENT_H

#include "AbstractComponent.h"


class SkyComponent : public AbstractComponent
{
public:
    SkyComponent(DrawCityCommonData& common);

    void setup();
    void draw();

protected:
    void setupSky();
    void setupSkyTop();

    const double _skyCoefCorrection;
    float _skyRadius;
    unsigned int _skyVao;
    unsigned int _skyTopVao;
    unsigned int _daySkyTex;
    unsigned int _nightSkyTex;
    int _skyNbStacks;
    int _skyNbSlices;
    int _skyNbElems;
    int _skyTopNbElems;
};

#endif // SKYCOMPONENT_H
