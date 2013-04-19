#include "BridgesComponent.h"

#include <cmath>
#include <GL3/gl3w.h>
using namespace std;

#include <Misc/CellarUtils.h>
#include <Graphics/GL/GlToolkit.h>
#include <Graphics/Image/ImageBank.h>
using namespace cellar;


BridgesComponent::BridgesComponent(City& city, cellar::GlProgram& shader) :
    AbstractComponent(city, shader),
    _bridgeBuffs(),
    _bridgeVao(0),    
    _bridgeTex(0),
    _bridgeNbElems(0)
{
    glGenVertexArrays(1, &_bridgeVao);
    glGenBuffers(_BRIDGE_NB_BUFFS, _bridgeBuffs);

    _bridgeTex = GlToolkit::genTextureId(
        getImageBank().getImage("resources/textures/bridgeTex.png")
    );
}

BridgesComponent::~BridgesComponent()
{
    glDeleteVertexArrays(1, &_bridgeVao);
    glDeleteBuffers(_BRIDGE_NB_BUFFS, _bridgeBuffs);
    GlToolkit::deleteTextureId(_bridgeTex);
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
        Vec3f endA = Vec3f(_city.bridges()[b].endA(),
                           _ground.heightAt(_city.bridges()[b].endA()));
        Vec3f endB = Vec3f(_city.bridges()[b].endB(),
                           _ground.heightAt(_city.bridges()[b].endB()));


        float length = ((endB - endA).length() - 2.0f*bridgeHalfWidth) / _description.bridgeWidth;
        float texDeckRepeat = length;
        float texBracingRepeat = floor(length / 4.0f + 0.5f);

        Vec3f dir = (endB - endA).normalize();
        Vec3f perp = Vec3f(perpCCW(Vec2f(dir).normalize()), 0.0f);
        Vec3f up = cross(dir, perp) * _description.bridgeHeight;

        Vec2f domDirAxe, domDiraxePerp;
        if(absolute(dir.x()) < absolute(dir.y()))
            domDirAxe(0, sign(dir.y()));
        else
            domDirAxe(sign(dir.x()), 0);
        domDiraxePerp = domDirAxe;
        domDiraxePerp = perpCCW(domDiraxePerp);


        // Positions
        corners[0] = endA + dir * diagonal + perp * bridgeHalfWidth;
        corners[1] = endA + dir * diagonal - perp * bridgeHalfWidth;
        corners[2] = endB - dir * diagonal - perp * bridgeHalfWidth;
        corners[3] = endB - dir * diagonal + perp * bridgeHalfWidth;



        // Deck
        positions.push_back(corners[0]);
        positions.push_back(corners[1]);
        positions.push_back(corners[2]);
        positions.push_back(corners[2]);
        positions.push_back(corners[3]);
        positions.push_back(corners[0]);

        for(int c=0; c<6; ++c)
            normals.push_back(up);

        texCoords.push_back(Vec2f(0.0f,          0.5f));
        texCoords.push_back(Vec2f(0.0f,          0.0f));
        texCoords.push_back(Vec2f(texDeckRepeat, 0.0f));
        texCoords.push_back(Vec2f(texDeckRepeat, 0.0f));
        texCoords.push_back(Vec2f(texDeckRepeat, 0.5f));
        texCoords.push_back(Vec2f(0.0f,          0.5f));



        // Entrance 1
        positions.push_back(endA + Vec3f(domDirAxe + domDiraxePerp, 0.0f) * roadHalfwidth);
        positions.push_back(endA + Vec3f(domDirAxe - domDiraxePerp, 0.0f) * roadHalfwidth);
        positions.push_back(corners[1]);
        positions.push_back(corners[0]);
        positions.push_back(corners[1]);
        positions.push_back(endA + Vec3f(domDirAxe + domDiraxePerp, 0.0f) * roadHalfwidth);

        for(int c=0; c<6; ++c)
            normals.push_back(up);

        texCoords.push_back(Vec2f(0.125f, 0.125f));
        texCoords.push_back(Vec2f(0.125f, 0.125f));
        texCoords.push_back(Vec2f(0.125f, 0.125f));
        texCoords.push_back(Vec2f(0.125f, 0.125f));
        texCoords.push_back(Vec2f(0.125f, 0.125f));
        texCoords.push_back(Vec2f(0.125f, 0.125f));

        // Entrance 2
        positions.push_back(endB - Vec3f(domDirAxe + domDiraxePerp, 0.0f) * roadHalfwidth);
        positions.push_back(endB - Vec3f(domDirAxe - domDiraxePerp, 0.0f) * roadHalfwidth);
        positions.push_back(corners[3]);
        positions.push_back(corners[3]);
        positions.push_back(corners[2]);
        positions.push_back(endB - Vec3f(domDirAxe + domDiraxePerp, 0.0f) * roadHalfwidth);

        for(int c=0; c<6; ++c)
            normals.push_back(up);

        texCoords.push_back(Vec2f(0.125f, 0.125f));
        texCoords.push_back(Vec2f(0.125f, 0.125f));
        texCoords.push_back(Vec2f(0.125f, 0.125f));
        texCoords.push_back(Vec2f(0.125f, 0.125f));
        texCoords.push_back(Vec2f(0.125f, 0.125f));
        texCoords.push_back(Vec2f(0.125f, 0.125f));



        // Side 1
        positions.push_back(corners[1]);
        positions.push_back(corners[2]);
        positions.push_back(corners[2] + up);
        positions.push_back(corners[2] + up);
        positions.push_back(corners[1] + up);
        positions.push_back(corners[1]);

        for(int c=0; c<6; ++c)
            normals.push_back(-perp);

        texCoords.push_back(Vec2f(0.0f,             0.50f));
        texCoords.push_back(Vec2f(texBracingRepeat, 0.50f));
        texCoords.push_back(Vec2f(texBracingRepeat, 0.75f));
        texCoords.push_back(Vec2f(texBracingRepeat, 0.75f));
        texCoords.push_back(Vec2f(0.0f,             0.75f));
        texCoords.push_back(Vec2f(0.0f,             0.50f));

        // Side 2
        positions.push_back(corners[0]);
        positions.push_back(corners[3]);
        positions.push_back(corners[3] + up);
        positions.push_back(corners[3] + up);
        positions.push_back(corners[0] + up);
        positions.push_back(corners[0]);

        for(int c=0; c<6; ++c)
            normals.push_back(perp);

        texCoords.push_back(Vec2f(0.0f,             0.50f));
        texCoords.push_back(Vec2f(texBracingRepeat, 0.50f));
        texCoords.push_back(Vec2f(texBracingRepeat, 0.75f));
        texCoords.push_back(Vec2f(texBracingRepeat, 0.75f));
        texCoords.push_back(Vec2f(0.0f,             0.75f));
        texCoords.push_back(Vec2f(0.0f,             0.50f));



        // Roof
        positions.push_back(corners[0] + up);
        positions.push_back(corners[1] + up);
        positions.push_back(corners[2] + up);
        positions.push_back(corners[2] + up);
        positions.push_back(corners[3] + up);
        positions.push_back(corners[0] + up);

        for(int c=0; c<6; ++c)
            normals.push_back(up);

        texCoords.push_back(Vec2f(0.0f,             1.0f));
        texCoords.push_back(Vec2f(0.0f,             0.75f));
        texCoords.push_back(Vec2f(texBracingRepeat, 0.75f));
        texCoords.push_back(Vec2f(texBracingRepeat, 0.75f));
        texCoords.push_back(Vec2f(texBracingRepeat, 1.0f));
        texCoords.push_back(Vec2f(0.0f,             1.0f));
    }

    _bridgeNbElems = static_cast<int>(positions.size());


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
    _shader.pushProgram();

    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _bridgeTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _bridgeTex);

    glBindVertexArray(_bridgeVao);
    glDrawArrays(GL_TRIANGLES, 0, _bridgeNbElems);
    glBindVertexArray(0);

    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    _shader.popProgram();
}
