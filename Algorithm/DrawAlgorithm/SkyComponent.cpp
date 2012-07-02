#include "SkyComponent.h"
#include "DrawCityModule.h"

#include <vector>
using namespace std;

#include <GL/glew.h>
#include <Misc/CellarUtils.h>
#include <Graphics/GL/GLToolkit.h>
#include <Graphics/ImageBank.h>
using namespace cellar;


SkyComponent::SkyComponent(DrawCityCommonData &common) :
    AbstractComponent(common),
    _skyCoefCorrection(0.1),
    _skyRadius(1.0f),
    _skyVao(0),
    _skyTopVao(0),
    _daySkyTex(0),
    _nightSkyTex(0),
    _skyNbStacks(16),
    _skyNbSlices(24),
    _skyNbElems(0),
    _skyTopNbElems(0)
{
    /*
    _daySkyTex = GLToolkit::genTextureId(getImageBank().getImage("daySkyTex.bmp", false));
    _nightSkyTex = GLToolkit::genTextureId(getImageBank().getImage("nightSkyTex.bmp", false));
    */

    _skyNbElems = _skyNbStacks * _skyNbSlices;
    _skyTopNbElems = _skyNbSlices + 2;
}

void SkyComponent::setup()
{
    setupSky();
    setupSkyTop();
}

void SkyComponent::setupSky()
{
}

void SkyComponent::setupSkyTop()
{
    vector<Vec3f> positions;
    vector<Vec2f> texCoords;

    positions.push_back(Vec3f(0.0f, 0.0f, _skyRadius));
    texCoords.push_back(Vec2f(0.5f, 1.0f));

    float firstStackHeight = cos(PI*1.0f/_skyNbStacks);
    float firstStackTCoord = 1.0f / _skyNbStacks;
    float firstStackTWidth = sin(PI*1.0f/_skyNbStacks);
    float fisrtStackTStart = 0.5f - 0.5f*firstStackTWidth;

    for(int i=0; i<_skyNbSlices+1; ++i)
    {
        float advance = static_cast<float>(i)/_skyNbSlices;
        float angle = 2*PI*advance;
        positions.push_back(Vec3f(cos(angle), sin(-angle), firstStackHeight) * _skyRadius);
        texCoords.push_back(Vec2f(fisrtStackTStart + advance*firstStackTWidth, firstStackTCoord));
    }


    glGenVertexArrays(1, &_skyTopVao);
    glBindVertexArray(_skyTopVao);

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
}

void SkyComponent::draw()
{
    _common.skyShader.pushThisProgram();
    glBindVertexArray(_skyTopVao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, _skyTopNbElems);
    _common.skyShader.popProgram();
}
