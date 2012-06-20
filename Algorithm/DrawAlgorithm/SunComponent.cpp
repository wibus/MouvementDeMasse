#include "SunComponent.h"
#include "DrawCityModule.h"

#include <GL/glew.h>
using namespace cellar;

SunComponent::SunComponent(DrawCityCommonData &common) :
    _common(common),
    _sunVao(0),
    _sunNbElems(4)
{
     _common.sunShader.pushThisProgram();
     _common.sunShader.setVec4f("Color", _common.sunColor);
     _common.sunShader.setFloat("Radius", _common.sunRadius);
     _common.sunShader.popProgram();

     glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
     glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
}

void SunComponent::setup()
{
    Vec2f* relPos = new Vec2f[_sunNbElems];
    relPos[0] = Vec2f(-1.0, -1.0);
    relPos[1] = Vec2f(1.00, -1.0);
    relPos[2] = Vec2f(1.00, 1.00);
    relPos[3] = Vec2f(-1.0, 1.00);


    glGenVertexArrays(1, &_sunVao);
    glBindVertexArray(_sunVao);

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2f) * _sunNbElems, relPos, GL_STATIC_DRAW);

    int relPos_loc = _common.sunShader.getAttributeLocation("relPos_att");
    glEnableVertexAttribArray(relPos_loc);
    glVertexAttribPointer(relPos_loc, 2, GL_FLOAT, 0, 0, 0);

    // Clearage
    delete [] relPos;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void SunComponent::draw()
{
    _common.sunShader.pushThisProgram();

    glEnable(GL_BLEND);
    glDepthMask(false);
    glBindVertexArray(_sunVao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, _sunNbElems);
    glDepthMask(true);
    glDisable(GL_BLEND);

    _common.sunShader.popProgram();
}

void SunComponent::update()
{
    _common.sunShader.pushThisProgram();
    _common.sunShader.setVec4f("SunDirection", _common.sunLight.direction);
    _common.sunShader.popProgram();
}

void SunComponent::updateProjectionMatrix()
{
    _common.sunShader.pushThisProgram();
    _common.sunShader.setMatrix4x4("Projection", _common.projMat);
    _common.sunShader.popProgram();
}

void SunComponent::updateModelViewMatrix()
{
    _common.sunShader.pushThisProgram();
    _common.sunShader.setMatrix3x3("Rotation",  _common.normalMat);
    _common.sunShader.popProgram();
}
