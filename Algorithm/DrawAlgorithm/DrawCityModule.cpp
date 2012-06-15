#include "DrawCityModule.h"

using namespace cellar;


DrawCityCommonData::DrawCityCommonData(CityMap& cityMap) :
    cityMap(cityMap),
    ground(cityMap.ground()),
    sunRadius(10.0f),
    sunLight(),
    sunColor(     1.00, 0.90, 0.20, 1.00),
    nightSkyColor(0.02, 0.01, 0.05, 1.00),
    daySkyColor(  0.40, 0.60, 0.80, 1.00),
    curSkyColor(  nightSkyColor         ),
    grassColor(   0.16, 0.94, 0.04, 1.00),
    waterColor(   0.03, 0.03, 0.30, 0.65),
    groundShininess(20.0f),
    waterShininess(160.0f),
    roadWidth(0.125f)
{
    sunLight.direction(0, 0, 0.0, 0.0).normalize();
    sunLight.ambient( 0.08, 0.09, 0.12);
    sunLight.diffuse( 0.62, 0.62, 0.60);
    sunLight.specular(0.65, 0.50, 0.30);
}

DrawCityModule::DrawCityModule(CityMap &cityMap) :
    _commonData(cityMap),
    _sunComponent(_commonData),
    _skyComponent(_commonData),
    _groundComponent(_commonData),
    _junctionComponent(_commonData),
    _waterComponent(_commonData)
{
}

void DrawCityModule::setup()
{
    _sunComponent.setup();
    _skyComponent.setup();
    _groundComponent.setup();
    _junctionComponent.setup();
    _waterComponent.setup();
}

void DrawCityModule::draw()
{
    _sunComponent.draw();
    _skyComponent.draw();
    _groundComponent.draw();
    _junctionComponent.draw();
    _waterComponent.draw();
}

void DrawCityModule::update()
{
    Vec4f sunDir = _commonData.cityMap.sun().direction().normalized();
    _commonData.sunLight.direction = sunDir;
    _commonData.viewedSunDirection = _commonData.viewMat * sunDir;

    _sunComponent.update();
    _skyComponent.update();
    _groundComponent.update();
    _junctionComponent.update();
    _waterComponent.update();
}

void DrawCityModule::updateProjectionMatrix(const Matrix4x4<float>& proj)
{
    _commonData.projMat = proj;

    _sunComponent.updateProjectionMatrix();
    _skyComponent.updateProjectionMatrix();
    _groundComponent.updateProjectionMatrix();
    _junctionComponent.updateProjectionMatrix();
    _waterComponent.updateProjectionMatrix();
}

void DrawCityModule::updateModelViewMatrix(const Matrix4x4<float>& view)
{
    _commonData.viewMat = view;
    _commonData.normalMat = view.subMat3();

    _sunComponent.updateModelViewMatrix();
    _skyComponent.updateModelViewMatrix();
    _groundComponent.updateModelViewMatrix();
    _junctionComponent.updateModelViewMatrix();
    _waterComponent.updateModelViewMatrix();
}

DrawCityCommonData& DrawCityModule::commonData()
{
    return _commonData;
}
