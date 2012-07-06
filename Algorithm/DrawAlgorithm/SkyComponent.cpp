#include "SkyComponent.h"
#include "DrawCityModule.h"

#include <vector>
using namespace std;

#include <GL/glew.h>
#include <Misc/CellarUtils.h>
#include <Graphics/GL/GLToolkit.h>
using namespace cellar;


SkyComponent::SkyComponent(DrawCityCommonData &common) :
    AbstractComponent(common),
    _skyVao(0),
    _cloudsTex(0),
    _daySkyTex(0),
    _nightSkyTex(0),
    _skyNbStacks(20),
    _skyNbSlices(20)
{
    glGenVertexArrays(1, &_skyVao);

    _cloudsTex = GLToolkit::genTextureId(_common.city.sky().cloudsImage());
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
    vector<Vec2f> texCoords;

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
            texCoords.push_back(Vec2f(cost , sint) * 0.2f * tan(jadv*2.8f));
        }
    }


    //VAO setup
    glBindVertexArray(_skyVao);

    unsigned int buffers[2];
    glGenBuffers(2, buffers);

    int position_loc = _common.skyShader.getAttributeLocation("position_att");
    int texCoord_loc = _common.skyShader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );


    // Sphere Threading
    int pointsByCircle = _skyNbSlices + 1;

    for(int i=0; i<pointsByCircle * _skyNbStacks/2; ++i)
    {
        _skyIndices.push_back(i + pointsByCircle);
        _skyIndices.push_back(i);
    }
}

void SkyComponent::draw()
{
    glDisable(GL_DEPTH_TEST);
    glDepthMask(false);

    _common.skyShader.pushThisProgram();
    glBindVertexArray(_skyVao);
    glBindTexture(GL_TEXTURE_2D, _cloudsTex);
    glDrawElements(GL_TRIANGLE_STRIP, _skyIndices.size(),
                   GL_UNSIGNED_INT,   _skyIndices.data());
    _common.skyShader.popProgram();

    glDepthMask(true);
    glEnable(GL_DEPTH_TEST);
}
