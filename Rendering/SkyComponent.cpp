#include "SkyComponent.h"

#include <vector>
using namespace std;

#include <GL/glew.h>
#include <Misc/CellarUtils.h>
#include <Graphics/GL/GLToolkit.h>
using namespace cellar;


SkyComponent::SkyComponent(City &city, GLShaderProgram &shader) :
    AbstractComponent(city, shader),
    _skyBuffs(),
    _skyVao(0),
    _skyNbElems(0),
    _cloudsTex(0),
    _daySkyTex(0),
    _nightSkyTex(0)
{
    glGenVertexArrays(1, &_skyVao);
    glGenBuffers(_SKY_NB_BUFFS, _skyBuffs);

    _cloudsTex = GLToolkit::genTextureId(_city.sky().cloudsImage());
}

SkyComponent::~SkyComponent()
{
    glDeleteVertexArrays(1, &_skyVao);
    glDeleteBuffers(_SKY_NB_BUFFS, _skyBuffs);
    GLToolkit::deleteTextureId(_cloudsTex);
    GLToolkit::deleteTextureId(_daySkyTex);
    GLToolkit::deleteTextureId(_nightSkyTex);
}

void SkyComponent::setup()
{
    setupSky();
}

void SkyComponent::setupSky()
{
    vector<Vec3f> positions;

    const float r = 2.0f;

    positions.push_back(Vec3f(0.0f,           0.0f,          r));
    positions.push_back(Vec3f(r*cos(6*PI/3),  r*sin(6*PI/3), -r/2.0f));
    positions.push_back(Vec3f(r*cos(4*PI/3),  r*sin(4*PI/3), -r/2.0f));
    positions.push_back(Vec3f(r*cos(2*PI/3),  r*sin(2*PI/3), -r/2.0f));
    positions.push_back(Vec3f(r*cos(0*PI/3),  r*sin(0*PI/3), -r/2.0f));
    _skyNbElems = positions.size();


    //VAO setup
    glBindVertexArray(_skyVao);

    int position_loc = _shader.getAttributeLocation("position_att");

    glBindBuffer(GL_ARRAY_BUFFER, _skyBuffs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void SkyComponent::draw()
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(false);

    _shader.pushThisProgram();
    glBindVertexArray(_skyVao);
    glBindTexture(GL_TEXTURE_2D, _cloudsTex);
    glDrawArrays(GL_TRIANGLE_FAN, 0, _skyNbElems);
    _shader.popProgram();

    glDepthMask(true);
    glEnable(GL_DEPTH_TEST);
}
