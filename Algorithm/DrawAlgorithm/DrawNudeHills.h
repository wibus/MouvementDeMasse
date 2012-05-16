#ifndef DRAWNUDEHILLS_H
#define DRAWNUDEHILLS_H

#include <MathsAndPhysics/Vector.h>
#include <Graphics/GL/GLShaderProgram.h>
#include <Graphics/Camera/Camera.h>
#include <DesignPatterns/SpecificObserver.h>

#include "DrawAlgorithm.h"


class DrawNudeHills : public DrawAlgorithm,
                      public cellar::SpecificObserver<cellar::CameraMsg>
{
public:
    DrawNudeHills();

    virtual void notify(cellar::CameraMsg &msg);

    virtual void setup(CityMap& cityMap);
    virtual void process();
    void setSizeRatio(float ratio);
    float sizeRatio() const;


protected:
    cellar::Vec3f derivate(cellar::Vec2ui pos);

    cellar::GLShaderProgram _shader;
    int _gNbElems;
    unsigned int  _gvao;
    int _wNbElems;
    unsigned int  _wvao;
    float _sizeRatio;
};

#endif // DRAWNUDEHILLS_H
