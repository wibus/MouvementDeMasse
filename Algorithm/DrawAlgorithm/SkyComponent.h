#ifndef SKYCOMPONENT_H
#define SKYCOMPONENT_H

#include <vector>

#include "AbstractComponent.h"


class SkyComponent : public AbstractComponent
{
public:
    SkyComponent(DrawCityCommonData& common);
    ~SkyComponent();

    void setup();
    void draw();

protected:
    void setupSky();
    void setupSkyTop();

    unsigned int _skyVao;
    unsigned int _cloudsTex;
    unsigned int _daySkyTex;
    unsigned int _nightSkyTex;
    int _skyNbStacks;
    int _skyNbSlices;
    std::vector<unsigned int> _skyIndices;
};

#endif // SKYCOMPONENT_H
