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

    cellar::GLShaderProgram _shader;
    cellar::DirectionnalLight3D _sun;
    cellar::Matrix4x4<float> _sunRotation;
    cellar::Matrix4x4<float> _modelview;
    int _gNbElems;
    unsigned int  _gvao;
    int _wNbElems;
    unsigned int  _wvao;
};

#endif // DRAWNUDEHILLS_H
