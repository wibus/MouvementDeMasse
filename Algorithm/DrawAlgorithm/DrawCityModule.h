#ifndef DRAWCITYMODULE_H
#define DRAWCITYMODULE_H

#include <vector>

#include <Graphics/Light/Light3D.h>
#include <Graphics/GL/GLShaderProgram.h>

#include "City/City.h"

class AbstractComponent;
class SkyComponent;
class GroundComponent;
class JunctionsComponent;
class StreetsComponent;
class BuildingsComponent;
class WaterComponent;


struct DrawCityCommonData
{
    DrawCityCommonData(City& city);

    // City
    City& city;
    Ground&  ground;

    // Matrices
    cellar::Matrix4x4<float> projMat;
    cellar::Matrix4x4<float> viewMat;
    cellar::Matrix3x3<float> normalMat;

    // Shaders
    cellar::GLShaderProgram skyShader;
    cellar::GLShaderProgram groundShader;
    cellar::GLShaderProgram infrastructShader;
    cellar::GLShaderProgram waterShader;

    // Sun
    float sunRadius;
    cellar::DirectionnalLight3D sunLight;
    cellar::Vec4f viewedSunDirection;

    // Colors
    cellar::Vec4f sunColor;
    cellar::Vec4f nightSkyColor;
    cellar::Vec4f daySkyColor;
    cellar::Vec4f skylineColor;
    cellar::Vec4f curSkyColor;
    cellar::Vec4f grassColor;
    cellar::Vec4f mudColor;
    cellar::Vec4f waterColor;

    //Clouds
    float cloudyness;
    float cloudsTightness;

    // Shininesses
    float waterShininess;

    // Roads
    float roadWidth;
};

class DrawCityModule
{
public:
    DrawCityModule(City& city);

    void setup();
    void draw();
    void update();

    void updateProjectionMatrix(const cellar::Matrix4x4<float>& proj);
    void updateModelViewMatrix(const cellar::Matrix4x4<float>& view);

    DrawCityCommonData& commonData();

protected:
    void updateShaders();
    void updateShadersProjectionMatrix();
    void updateShadersModelViewMatrix();


    DrawCityCommonData  _commonData;
    std::vector<AbstractComponent*> _components;
    SkyComponent*       _skyComponent;
    GroundComponent*    _groundComponent;
    JunctionsComponent* _junctionsComponent;
    StreetsComponent*   _streetsComponent;
    BuildingsComponent* _buildingsComponent;
    WaterComponent*     _waterComponent;
};



#endif // DRAWCITYMODULE_H
