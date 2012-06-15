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
    Vec4f  nLightDir    = _common.sunLight.direction.normalized();
    double sunIntensity = nLightDir * Vec4f(0, 0, -1, 0);
    double nomalizedSunIntensity = cellar::max(sunIntensity + _skyCoefCorrection, 0.0);
    double skyColorCoef = pow(nomalizedSunIntensity, 0.75) / (_skyCoefCorrection + 1.0);

    _common.curSkyColor = _common.nightSkyColor * (1 - skyColorCoef) +
                          _common.daySkyColor   * skyColorCoef;
}

void SkyComponent::updateProjectionMatrix()
{
}

void SkyComponent::updateModelViewMatrix()
{
}
