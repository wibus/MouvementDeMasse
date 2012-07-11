#include "BridgesComponent.h"

#include <cmath>
#include <GL/glew.h>
using namespace std;

#include <Misc/CellarUtils.h>
#include <Graphics/GL/GLToolkit.h>
#include <Graphics/ImageBank.h>
using namespace cellar;


BridgesComponent::BridgesComponent(City& city, cellar::GLShaderProgram& shader) :
    AbstractComponent(city, shader),
    _bridgeVao(0),
    _bridgeTex(0),
    _bridgeNbElems(0)
{
    glGenVertexArrays(1, &_bridgeVao);

    _bridgeTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/street.bmp", false)
    );
}

BridgesComponent::~BridgesComponent()
{
    glDeleteVertexArrays(1, &_bridgeVao);
    GLToolkit::deleteTextureId(_bridgeTex);
}

void BridgesComponent::setup()
{
    vector<Vec3f> positions;
    vector<Vec2f> texCoords;

    for(size_t b=0; b<_city.bridges().size(); ++b)
    {
        float bridgeHalfWidth = _visual.bridgeWidth * 0.5f;
        float diagonal = Vec2f(_visual.bridgeWidth, _visual.bridgeWidth).length();

        Vec2f endA = _city.bridges()[b].endA();
        Vec2f endB = _city.bridges()[b].endB();

        float endAHeight = _ground.heightAt(_city.bridges()[b].endA());
        float endBHeight = _ground.heightAt(_city.bridges()[b].endB());

        Vec2f dir = endB - endA;
        float length = dir.length() - 2.0f*bridgeHalfWidth;
        dir.normalize();
        Vec2f perp = dir;
        perp.rotateQuarterCCW();

        Vec3f corners[4];
        corners[0] = vec3(endA + dir * diagonal + perp * bridgeHalfWidth, endAHeight);
        corners[1] = vec3(endA + dir * diagonal - perp * bridgeHalfWidth, endAHeight);
        corners[2] = vec3(endB - dir * diagonal - perp * bridgeHalfWidth, endBHeight);
        corners[3] = vec3(endB - dir * diagonal + perp * bridgeHalfWidth, endBHeight);

        for(int c=0; c<4; ++c)
            positions.push_back(corners[c]);

        texCoords.push_back(Vec2f(0.0f, 2.0f));
        texCoords.push_back(Vec2f(0.0f, 0.0f));
        texCoords.push_back(Vec2f(length, 0.0f));
        texCoords.push_back(Vec2f(length, 2.0f));
    }

    _bridgeNbElems = positions.size();

    // Setup Vao
    glBindVertexArray( _bridgeVao );

    const int NB_BUFFS = 2;
    GLuint buffers[NB_BUFFS];
    glGenBuffers(NB_BUFFS, buffers);

    int position_loc = _shader.getAttributeLocation("position_att");
    int texCoord_loc = _shader.getAttributeLocation("texCoord_att");

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

void BridgesComponent::draw()
{
    _shader.pushThisProgram();

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _bridgeTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _bridgeTex);

    glBindVertexArray(_bridgeVao);
    glDrawArrays(GL_QUADS, 0, _bridgeNbElems);
    glBindVertexArray(0);

    _shader.popProgram();
}
