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
    _bridgeBuffs(),
    _bridgeVao(0),    
    _bridgeTex(0),
    _bridgeNbElems(0)
{
    glGenVertexArrays(1, &_bridgeVao);
    glGenBuffers(_BRIDGE_NB_BUFFS, _bridgeBuffs);

    _bridgeTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/bridgeTex.bmp", true)
    );
}

BridgesComponent::~BridgesComponent()
{
    glDeleteVertexArrays(1, &_bridgeVao);
    glDeleteBuffers(_BRIDGE_NB_BUFFS, _bridgeBuffs);
    GLToolkit::deleteTextureId(_bridgeTex);
}

void BridgesComponent::setup()
{
    vector<Vec3f> positions;
    vector<Vec3f> normals;
    vector<Vec2f> texCoords;

    float roadHalfwidth = _description.roadWidth * 0.5f;
    float bridgeHalfWidth = _description.bridgeWidth * 0.5f;
    float diagonal = Vec2f(_description.bridgeWidth, _description.bridgeWidth).length();

    Vec3f corners[4];

    for(size_t b=0; b<_city.bridges().size(); ++b)
    {
        Vec3f endA = vec3<float>(_city.bridges()[b].endA(), _ground.heightAt(_city.bridges()[b].endA()));
        Vec3f endB = vec3<float>(_city.bridges()[b].endB(), _ground.heightAt(_city.bridges()[b].endB()));


        float length = ((endB - endA).length() - 2.0f*bridgeHalfWidth) / _description.bridgeWidth;
        float texDeckRepeat = length;
        float texBracingRepeat = round(length / 4.0f);

        Vec3f dir = (endB - endA).normalize();
        Vec3f perp = vec3(vec2(dir).normalize().rotateQuarterCCW(), 0.0f);
        Vec3f up = (dir ^ perp) * _description.bridgeHeight;

        Vec2f domDirAxe, domDiraxePerp;
        if(absolute(dir.x()) < absolute(dir.y()))
            domDirAxe(0, sign(dir.y()));
        else
            domDirAxe(sign(dir.x()), 0);
        domDiraxePerp = domDirAxe;
        domDiraxePerp.rotateQuarterCCW();


        // Positions
        corners[0] = endA + dir * diagonal + perp * bridgeHalfWidth;
        corners[1] = endA + dir * diagonal - perp * bridgeHalfWidth;
        corners[2] = endB - dir * diagonal - perp * bridgeHalfWidth;
        corners[3] = endB - dir * diagonal + perp * bridgeHalfWidth;



        // Deck
        for(int c=0; c<4; ++c)
            positions.push_back(corners[c]);

        for(int c=0; c<4; ++c)
            normals.push_back(up);

        texCoords.push_back(Vec2f(0.0f,          0.5f));
        texCoords.push_back(Vec2f(0.0f,          0.0f));
        texCoords.push_back(Vec2f(texDeckRepeat, 0.0f));
        texCoords.push_back(Vec2f(texDeckRepeat, 0.5f));



        // Entrance 1
        positions.push_back(endA + vec3(domDirAxe + domDiraxePerp, 0.0f) * roadHalfwidth);
        positions.push_back(endA + vec3(domDirAxe - domDiraxePerp, 0.0f) * roadHalfwidth);
        positions.push_back(corners[1]);
        positions.push_back(corners[0]);

        for(int c=0; c<4; ++c)
            normals.push_back(up);

        texCoords.push_back(Vec2f(0.125f, 0.125f));
        texCoords.push_back(Vec2f(0.125f, 0.125f));
        texCoords.push_back(Vec2f(0.125f, 0.125f));
        texCoords.push_back(Vec2f(0.125f, 0.125f));

        // Entrance 2
        positions.push_back(endB - vec3(domDirAxe + domDiraxePerp, 0.0f) * roadHalfwidth);
        positions.push_back(endB - vec3(domDirAxe - domDiraxePerp, 0.0f) * roadHalfwidth);
        positions.push_back(corners[3]);
        positions.push_back(corners[2]);

        for(int c=0; c<4; ++c)
            normals.push_back(up);

        texCoords.push_back(Vec2f(0.125f, 0.125f));
        texCoords.push_back(Vec2f(0.125f, 0.125f));
        texCoords.push_back(Vec2f(0.125f, 0.125f));
        texCoords.push_back(Vec2f(0.125f, 0.125f));



        // Side 1
        positions.push_back(corners[1]);
        positions.push_back(corners[2]);
        positions.push_back(corners[2] + up);
        positions.push_back(corners[1] + up);

        for(int c=0; c<4; ++c)
            normals.push_back(-perp);

        texCoords.push_back(Vec2f(0.0f,             0.50f));
        texCoords.push_back(Vec2f(texBracingRepeat, 0.50f));
        texCoords.push_back(Vec2f(texBracingRepeat, 0.75f));
        texCoords.push_back(Vec2f(0.0f,             0.75f));

        // Side 2
        positions.push_back(corners[0]);
        positions.push_back(corners[3]);
        positions.push_back(corners[3] + up);
        positions.push_back(corners[0] + up);

        for(int c=0; c<4; ++c)
            normals.push_back(perp);

        texCoords.push_back(Vec2f(0.0f,             0.50f));
        texCoords.push_back(Vec2f(texBracingRepeat, 0.50f));
        texCoords.push_back(Vec2f(texBracingRepeat, 0.75f));
        texCoords.push_back(Vec2f(0.0f,             0.75f));



        // Roof
        for(int c=0; c<4; ++c)
            positions.push_back(corners[c] + up);

        for(int c=0; c<4; ++c)
            normals.push_back(up);

        texCoords.push_back(Vec2f(0.0f,             1.0f));
        texCoords.push_back(Vec2f(0.0f,             0.75f));
        texCoords.push_back(Vec2f(texBracingRepeat, 0.75f));
        texCoords.push_back(Vec2f(texBracingRepeat, 1.0f));
    }

    _bridgeNbElems = positions.size();


    // Setup Vao
    glBindVertexArray( _bridgeVao );

    int position_loc = _shader.getAttributeLocation("position_att");
    int normal_loc   = _shader.getAttributeLocation("normal_att");
    int texCoord_loc = _shader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, _bridgeBuffs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _bridgeBuffs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _bridgeBuffs[2]);
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

    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _bridgeTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _bridgeTex);

    glBindVertexArray(_bridgeVao);
    glDrawArrays(GL_QUADS, 0, _bridgeNbElems);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    _shader.popProgram();
}
