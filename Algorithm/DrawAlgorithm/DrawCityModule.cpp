#include "DrawCityModule.h"

#include <GL/glew.h>

#include "City/CityMap.h"
#include "SkyComponent.h"
#include "GroundComponent.h"
#include "JunctionsComponent.h"
#include "StreetsComponent.h"
#include "BuildingsComponent.h"
#include "WaterComponent.h"

using namespace cellar;


DrawCityCommonData::DrawCityCommonData(CityMap& cityMap) :
    cityMap(cityMap),
    ground(cityMap.ground()),
    sunRadius(10.0f),
    sunLight(),
    sunColor(     1.00, 0.90, 0.20, 1.00),
    nightSkyColor(0.02, 0.01, 0.05, 1.00),
    daySkyColor(  0.40, 0.60, 0.80, 1.00),
    skylineColor( 0.12, 0.12, 0.12, 1.00),
    curSkyColor(  nightSkyColor         ),
    grassColor(   0.32, 0.90, 0.08, 1.00),
    mudColor(     0.20, 0.14, 0.01, 1.00),
    waterColor(   0.03, 0.03, 0.30, 0.42),
    cloudyness(0.5),
    cloudsTightness(0.4),
    waterShininess( 200.0f),
    roadWidth(0.125f)
{
    sunLight.direction(0, 0, 0.0, 0.0).normalize();
    sunLight.ambient = curSkyColor;
    sunLight.diffuse( 0.76, 0.74, 0.72);
    sunLight.specular(0.65, 0.58, 0.46);


    // Sky
    GLInOutProgramLocation skyLocations;
    skyLocations.setInput(0, "psotion_att");
    skyLocations.setInput(1, "texCoord_att");
    skyShader.setInAndOutLocations(skyLocations);
    skyShader.loadShadersFromFile("resources/shaders/sky.vert",
                                  "resources/shaders/sky.frag");
    skyShader.pushThisProgram();
    skyShader.setVec4f("SkyColor", curSkyColor);
    skyShader.setVec4f("SkylineColor", skylineColor);
    skyShader.setVec4f("SunColor", sunColor);
    skyShader.setFloat("SunRadius", sunRadius);
    skyShader.setVec3f("SunPosition", Vec3f(0.0f, 0.0f, -1.0f));
    skyShader.setVec2f("TexShift",    Vec2f(0.0f, 0.0f));
    skyShader.setFloat("Cloudyness",  cloudyness);
    skyShader.setFloat("CloudsTightness", cloudsTightness);
    skyShader.setInt("CloudsTexUnit", 0);
    skyShader.popProgram();

    // Ground
    GLInOutProgramLocation groundLocations;
    groundLocations.setInput(0, "position_att");
    groundLocations.setInput(1, "normal_att");
    groundLocations.setInput(2, "texCoord_att");
    groundShader.setInAndOutLocations(groundLocations);
    groundShader.loadShadersFromFile("resources/shaders/ground.vert",
                                     "resources/shaders/ground.frag");
    groundShader.pushThisProgram();
    groundShader.setVec4f("sun.direction", sunLight.direction);
    groundShader.setVec4f("sun.ambient",   sunLight.ambient);
    groundShader.setVec4f("sun.diffuse",   sunLight.diffuse);
    groundShader.setVec4f("sun.specular",  sunLight.specular);
    groundShader.setFloat("WaterHeight",   ground.waterHeight());
    groundShader.setVec4f("GrassColor",    grassColor);
    groundShader.setVec4f("MudColor",      mudColor);
    groundShader.setVec4f("WaterColor",    waterColor);
    groundShader.popProgram();

    // Roads
    GLInOutProgramLocation junctionsLocations;
    junctionsLocations.setInput(0, "position_att");
    junctionsLocations.setInput(1, "normal_att");
    junctionsLocations.setInput(2, "texCoord_att");
    infrastructShader.setInAndOutLocations(junctionsLocations);
    infrastructShader.loadShadersFromFile("resources/shaders/infrastruct.vert",
                                        "resources/shaders/infrastruct.frag");
    infrastructShader.pushThisProgram();
    infrastructShader.setVec4f("sun.direction", sunLight.direction);
    infrastructShader.setVec4f("sun.ambient",   sunLight.ambient);
    infrastructShader.setVec4f("sun.diffuse",   sunLight.diffuse);
    infrastructShader.setVec4f("sun.specular",  sunLight.specular);
    infrastructShader.setFloat("Shininess",     128.0f);
    infrastructShader.setInt("TexUnit",         0);
    infrastructShader.setInt("SpecUnit",        1);
    infrastructShader.popProgram();

    // Water
    GLInOutProgramLocation waterLocations;
    waterLocations.setInput(0, "position_att");
    waterShader.setInAndOutLocations(waterLocations);
    waterShader.loadShadersFromFile("resources/shaders/water.vert",
                                    "resources/shaders/water.frag");
    waterShader.pushThisProgram();
    waterShader.setVec4f("sun.direction", sunLight.direction);
    waterShader.setVec4f("sun.ambient",   sunLight.ambient);
    waterShader.setVec4f("sun.diffuse",   sunLight.diffuse);
    waterShader.setVec4f("sun.specular",  sunLight.specular);
    waterShader.setFloat("Shininess",     waterShininess);
    waterShader.setVec4f("WaterColor",    waterColor);
    waterShader.popProgram();
}

