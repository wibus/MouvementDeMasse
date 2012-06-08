#ifndef DRAWHIGHPRECISIONGROUND_H
#define DRAWHIGHPRECISIONGROUND_H

#include "DrawSunAndGround.h"

class DrawHighPrecisionGround : public DrawSunAndGround
{
public:
    DrawHighPrecisionGround();

    virtual void setup(CityMap& cityMap);
    virtual void draw();

    virtual void updateViewMatrix(const cellar::Matrix4x4<float>& view);
    virtual void updateProjectionMatrix(const cellar::Matrix4x4<float>& proj);

private:
    cellar::GLShaderProgram _highPrecShader;
    unsigned int _highPrecvao;
    unsigned int _highPrecNbElems;
};

#endif // DRAWHIGHPRECISIONGROUND_H
