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
    cellar::Vec3f derivate(cellar::Vec2ui pos);

    cellar::GLShaderProgram _sunShader;
    float _sunRadius;
    cellar::GLShaderProgram _hillsShader;
    cellar::DirectionnalLight3D _sunLight;
    cellar::Matrix4x4<float> _sunRotation;
    cellar::Matrix4x4<float> _modelview;
    int _sNbElems;
    int _gNbElems;
    int _wNbElems;
    unsigned int _svao;
    unsigned int _gvao;
    unsigned int _wvao;
};

#endif // DRAWNUDEHILLS_H