DrawCityModule::DrawCityModule(CityMap &cityMap) :
    _commonData(cityMap),
    _components(),
    _skyComponent(new SkyComponent(_commonData)),
    _groundComponent(new GroundComponent(_commonData)),
    _junctionsComponent(new JunctionsComponent(_commonData)),
    _streetsComponent(new StreetsComponent(_commonData)),
    _buildingsComponent(new BuildingsComponent(_commonData)),
    _waterComponent(new WaterComponent(_commonData))
{
    _components.push_back(_skyComponent);
    _components.push_back(_groundComponent);
    _components.push_back(_junctionsComponent);
    _components.push_back(_streetsComponent);
    _components.push_back(_buildingsComponent);
    _components.push_back(_waterComponent);
}

void DrawCityModule::setup()
{
    for(size_t i=0; i<_components.size(); ++i)
        _components[i]->setup();
}

void DrawCityModule::draw()
{
    for(size_t i=0; i<_components.size(); ++i)
        _components[i]->draw();
}

void DrawCityModule::update()
{
    // Sky color
    const double skyCoefCorrection = 0.1;
    Vec4f  nLightDir    = _commonData.sunLight.direction.normalized();
    double sunIntensity = cellar::max(nLightDir * Vec4f(0, 0, -1, 0) + skyCoefCorrection, 0.0);
    double skyColorCoef = pow(sunIntensity, 0.75) / (skyCoefCorrection + 1.0);
    Vec4f  skyCol = _commonData.nightSkyColor * (1 - skyColorCoef) +
                    _commonData.daySkyColor   * skyColorCoef;
    _commonData.curSkyColor = skyCol;

    skyCol += _commonData.skylineColor;
    glClearColor(skyCol[0], skyCol[1], skyCol[2], skyCol[3]);


    // Sun ambient light
    const float AMBIENT_EFF_FACT = 0.50;
    const float BASE_INTENSITY = 0.03;
    const Vec4f BASE_LIGHT = Vec4f(BASE_INTENSITY, BASE_INTENSITY, BASE_INTENSITY, 0.0f);
    _commonData.sunLight.ambient = BASE_LIGHT + _commonData.curSkyColor * AMBIENT_EFF_FACT;


    // Sun position
    Vec4f sunDir = _commonData.cityMap.sun().direction().normalized();
    _commonData.sunLight.direction = sunDir;
    _commonData.viewedSunDirection = _commonData.viewMat * sunDir;    

    updateShaders();
}

