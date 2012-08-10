#ifndef SKYCOMPONENT_H
#define SKYCOMPONENT_H

#include <vector>

#include "AbstractComponent.h"


class SkyComponent : public AbstractComponent
{
public:
    SkyComponent(City& city, cellar::GLShaderProgram& shader);
    ~SkyComponent();

    void setup();
    void draw();

protected:
    void setupSky();
    void setupSkyTop();

    static const int _SKY_NB_BUFFS = 1;
    unsigned int _skyBuffs[_SKY_NB_BUFFS];
    unsigned int _skyVao;
    unsigned int _skyNbElems;
    unsigned int _cloudsTex;
    unsigned int _daySkyTex;
    unsigned int _nightSkyTex;
    std::vector<unsigned int> _skyIndices;
};

#endif // SKYCOMPONENT_H
