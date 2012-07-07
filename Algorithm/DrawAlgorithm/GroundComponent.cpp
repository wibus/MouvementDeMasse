#include "GroundComponent.h"

#include <iostream>
#include <vector>
#include <GL/glew.h>
using namespace std;
#include <Misc/CellarUtils.h>
#include <Graphics/GL/GLToolkit.h>
#include <Graphics/ImageBank.h>
using namespace cellar;


GroundComponent::GroundComponent(City &city, GLShaderProgram &shader) :
    AbstractComponent(city, shader),
    _landsVao(0),
    _landsNbElems(0),
    _roadsVao(0),
    _roadsNbElems(0),
    _trianglesVao(0),
    _trianglesNbElems(0)
{
    glGenVertexArrays(1, &_landsVao);
    glGenVertexArrays(1, &_roadsVao);
    glGenVertexArrays(1, &_trianglesVao);

    _groundTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/grass.bmp", false)
    );
}

GroundComponent::~GroundComponent()
{
    glDeleteVertexArrays(1, &_landsVao);
    glDeleteVertexArrays(1, &_roadsVao);
    glDeleteVertexArrays(1, &_trianglesVao);

    GLToolkit::deleteTextureId(_groundTex);
}

void GroundComponent::setup()
{
    setupLands();
    setupRoads();
    setupTriangles();
}

void GroundComponent::setupLands()
{
    vector<Vec3f> lpositions;
    vector<Vec3f> lnormals;
    vector<Vec2f> ltexCoords;

    for(int j=0; j<_city.size().y(); ++j)
    {
        for(int i=0; i<_city.size().x(); ++i)
        {
            if(_city.lands().get(i, j)->type() != Land::GRASS)
                continue;

            if(_city.junctions().get(i, j)->type() != Junction::GRASS)
                lpositions.push_back(Vec3f(i+_visual.roadWidth, j+_visual.roadWidth, _ground.heightAt(i, j)));
            else
                lpositions.push_back(Vec3f(i, j, _ground.heightAt(i, j)));

            if(_city.junctions().get(i+1, j)->type() != Junction::GRASS)
                lpositions.push_back(Vec3f(i+1-_visual.roadWidth, j+_visual.roadWidth, _ground.heightAt(i+1, j)));
            else
                lpositions.push_back(Vec3f(i+1, j, _ground.heightAt(i+1, j)));

            if(_city.junctions().get(i+1, j+1)->type() != Junction::GRASS)
                lpositions.push_back(Vec3f(i+1-_visual.roadWidth, j+1-_visual.roadWidth, _ground.heightAt(i+1, j+1)));
            else
                lpositions.push_back(Vec3f(i+1, j+1, _ground.heightAt(i+1, j+1)));

            if(_city.junctions().get(i, j+1)->type() != Junction::GRASS)
                lpositions.push_back(Vec3f(i+_visual.roadWidth, j+1-_visual.roadWidth, _ground.heightAt(i, j+1)));
            else
                lpositions.push_back(Vec3f(i, j+1, _ground.heightAt(i, j+1)));

            ltexCoords.push_back(Vec2f(0, 0));
            ltexCoords.push_back(Vec2f(1, 0));
            ltexCoords.push_back(Vec2f(1, 1));
            ltexCoords.push_back(Vec2f(0, 1));

            lnormals.push_back(_ground.normalAt(i,   j));
            lnormals.push_back(_ground.normalAt(i+1, j));
            lnormals.push_back(_ground.normalAt(i+1, j+1));
            lnormals.push_back(_ground.normalAt(i,   j+1));
        }
    }

    _landsNbElems = lpositions.size();


    // Ground VAO setup
    glBindVertexArray(_landsVao);

    const int nbAttributes = 3;
    GLuint gBuffers[nbAttributes];
    glGenBuffers(nbAttributes, gBuffers);

    int position_loc = _shader.getAttributeLocation("position_att");
    int normal_loc   = _shader.getAttributeLocation("normal_att");
    int texCoord_loc = _shader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lpositions[0]) * lpositions.size(), lpositions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ltexCoords[0]) * ltexCoords.size(), ltexCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(lnormals[0]) * lnormals.size(), lnormals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void GroundComponent::setupRoads()
{
    vector<Vec3f> rpositions;
    vector<Vec3f> rnormals;
    vector<Vec2f> rtexCoords;
    int idx = 0;

    for(int j=0; j<_city.size().y(); ++j)
    {
        for(int i=0; i<_city.size().x(); ++i)
        {
            if( isGrassRoad(i, j, EAST) )
            {
                rpositions.push_back(Vec3f(i+_visual.roadWidth,   j-_visual.roadWidth, _ground.heightAt(i,   j)));
                rpositions.push_back(Vec3f(i+1-_visual.roadWidth, j-_visual.roadWidth, _ground.heightAt(i+1, j)));
                rpositions.push_back(Vec3f(i+1-_visual.roadWidth, j+_visual.roadWidth, _ground.heightAt(i+1, j)));
                rpositions.push_back(Vec3f(i+_visual.roadWidth,   j+_visual.roadWidth, _ground.heightAt(i,   j)));

                rtexCoords.push_back(Vec2f(0, -_visual.roadWidth));
                rtexCoords.push_back(Vec2f(1, -_visual.roadWidth));
                rtexCoords.push_back(Vec2f(1, _visual.roadWidth));
                rtexCoords.push_back(Vec2f(0, _visual.roadWidth));

                rnormals.push_back(_ground.normalAt(i,   j));
                rnormals.push_back(_ground.normalAt(i+1, j));
                rnormals.push_back(_ground.normalAt(i+1, j));
                rnormals.push_back(_ground.normalAt(i,   j));

                idx += 4;
            }

            if( isGrassRoad(i, j, NORTH) )
            {
                rpositions.push_back(Vec3f(i-_visual.roadWidth, j+_visual.roadWidth,   _ground.heightAt(i, j)));
                rpositions.push_back(Vec3f(i+_visual.roadWidth, j+_visual.roadWidth,   _ground.heightAt(i, j)));
                rpositions.push_back(Vec3f(i+_visual.roadWidth, j+1-_visual.roadWidth, _ground.heightAt(i, j+1)));
                rpositions.push_back(Vec3f(i-_visual.roadWidth, j+1-_visual.roadWidth, _ground.heightAt(i, j+1)));

                rtexCoords.push_back(Vec2f(-_visual.roadWidth, 0));
                rtexCoords.push_back(Vec2f(_visual.roadWidth,  0));
                rtexCoords.push_back(Vec2f(_visual.roadWidth,  1));
                rtexCoords.push_back(Vec2f(-_visual.roadWidth, 1));

                rnormals.push_back(_ground.normalAt(i, j));
                rnormals.push_back(_ground.normalAt(i, j));
                rnormals.push_back(_ground.normalAt(i, j+1));
                rnormals.push_back(_ground.normalAt(i, j+1));

                idx += 4;
            }
        }
    }

    _roadsNbElems = rpositions.size();


    // Ground VAO setup
    glBindVertexArray(_roadsVao);

    const int nbAttributes = 3;
    GLuint gBuffers[nbAttributes];
    glGenBuffers(nbAttributes, gBuffers);

    int position_loc = _shader.getAttributeLocation("position_att");
    int normal_loc   = _shader.getAttributeLocation("normal_att");
    int texCoord_loc = _shader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rpositions[0]) * rpositions.size(), rpositions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rtexCoords[0]) * rtexCoords.size(), rtexCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rnormals[0]) * rnormals.size(), rnormals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

