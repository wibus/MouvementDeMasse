#include "DrawCityModule.h"

#include <GL/glew.h>

#include "City/City.h"
#include "SkyComponent.h"
#include "GroundComponent.h"
#include "JunctionsComponent.h"
#include "StreetsComponent.h"
#include "BuildingsComponent.h"
#include "BridgesComponent.h"
#include "WaterComponent.h"

using namespace cellar;


DrawCityModule::DrawCityModule(City &city) :
    _city(  city),
    _ground(city.ground()),
    _visual(city.visual()),
    _skyShader(),
    _groundShader(),
    _infrastructShader(),
    _bridgeShader(),
    _waterShader(),
    _components(),
    _skyComponent(      new SkyComponent(      _city, _skyShader)),
    _groundComponent(   new GroundComponent(   _city, _groundShader)),
    _junctionsComponent(new JunctionsComponent(_city, _infrastructShader)),
    _streetsComponent(  new StreetsComponent(  _city, _infrastructShader)),
    _buildingsComponent(new BuildingsComponent(_city, _infrastructShader)),
    _bridgesComponent(  new BridgesComponent(  _city, _bridgeShader)),
    _waterComponent(    new WaterComponent(    _city, _waterShader))
{
    // Draw components
    _components.push_back(_skyComponent);
    _components.push_back(_groundComponent);
    _components.push_back(_junctionsComponent);
    _components.push_back(_streetsComponent);
    _components.push_back(_buildingsComponent);
    _components.push_back(_bridgesComponent);
    _components.push_back(_waterComponent);


    // Sky
    GLInOutProgramLocation skyLocations;
    skyLocations.setInput(0, "psotion_att");
    skyLocations.setInput(1, "texCoord_att");
    _skyShader.setInAndOutLocations(skyLocations);
    _skyShader.loadShadersFromFile("resources/shaders/sky.vert",
                                  "resources/shaders/sky.frag");
    _skyShader.pushThisProgram();
    _skyShader.setVec4f("SkyColor",     _visual.curSkyColor);
    _skyShader.setVec4f("SkylineColor", _visual.skylineColor);
    _skyShader.setVec4f("SunColor",     _visual.sunColor);
    _skyShader.setFloat("SunRadius",    _visual.sunRadius);
    _skyShader.setVec3f("SunPosition",  Vec3f(0.0f, 0.0f, -1.0f));
    _skyShader.setVec2f("TexShift",     Vec2f(0.0f, 0.0f));
    _skyShader.setInt("CloudsTexUnit",  0);
    _skyShader.popProgram();

    // Ground
    GLInOutProgramLocation groundLocations;
    groundLocations.setInput(0, "position_att");
    groundLocations.setInput(1, "normal_att");
    groundLocations.setInput(2, "texCoord_att");
    _groundShader.setInAndOutLocations(groundLocations);
    _groundShader.loadShadersFromFile("resources/shaders/ground.vert",
                                     "resources/shaders/ground.frag");
    _groundShader.pushThisProgram();
    _groundShader.setVec4f("sun.direction", _visual.sunLight.direction);
    _groundShader.setVec4f("sun.ambient",   _visual.sunLight.ambient);
    _groundShader.setVec4f("sun.diffuse",   _visual.sunLight.diffuse);
    _groundShader.setVec4f("sun.specular",  _visual.sunLight.specular);
    _groundShader.setFloat("WaterHeight",   _ground.waterHeight());
    _groundShader.setVec4f("GrassColor",    _visual.grassColor);
    _groundShader.setVec4f("MudColor",      _visual.mudColor);
    _groundShader.setVec4f("WaterColor",    _visual.waterColor);
    _groundShader.popProgram();

    // Infrastructure
    GLInOutProgramLocation infrastructureLocations;
    infrastructureLocations.setInput(0, "position_att");
    infrastructureLocations.setInput(1, "normal_att");
    infrastructureLocations.setInput(2, "texCoord_att");
    _infrastructShader.setInAndOutLocations(infrastructureLocations);
    _infrastructShader.loadShadersFromFile("resources/shaders/infrastruct.vert",
                                        "resources/shaders/infrastruct.frag");
    _infrastructShader.pushThisProgram();
    _infrastructShader.setVec4f("sun.direction", _visual.sunLight.direction);
    _infrastructShader.setVec4f("sun.ambient",   _visual.sunLight.ambient);
    _infrastructShader.setVec4f("sun.diffuse",   _visual.sunLight.diffuse);
    _infrastructShader.setVec4f("sun.specular",  _visual.sunLight.specular);
    _infrastructShader.setFloat("Shininess",     128.0f);
    _infrastructShader.setInt("TexUnit",         0);
    _infrastructShader.setInt("SpecUnit",        1);
    _infrastructShader.popProgram();


    // Infrastructure
    GLInOutProgramLocation bridgeLocations;
    bridgeLocations.setInput(0, "position_att");
    bridgeLocations.setInput(1, "normal_att");
    bridgeLocations.setInput(2, "texCoord_att");
    _bridgeShader.setInAndOutLocations(bridgeLocations);
    _bridgeShader.loadShadersFromFile("resources/shaders/bridge.vert",
                                      "resources/shaders/bridge.frag");
    _bridgeShader.pushThisProgram();
    _infrastructShader.popProgram();


    // Water
    GLInOutProgramLocation waterLocations;
    waterLocations.setInput(0, "position_att");
    _waterShader.setInAndOutLocations(waterLocations);
    _waterShader.loadShadersFromFile("resources/shaders/water.vert",
                                    "resources/shaders/water.frag");
    _waterShader.pushThisProgram();
    _waterShader.setVec4f("sun.direction", _visual.sunLight.direction);
    _waterShader.setVec4f("sun.ambient",   _visual.sunLight.ambient);
    _waterShader.setVec4f("sun.diffuse",   _visual.sunLight.diffuse);
    _waterShader.setVec4f("sun.specular",  _visual.sunLight.specular);
    _waterShader.setFloat("Shininess",     _visual.waterShininess);
    _waterShader.setVec4f("WaterColor",    _visual.waterColor);
    _waterShader.popProgram();
}