void DrawCityModule::updateShaders()
{
    static Vec2f shift(0.0f, 0.0f);
    shift[1] += 0.0004;

    _commonData.skyShader.pushThisProgram();
    _commonData.skyShader.setVec4f("SkyColor", _commonData.curSkyColor);
    _commonData.skyShader.setVec4f("SunPosition", -_commonData.viewedSunDirection);
    _commonData.skyShader.setVec2f("TexShift", shift);
    _commonData.skyShader.popProgram();

    _commonData.groundShader.pushThisProgram();
    _commonData.groundShader.setVec4f("sun.direction", _commonData.viewedSunDirection);
    _commonData.groundShader.setVec4f("sun.ambient",   _commonData.sunLight.ambient);
    _commonData.groundShader.popProgram();

    _commonData.infrastructShader.pushThisProgram();
    _commonData.infrastructShader.setVec4f("sun.direction", _commonData.viewedSunDirection);
    _commonData.infrastructShader.setVec4f("sun.ambient",   _commonData.sunLight.ambient);
    _commonData.infrastructShader.popProgram();

    _commonData.waterShader.pushThisProgram();
    _commonData.waterShader.setVec4f("sun.direction", _commonData.viewedSunDirection);
    _commonData.waterShader.setVec4f("sun.ambient",   _commonData.sunLight.ambient);
    _commonData.waterShader.popProgram();
}

void DrawCityModule::updateProjectionMatrix(const Matrix4x4<float>& proj)
{
    _commonData.projMat = proj;

    updateShadersProjectionMatrix();
}

void DrawCityModule::updateShadersProjectionMatrix()
{
    _commonData.skyShader.pushThisProgram();
    _commonData.skyShader.setMatrix4x4("Projection", _commonData.projMat);
    _commonData.skyShader.popProgram();

    _commonData.groundShader.pushThisProgram();
    _commonData.groundShader.setMatrix4x4("ProjectionMatrix", _commonData.projMat);
    _commonData.groundShader.popProgram();

    _commonData.infrastructShader.pushThisProgram();
    _commonData.infrastructShader.setMatrix4x4("ProjectionMatrix", _commonData.projMat);
    _commonData.infrastructShader.popProgram();

    _commonData.waterShader.pushThisProgram();
    _commonData.waterShader.setMatrix4x4("ProjectionMatrix", _commonData.projMat);
    _commonData.waterShader.popProgram();
}

void DrawCityModule::updateModelViewMatrix(const Matrix4x4<float>& view)
{
    _commonData.viewMat = view;
    _commonData.normalMat = view.subMat3();

    updateShadersModelViewMatrix();
}

void DrawCityModule::updateShadersModelViewMatrix()
{
    _commonData.skyShader.pushThisProgram();
    _commonData.skyShader.setMatrix3x3("View",  _commonData.normalMat);
    _commonData.skyShader.popProgram();

    _commonData.groundShader.pushThisProgram();
    _commonData.groundShader.setMatrix4x4("ModelViewMatrix", _commonData.viewMat);
    _commonData.groundShader.setMatrix3x3("NormalMatrix",    _commonData.normalMat);
    _commonData.groundShader.popProgram();

    _commonData.infrastructShader.pushThisProgram();
    _commonData.infrastructShader.setMatrix4x4("ModelViewMatrix", _commonData.viewMat);
    _commonData.infrastructShader.setMatrix3x3("NormalMatrix",    _commonData.normalMat);
    _commonData.infrastructShader.popProgram();

    _commonData.waterShader.pushThisProgram();
    _commonData.waterShader.setMatrix4x4("ModelViewMatrix", _commonData.viewMat);
    _commonData.waterShader.setMatrix3x3("NormalMatrix",    _commonData.normalMat);
    _commonData.waterShader.popProgram();
}

DrawCityCommonData& DrawCityModule::commonData()
{
    return _commonData;
}
