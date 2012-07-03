#include "SkyComponent.h"
#include "DrawCityModule.h"

#include <vector>
using namespace std;

#include <GL/glew.h>
#include <Misc/CellarUtils.h>
#include <Graphics/GL/GLToolkit.h>
#include <MathsAndPhysics/Algorithms.h>
#include <Graphics/Image.h>
using namespace cellar;


SkyComponent::SkyComponent(DrawCityCommonData &common) :
    AbstractComponent(common),
    _skyVao(0),
    _cloudsTex(0),
    _daySkyTex(0),
    _nightSkyTex(0),
    _skyNbStacks(10),
    _skyNbSlices(16)
{
    Grid<float> grid(256, 256, 0.0f);
    perlinNoise(LinearWeighter(0, 100), grid);

    Image nightSky(new unsigned char[grid.width()*grid.height()*4],
                   grid.width(), grid.height(), Image::RGBA);

    for(int j=0; j<grid.height(); ++j)
    {
        for(int i=0; i<grid.width(); ++i)
        {
            float skyDepth = grid.get(i, j);

            if(skyDepth < 0.0f)
                nightSky.setColorAt(i, j, RGBAColor(255, 255, 255, 0));
            else
            {
                unsigned char intensity = clip(255 * 2.0f *  sqrt(skyDepth), 0.0f, 255.0f);
                nightSky.setColorAt(i, j, RGBAColor(255, 255, 255, intensity));
            }
        }
    }

    _cloudsTex = GLToolkit::genTextureId(nightSky);
}

void SkyComponent::setup()
{
    setupSky();
}

void SkyComponent::setupSky()
{
    vector<Vec3f> positions;
    vector<Vec2f> texCoords;

    for(int j=0; j<_skyNbStacks+1; ++j)
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
            texCoords.push_back(Vec2f(cost , sint) * 0.5f * jadv);
        }
    }


    glGenVertexArrays(1, &_skyVao);
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

    for(int i=0; i<pointsByCircle * (3 * _skyNbStacks / 5); ++i)
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