DrawCityModule::~DrawCityModule()
{
    for(size_t i=0; i<_components.size(); ++i)
        delete _components[i];
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
    Vec4f  nLightDir    = _visual.sunLight.direction.normalized();
    double sunIntensity = maxVal(nLightDir * Vec4f(0, 0, -1, 0) + skyCoefCorrection, 0.0);
    double skyColorCoef = pow(sunIntensity, 0.75) / (skyCoefCorrection + 1.0);
    Vec4f  skyCol = _visual.nightSkyColor * (1 - skyColorCoef) +
                    _visual.daySkyColor   * skyColorCoef;
    _visual.curSkyColor = skyCol;

    skyCol += _visual.skylineColor;
    glClearColor(skyCol[0], skyCol[1], skyCol[2], skyCol[3]);


    // Sun ambient light
    const float AMBIENT_EFF_FACT = 0.50;
    const float BASE_INTENSITY = 0.03;
    const Vec4f BASE_LIGHT = Vec4f(BASE_INTENSITY, BASE_INTENSITY, BASE_INTENSITY, 0.0f);
    _visual.sunLight.ambient = BASE_LIGHT + _visual.curSkyColor * AMBIENT_EFF_FACT;


    // Sun position
    Vec4f sunDir = _city.sun().direction().normalized();
    _visual.sunLight.direction = sunDir;
    _visual.viewedSunDirection = _visual.viewMat * sunDir;

    updateShaders();
}

void DrawCityModule::updateShaders()
{
    static Vec2f shift(0.0f, 0.0f);
    shift[1] += 0.0004;

    _skyShader.pushThisProgram();
    _skyShader.setVec4f("SkyColor", _visual.curSkyColor);
    _skyShader.setVec4f("SunPosition", -_visual.viewedSunDirection);
    _skyShader.setVec2f("TexShift", shift);
    _skyShader.popProgram();

    _groundShader.pushThisProgram();
    _groundShader.setVec4f("sun.direction", _visual.viewedSunDirection);
    _groundShader.setVec4f("sun.ambient",   _visual.sunLight.ambient);
    _groundShader.popProgram();

    _infrastructShader.pushThisProgram();
    _infrastructShader.setVec4f("sun.direction", _visual.viewedSunDirection);
    _infrastructShader.setVec4f("sun.ambient",   _visual.sunLight.ambient);
    _infrastructShader.popProgram();

    _waterShader.pushThisProgram();
    _waterShader.setVec4f("sun.direction", _visual.viewedSunDirection);
    _waterShader.setVec4f("sun.ambient",   _visual.sunLight.ambient);
    _waterShader.popProgram();
}

void DrawCityModule::updateProjectionMatrix(const Matrix4x4<float>& proj)
{
    _visual.projMat = proj;

    updateShadersProjectionMatrix();
}

void DrawCityModule::updateShadersProjectionMatrix()
{
    _skyShader.pushThisProgram();
    _skyShader.setMatrix4x4("ProjectionMatrix", _visual.projMat);
    _skyShader.popProgram();

    _groundShader.pushThisProgram();
    _groundShader.setMatrix4x4("ProjectionMatrix", _visual.projMat);
    _groundShader.popProgram();

    _infrastructShader.pushThisProgram();
    _infrastructShader.setMatrix4x4("ProjectionMatrix", _visual.projMat);
    _infrastructShader.popProgram();

    _bridgeShader.pushThisProgram();
    _bridgeShader.setMatrix4x4("ProjectionMatrix", _visual.projMat);
    _bridgeShader.popProgram();

    _waterShader.pushThisProgram();
    _waterShader.setMatrix4x4("ProjectionMatrix", _visual.projMat);
    _waterShader.popProgram();
}

void DrawCityModule::updateModelViewMatrix(const Matrix4x4<float>& view)
{
    _visual.viewMat = view;
    _visual.normalMat = view.subMat3();

    updateShadersModelViewMatrix();
}

void DrawCityModule::updateShadersModelViewMatrix()
{
    _skyShader.pushThisProgram();
    _skyShader.setMatrix3x3("ModelViewMatrix",  _visual.normalMat);
    _skyShader.popProgram();

    _groundShader.pushThisProgram();
    _groundShader.setMatrix4x4("ModelViewMatrix", _visual.viewMat);
    _groundShader.setMatrix3x3("NormalMatrix",    _visual.normalMat);
    _groundShader.popProgram();

    _infrastructShader.pushThisProgram();
    _infrastructShader.setMatrix4x4("ModelViewMatrix", _visual.viewMat);
    _infrastructShader.setMatrix3x3("NormalMatrix",    _visual.normalMat);
    _infrastructShader.popProgram();

    _bridgeShader.pushThisProgram();
    _bridgeShader.setMatrix4x4("ModelViewMatrix", _visual.viewMat);
    _bridgeShader.popProgram();

    _waterShader.pushThisProgram();
    _waterShader.setMatrix4x4("ModelViewMatrix", _visual.viewMat);
    _waterShader.setMatrix3x3("NormalMatrix",    _visual.normalMat);
    _waterShader.popProgram();
}
