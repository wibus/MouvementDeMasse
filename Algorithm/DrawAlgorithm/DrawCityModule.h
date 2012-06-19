#ifndef DRAWCITYMODULE_H
#define DRAWCITYMODULE_H

#include <Graphics/Light/Light3D.h>
#include <Graphics/GL/GLShaderProgram.h>

#include "City/CityMap.h"
#include "SkyComponent.h"
#include "SunComponent.h"
#include "GroundComponent.h"
#include "JunctionsComponent.h"
#include "StreetsComponent.h"
#include "WaterComponent.h"



struct DrawCityCommonData
{
    DrawCityCommonData(CityMap& cityMap);

    // CityMap
    CityMap& cityMap;
    Ground&  ground;

    // Matrices
    cellar::Matrix4x4<float> projMat;
    cellar::Matrix4x4<float> viewMat;
    cellar::Matrix3x3<float> normalMat;

    // Shaders
    cellar::GLShaderProgram sunShader;
    cellar::GLShaderProgram groundShader;
    cellar::GLShaderProgram roadsShader;
    cellar::GLShaderProgram waterShader;

    // Sun
    float sunRadius;
    cellar::DirectionnalLight3D sunLight;
    cellar::Vec4f viewedSunDirection;

    // Colors
    cellar::Vec4f sunColor;
    cellar::Vec4f nightSkyColor;
    cellar::Vec4f daySkyColor;
    cellar::Vec4f curSkyColor;
    cellar::Vec4f grassColor;
    cellar::Vec4f mudColor;
    cellar::Vec4f waterColor;

    // Shininesses
    float groundShininess;
    float waterShininess;

    // Roads
    float roadWidth;
};

class DrawCityModule
{
public:
    DrawCityModule(CityMap& cityMap);

    void setup();
    void draw();
    void update();

    void updateProjectionMatrix(const cellar::Matrix4x4<float>& proj);
    void updateModelViewMatrix(const cellar::Matrix4x4<float>& view);

    DrawCityCommonData& commonData();

protected:
    DrawCityCommonData _commonData;
    SunComponent _sunComponent;
    SkyComponent _skyComponent;
    GroundComponent _groundComponent;
    JunctionsComponent _junctionComponent;
    StreetsComponent _streetsComponent;
    WaterComponent _waterComponent;
};



#endif // DRAWCITYMODULE_H
