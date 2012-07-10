#include "SkyComponent.h"

#include <vector>
using namespace std;

#include <GL/glew.h>
#include <Misc/CellarUtils.h>
#include <Graphics/GL/GLToolkit.h>
using namespace cellar;


SkyComponent::SkyComponent(City &city, GLShaderProgram &shader) :
    AbstractComponent(city, shader),
    _skyVao(0),
    _cloudsTex(0),
    _daySkyTex(0),
    _nightSkyTex(0),
    _skyNbStacks(2),
    _skyNbSlices(3)
{
    glGenVertexArrays(1, &_skyVao);

    _cloudsTex = GLToolkit::genTextureId(_city.sky().cloudsImage());

    // Sphere Threading
    int pointsByCircle = _skyNbSlices + 1;

    for(int i=0; i<pointsByCircle * _skyNbStacks/2; ++i)
    {
        _skyIndices.push_back(i + pointsByCircle);
        _skyIndices.push_back(i);
    }
}

SkyComponent::~SkyComponent()
{
    glDeleteVertexArrays(1, &_skyVao);
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

    for(int j=0; j<(_skyNbStacks+1)/2+1; ++j)
    {
        float jadv = static_cast<float>(j) / _skyNbStacks;
        float p = jadv * PI;
        float cosp = cos(p);
        float sinp = sin(p);

        for(int i=0; i<_skyNbSlices+1; ++i)
        {
            float iadv = static_cast<float>(i) / _skyNbSlices;
            float t = iadv * (2.0f*PI);
            float cost = cos(t);
            float sint = sin(t);

            positions.push_back(Vec3f(cost * sinp, sint * sinp, cosp));
        }
    }


    //VAO setup
    glBindVertexArray(_skyVao);

    unsigned int buffers[1];
    glGenBuffers(1, buffers);

    int position_loc = _shader.getAttributeLocation("position_att");

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
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
    glDrawElements(GL_TRIANGLE_STRIP, _skyIndices.size(),
                   GL_UNSIGNED_INT,   _skyIndices.data());
    _shader.popProgram();

    glDepthMask(true);
    glEnable(GL_DEPTH_TEST);
}
