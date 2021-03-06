#include "WaterComponent.h"

using namespace std;


using namespace cellar;

#include <GL3/gl3w.h>



WaterComponent::WaterComponent(City &city, GlProgram &shader) :
    AbstractComponent(city, shader),
    _waterBuffs(),
    _waterVao(0)
{
    glGenVertexArrays(1, &_waterVao);
    glGenBuffers(_WATER_NB_BUFFS, _waterBuffs);
}

WaterComponent::~WaterComponent()
{
    glDeleteVertexArrays(1, &_waterVao);
    glDeleteBuffers(_WATER_NB_BUFFS, _waterBuffs);
}

void WaterComponent::setup()
{
    // Positions
    float waterHeight = _ground.waterHeight();
    glm::vec3 positions[_WATER_NB_ELEMS];
    positions[0] = glm::vec3(0.0,              0.0,                waterHeight);
    positions[1] = glm::vec3(_city.size().x, 0.0,                waterHeight);
    positions[2] = glm::vec3(_city.size().x, _city.size().y,   waterHeight);
    positions[3] = glm::vec3(0.0,              _city.size().y,   waterHeight);


    // Water VAO setup
    glBindVertexArray( _waterVao );

    int position_loc = _shader.getAttributeLocation("position_att");

    glBindBuffer(GL_ARRAY_BUFFER, _waterBuffs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*positions) * _WATER_NB_ELEMS, positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void WaterComponent::draw()
{
    _shader.pushProgram();

    glBindVertexArray(_waterVao);
    glEnable(GL_BLEND);
    glDrawArrays(GL_TRIANGLE_FAN, 0, _WATER_NB_ELEMS);
    glDisable(GL_BLEND);

    _shader.popProgram();
}
