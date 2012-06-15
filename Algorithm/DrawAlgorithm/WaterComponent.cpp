#include "WaterComponent.h"
#include "DrawCityModule.h"

#include <GL/glew.h>
using namespace std;
#include <Misc/CellarUtils.h>
using namespace cellar;


WaterComponent::WaterComponent(DrawCityCommonData& common) :
    _common(common),
    _waterShader(),
    _waterVao(0),
    _waterNbElems(4)
{
    GLInOutProgramLocation waterLocations;
    waterLocations.setInput(0, "position_att");
    _waterShader.setInAndOutLocations(waterLocations);
    _waterShader.loadShadersFromFile("resources/shaders/water.vert",
                                     "resources/shaders/water.frag");
    _waterShader.pushThisProgram();
    _waterShader.setVec4f("sun.direction", _common.sunLight.direction);
    _waterShader.setVec3f("sun.ambient",   _common.sunLight.ambient);
    _waterShader.setVec3f("sun.diffuse",   _common.sunLight.diffuse);
    _waterShader.setVec3f("sun.specular",  _common.sunLight.specular);
    _waterShader.setFloat("Shininess",     _common.waterShininess);
    _waterShader.setVec4f("WaterColor",    _common.waterColor);
    _waterShader.popProgram();
}

void WaterComponent::setup()
{
    // Positions
    float waterHeight = _common.ground.waterHeight();
    Vec3f* wpositions = new Vec3f[_waterNbElems];
    wpositions[0] = Vec3f(0.0,                        0.0,                          waterHeight);
    wpositions[1] = Vec3f(_common.cityMap.size().x(), 0.0,                          waterHeight);
    wpositions[2] = Vec3f(_common.cityMap.size().x(), _common.cityMap.size().y(),   waterHeight);
    wpositions[3] = Vec3f(0.0,                        _common.cityMap.size().y(),   waterHeight);


    // Water VAO setup
    glGenVertexArrays(1, &_waterVao);
    glBindVertexArray( _waterVao );

    GLuint wBuffer;
    glGenBuffers(1, &wBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, wBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*wpositions) * _waterNbElems, wpositions, GL_STATIC_DRAW);

    int position_loc = _waterShader.getAttributeLocation("position_att");
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );

    // Arrays sweaping
    delete [] wpositions;
}

void WaterComponent::draw()
{
    _waterShader.pushThisProgram();

    glBindVertexArray(_waterVao);
    glEnable(GL_BLEND);
    glDrawArrays(GL_TRIANGLE_FAN, 0, _waterNbElems);
    glDisable(GL_BLEND);

    _waterShader.popProgram();
}

void WaterComponent::update()
{
    _waterShader.pushThisProgram();
    _waterShader.setVec4f("sun.direction",   _common.viewedSunDirection);
    _waterShader.popProgram();
}

void WaterComponent::updateProjectionMatrix()
{
    _waterShader.pushThisProgram();
    _waterShader.setMatrix4x4("ProjectionMatrix", _common.projMat);
    _waterShader.popProgram();
}

void WaterComponent::updateModelViewMatrix()
{
    _waterShader.pushThisProgram();
    _waterShader.setMatrix4x4("ModelViewMatrix", _common.viewMat);
    _waterShader.setMatrix3x3("NormalMatrix",    _common.normalMat);
    _waterShader.popProgram();
}
