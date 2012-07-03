#include "WaterComponent.h"
#include "DrawCityModule.h"

#include <GL/glew.h>
using namespace std;
#include <Misc/CellarUtils.h>
using namespace cellar;


WaterComponent::WaterComponent(DrawCityCommonData& common) :
    AbstractComponent(common),
    _waterVao(0),
    _waterNbElems(4)
{
    glGenVertexArrays(1, &_waterVao);
}

WaterComponent::~WaterComponent()
{
    glDeleteVertexArrays(1, &_waterVao);
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
    glBindVertexArray( _waterVao );

    GLuint wBuffer;
    glGenBuffers(1, &wBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, wBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*wpositions) * _waterNbElems, wpositions, GL_STATIC_DRAW);

    int position_loc = _common.waterShader.getAttributeLocation("position_att");
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
    _common.waterShader.pushThisProgram();

    glBindVertexArray(_waterVao);
    glEnable(GL_BLEND);
    glDrawArrays(GL_TRIANGLE_FAN, 0, _waterNbElems);
    glDisable(GL_BLEND);

    _common.waterShader.popProgram();
}
