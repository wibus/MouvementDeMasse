#ifndef DRAWNUDEHILLS_H
#define DRAWNUDEHILLS_H

#include <MathsAndPhysics/Vector.h>
#include <Graphics/GL/GLShaderProgram.h>
#include <Graphics/Camera/Camera.h>
#include <Graphics/Light/Light3D.h>
#include <DesignPatterns/SpecificObserver.h>

#include "DrawAlgorithm.h"


class DrawNudeHills : public DrawAlgorithm,
                      public cellar::SpecificObserver<cellar::CameraMsg>
{
public:
    DrawNudeHills();

    virtual void notify(cellar::CameraMsg &msg);

    virtual void setup(CityMap& cityMap);
    virtual void draw();


protected:
    void setupSun();
    void setupGround();
    void setupWater();
    cellar::Vec3f derivate(cellar::Vec2ui pos);
    void genGroundTexture();
    void genWaterTexture();

    cellar::Matrix4x4<float> _modelview;
    cellar::GLShaderProgram _sunShader;
    int _sNbElems;
    unsigned int _svao;
    cellar::GLShaderProgram _hillsShader;
    int _gNbElems;
    unsigned int _gvao;
    unsigned int _gTex;
    int _wNbElems;
    unsigned int _wvao;
    unsigned int _wTex;

    float _sunRadius;
    cellar::DirectionnalLight3D _sunLight;
    cellar::Matrix4x4<float> _sunRotation;
    cellar::Vec4f _nightSkyColor;
    cellar::Vec4f _daySkyColor;
    cellar::Vec4f _skyColor;
    cellar::Vec4f _waterColor;

};

#endif // DRAWNUDEHILLS_H
