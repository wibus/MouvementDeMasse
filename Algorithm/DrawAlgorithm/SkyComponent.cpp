#include "SkyComponent.h"
#include "DrawCityModule.h"

#include <GL/glew.h>
#include <Misc/CellarUtils.h>
using namespace cellar;


SkyComponent::SkyComponent(DrawCityCommonData &common) :
    _common(common),
    _skyCoefCorrection(0.1)
{
}

void SkyComponent::setup()
{
}

void SkyComponent::draw()
{
    glClearColor(_common.curSkyColor.x(), _common.curSkyColor.y(),
                 _common.curSkyColor.z(), _common.curSkyColor.w());
}

void SkyComponent::update()
{
}

void SkyComponent::updateProjectionMatrix()
{
}

void SkyComponent::updateModelViewMatrix()
{
}
