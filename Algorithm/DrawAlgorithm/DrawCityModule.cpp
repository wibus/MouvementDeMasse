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
    grassColor(   0.32, 0.90, 0.08, 1.00),
    mudColor(     0.20, 0.14, 0.01, 1.00),
    waterColor(   0.03, 0.03, 0.30, 0.50),
    groundShininess(4.0f),
    waterShininess( 200.0f),
    roadWidth(0.125f)
{
    sunLight.direction(0, 0, 0.0, 0.0).normalize();
    sunLight.ambient = curSkyColor;
    sunLight.diffuse( 0.62, 0.62, 0.60);
    sunLight.specular(0.65, 0.50, 0.30);


    // Sun
    GLInOutProgramLocation sunLocations;
    sunLocations.setInput(0, "relPos_att");
    sunShader.setInAndOutLocations(sunLocations);
    sunShader.loadShadersFromFile("resources/shaders/sun.vert",
                                  "resources/shaders/sun.frag");

    // Ground
    GLInOutProgramLocation groundLocations;
    groundLocations.setInput(0, "position_att");
    groundLocations.setInput(1, "normal_att");
    groundLocations.setInput(2, "texCoord_att");
    groundShader.setInAndOutLocations(groundLocations);
    groundShader.loadShadersFromFile("resources/shaders/ground.vert",
                                     "resources/shaders/ground.frag");

    // Roads
    GLInOutProgramLocation junctionsLocations;
    junctionsLocations.setInput(0, "position_att");
    junctionsLocations.setInput(1, "normal_att");
    junctionsLocations.setInput(2, "texCoord_att");
    roadsShader.setInAndOutLocations(junctionsLocations);
    roadsShader.loadShadersFromFile("resources/shaders/roads.vert",
                                        "resources/shaders/roads.frag");

    // Water
    GLInOutProgramLocation waterLocations;
    waterLocations.setInput(0, "position_att");
    waterShader.setInAndOutLocations(waterLocations);
    waterShader.loadShadersFromFile("resources/shaders/water.vert",
                                    "resources/shaders/water.frag");
}

DrawCityModule::DrawCityModule(CityMap &cityMap) :
    _commonData(cityMap),
    _sunComponent(_commonData),
    _skyComponent(_commonData),
    _groundComponent(_commonData),
    _junctionComponent(_commonData),
    _streetsComponent(_commonData),
    _waterComponent(_commonData)
{
}

void DrawCityModule::setup()
{
    _sunComponent.setup();
    _skyComponent.setup();
    _groundComponent.setup();
    _junctionComponent.setup();
    _streetsComponent.setup();
    _waterComponent.setup();
}

void DrawCityModule::draw()
{
    _sunComponent.draw();
    _skyComponent.draw();
    _groundComponent.draw();
    _junctionComponent.draw();
    _streetsComponent.draw();
    _waterComponent.draw();
}

void DrawCityModule::update()
{
    // Sky color
    const double skyCoefCorrection = 0.1;
    Vec4f  nLightDir    = _commonData.sunLight.direction.normalized();
    double sunIntensity = cellar::max(nLightDir * Vec4f(0, 0, -1, 0) + skyCoefCorrection, 0.0);
    double skyColorCoef = pow(sunIntensity, 0.75) / (skyCoefCorrection + 1.0);
    _commonData.curSkyColor = _commonData.nightSkyColor * (1 - skyColorCoef) +
                              _commonData.daySkyColor   * skyColorCoef;

    // Sun position
    Vec4f sunDir = _commonData.cityMap.sun().direction().normalized();
    _commonData.sunLight.direction = sunDir;
    _commonData.viewedSunDirection = _commonData.viewMat * sunDir;
    _commonData.sunLight.ambient = _commonData.curSkyColor * 0.5f;


    // Components Updates
    _sunComponent.update();
    _skyComponent.update();
    _groundComponent.update();
    _junctionComponent.update();
    _streetsComponent.update();
    _waterComponent.update();
}

void DrawCityModule::updateProjectionMatrix(const Matrix4x4<float>& proj)
{
    _commonData.projMat = proj;

    _sunComponent.updateProjectionMatrix();
    _skyComponent.updateProjectionMatrix();
    _groundComponent.updateProjectionMatrix();
    _junctionComponent.updateProjectionMatrix();
    _streetsComponent.updateProjectionMatrix();
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
    _streetsComponent.updateModelViewMatrix();
    _waterComponent.updateModelViewMatrix();
}

DrawCityCommonData& DrawCityModule::commonData()
{
    return _commonData;
}