bool GroundComponent::isGrassRoad(int i, int j, CardinalDirection dir)
{
    Vec2i vecDir = toVec(dir);

    if(_city.junctions().get(i, j)->type() != Junction::GRASS)
        if(_city.junctions().get(i + vecDir.x(), j + vecDir.y())->type() != Junction::GRASS)
            if(_city.junctions().get(i, j)->getStreet(dir) == 0x0)
                return true;

    return false;
}

void GroundComponent::setupTriangles()
{
    vector<Vec3f> tpositions;
    vector<Vec3f> tnormals;
    vector<Vec2f> ttexCoords;

    // East
    for(int j=0; j<_city.size().y()+1; ++j)
    {
        for(int i=0; i<_city.size().x(); ++i)
        {
            if(_city.junctions().get(i, j)->type() != Junction::GRASS)
            if(_city.junctions().get(i+1, j)->type() == Junction::GRASS)
            {
                float curJuncHeight = _ground.heightAt(i, j);

                tpositions.push_back(Vec3f(i+_visual.roadWidth, j+_visual.roadWidth, curJuncHeight));
                tpositions.push_back(Vec3f(i+_visual.roadWidth, j-_visual.roadWidth, curJuncHeight));
                tpositions.push_back(Vec3f(i+1, j, _ground.heightAt(i+1, j)));

                ttexCoords.push_back(Vec2f(0, -_visual.roadWidth));
                ttexCoords.push_back(Vec2f(0, +_visual.roadWidth));
                ttexCoords.push_back(Vec2f(1 - _visual.roadWidth, 0));

                tnormals.push_back(_ground.normalAt(i,   j));
                tnormals.push_back(_ground.normalAt(i,   j));
                tnormals.push_back(_ground.normalAt(i+1, j));
            }
        }
    }

    // North
    for(int j=0; j<_city.size().y(); ++j)
    {
        for(int i=0; i<_city.size().x()+1; ++i)
        {
            if(_city.junctions().get(i, j)->type() != Junction::GRASS)
            if(_city.junctions().get(i, j+1)->type() == Junction::GRASS)
            {
                float curJuncHeight = _ground.heightAt(i, j);

                tpositions.push_back(Vec3f(i-_visual.roadWidth, j+_visual.roadWidth, curJuncHeight));
                tpositions.push_back(Vec3f(i+_visual.roadWidth, j+_visual.roadWidth, curJuncHeight));
                tpositions.push_back(Vec3f(i, j+1, _ground.heightAt(i, j+1)));

                ttexCoords.push_back(Vec2f(0, -_visual.roadWidth));
                ttexCoords.push_back(Vec2f(0, +_visual.roadWidth));
                ttexCoords.push_back(Vec2f(1 - _visual.roadWidth, 0));

                tnormals.push_back(_ground.normalAt(i, j));
                tnormals.push_back(_ground.normalAt(i, j));
                tnormals.push_back(_ground.normalAt(i, j+1));
            }
        }
    }

    // West
    for(int j=0; j<_city.size().y()+1; ++j)
    {
        for(int i=1; i<_city.size().x()+1; ++i)
        {
            if(_city.junctions().get(i, j)->type() != Junction::GRASS)
            if(_city.junctions().get(i-1, j)->type() == Junction::GRASS)
            {
                float curJuncHeight = _ground.heightAt(i, j);

                tpositions.push_back(Vec3f(i-_visual.roadWidth, j-_visual.roadWidth, curJuncHeight));
                tpositions.push_back(Vec3f(i-_visual.roadWidth, j+_visual.roadWidth, curJuncHeight));
                tpositions.push_back(Vec3f(i-1, j, _ground.heightAt(i-1, j)));

                ttexCoords.push_back(Vec2f(0, -_visual.roadWidth));
                ttexCoords.push_back(Vec2f(0, +_visual.roadWidth));
                ttexCoords.push_back(Vec2f(1 - _visual.roadWidth, 0));

                tnormals.push_back(_ground.normalAt(i,   j));
                tnormals.push_back(_ground.normalAt(i,   j));
                tnormals.push_back(_ground.normalAt(i-1, j));
            }
        }
    }

    // South
    for(int j=1; j<_city.size().y()+1; ++j)
    {
        for(int i=0; i<_city.size().x()+1; ++i)
        {
            if(_city.junctions().get(i, j)->type() != Junction::GRASS)
            if(_city.junctions().get(i, j-1)->type() == Junction::GRASS)
            {
                float curJuncHeight = _ground.heightAt(i, j);

                tpositions.push_back(Vec3f(i+_visual.roadWidth, j-_visual.roadWidth, curJuncHeight));
                tpositions.push_back(Vec3f(i-_visual.roadWidth, j-_visual.roadWidth, curJuncHeight));
                tpositions.push_back(Vec3f(i, j-1, _ground.heightAt(i, j-1)));

                ttexCoords.push_back(Vec2f(0, -_visual.roadWidth));
                ttexCoords.push_back(Vec2f(0, +_visual.roadWidth));
                ttexCoords.push_back(Vec2f(1 - _visual.roadWidth, 0));

                tnormals.push_back(_ground.normalAt(i, j));
                tnormals.push_back(_ground.normalAt(i, j));
                tnormals.push_back(_ground.normalAt(i, j-1));
            }
        }
    }

    _trianglesNbElems = tpositions.size();

    // Ground VAO setup
    glBindVertexArray(_trianglesVao);

    const int nbAttributes = 3;
    GLuint gBuffers[nbAttributes];
    glGenBuffers(nbAttributes, gBuffers);

    int position_loc = _shader.getAttributeLocation("position_att");
    int normal_loc   = _shader.getAttributeLocation("normal_att");
    int texCoord_loc = _shader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tpositions[0]) * tpositions.size(), tpositions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ttexCoords[0]) * ttexCoords.size(), ttexCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(tnormals[0]) * tnormals.size(), tnormals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void GroundComponent::draw()
{
    _shader.pushThisProgram();

    glBindTexture(GL_TEXTURE_2D, _groundTex);

    glBindVertexArray(_landsVao);
    glDrawArrays(GL_QUADS, 0, _landsNbElems);

    glBindVertexArray(_roadsVao);
    glDrawArrays(GL_QUADS, 0, _roadsNbElems);

    glBindVertexArray(_trianglesVao);
    glDrawArrays(GL_TRIANGLES, 0, _trianglesNbElems);

    _shader.popProgram();
}
