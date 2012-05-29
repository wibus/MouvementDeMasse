#ifndef DRAWSUN_H
#define DRAWSUN_H

#include <Graphics/GL/GLShaderProgram.h>
#include <Graphics/Light/Light3D.h>

#include "DrawAlgorithm.h"


class DrawSun : public DrawAlgorithm
{
public:
    DrawSun();

    // DrawAlgorithm interface
    virtual void setup(CityMap& cityMap);
    virtual void draw();

    virtual void updateViewMatrix(const cellar::Matrix4x4<float>& view);
    virtual void updateProjectionMatrix(const cellar::Matrix4x4<float>& proj);

protected:
    virtual void updateSunDirection();
    virtual void drawSky();
    virtual void drawSun();


    // Shader specific
    cellar::GLShaderProgram _sunShader;
    unsigned int _sunVao;
    const int _sunNbElems;

    // Sun description
    float _sunRadius;
    cellar::DirectionnalLight3D _sunLight;
    cellar::Vec4f _sunColor;
    cellar::Vec4f _nightSkyColor;
    cellar::Vec4f _daySkyColor;
    cellar::Vec4f _skyColor;
    cellar::Matrix4x4<float> _sunRotation;
};

#endif // DRAWSUN_H
