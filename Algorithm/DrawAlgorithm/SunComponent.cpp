#include "SunComponent.h"
#include "DrawCityModule.h"

#include <GL/glew.h>
using namespace cellar;

SunComponent::SunComponent(DrawCityCommonData &common) :
    _common(common),
    _sunShader(),
    _sunVao(0),
    _sunNbElems(4)
{
    GLInOutProgramLocation locations;
    locations.setInput(0, "relPos_att");
     _sunShader.setInAndOutLocations(locations);
     _sunShader.loadShadersFromFile("resources/shaders/sun.vert",
                                    "resources/shaders/sun.frag");
     _sunShader.pushThisProgram();
     _sunShader.setVec4f("Color", _common.sunColor);
     _sunShader.setFloat("Radius", _common.sunRadius);
     glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
     glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
     _sunShader.popProgram();
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

    int relPos_loc = _sunShader.getAttributeLocation("relPos_att");
    glEnableVertexAttribArray(relPos_loc);
    glVertexAttribPointer(relPos_loc, 2, GL_FLOAT, 0, 0, 0);

    // Clearage
    delete [] relPos;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void SunComponent::draw()
{
    _sunShader.pushThisProgram();

    glEnable(GL_BLEND);
    glDepthMask(false);
    glBindVertexArray(_sunVao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, _sunNbElems);
    glDepthMask(true);
    glDisable(GL_BLEND);

    _sunShader.popProgram();
}

void SunComponent::update()
{
    _sunShader.pushThisProgram();
    _sunShader.setVec4f("SunDirection", _common.sunLight.direction);
    _sunShader.popProgram();
}

void SunComponent::updateProjectionMatrix()
{
    _sunShader.pushThisProgram();
    _sunShader.setMatrix4x4("Projection", _common.projMat);
    _sunShader.popProgram();
}

void SunComponent::updateModelViewMatrix()
{
    _sunShader.pushThisProgram();
    _sunShader.setMatrix3x3("Rotation",  _common.normalMat);
    _sunShader.popProgram();
}
