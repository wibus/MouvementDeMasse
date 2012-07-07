#include "WaterComponent.h"

#include <GL/glew.h>
using namespace std;
#include <Misc/CellarUtils.h>
using namespace cellar;


WaterComponent::WaterComponent(City &city, GLShaderProgram &shader) :
    AbstractComponent(city, shader),
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
    float waterHeight = _ground.waterHeight();
    Vec3f* wpositions = new Vec3f[_waterNbElems];
    wpositions[0] = Vec3f(0.0,              0.0,                waterHeight);
    wpositions[1] = Vec3f(_city.size().x(), 0.0,                waterHeight);
    wpositions[2] = Vec3f(_city.size().x(), _city.size().y(),   waterHeight);
    wpositions[3] = Vec3f(0.0,              _city.size().y(),   waterHeight);


    // Water VAO setup
    glBindVertexArray( _waterVao );

    GLuint wBuffer;
    glGenBuffers(1, &wBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, wBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*wpositions) * _waterNbElems, wpositions, GL_STATIC_DRAW);

    int position_loc = _shader.getAttributeLocation("position_att");
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
    _shader.pushThisProgram();

    glBindVertexArray(_waterVao);
    glEnable(GL_BLEND);
    glDrawArrays(GL_TRIANGLE_FAN, 0, _waterNbElems);
    glDisable(GL_BLEND);

    _shader.popProgram();
}
