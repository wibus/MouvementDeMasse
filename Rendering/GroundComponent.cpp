#include "GroundComponent.h"

#include <iostream>
#include <vector>
#include <GL3/gl3w.h>
using namespace std;
#include <Misc/CellarUtils.h>
#include <Graphics/GL/GlToolkit.h>
#include <Graphics/Image/ImageBank.h>
using namespace cellar;


GroundComponent::GroundComponent(City &city, GlProgram &shader) :
    AbstractComponent(city, shader),
    _landsBuffs(),
    _landsVao(0),
    _landsNbElems(0),
    _roadsBuffs(),
    _roadsVao(0),
    _roadsNbElems(0),
    _trianglesBuffs(),
    _trianglesVao(0),
    _trianglesNbElems(0)
{
    glGenVertexArrays(1, &_landsVao);
    glGenBuffers(_LANDS_NB_BUFFS, _landsBuffs);
    glGenVertexArrays(1, &_roadsVao);
    glGenBuffers(_ROADS_NB_BUFFS, _roadsBuffs);
    glGenVertexArrays(1, &_trianglesVao);
    glGenBuffers(_TRIANGLES_NB_BUFFS, _trianglesBuffs);

    _groundTex = GlToolkit::genTextureId(
        getImageBank().getImage("resources/textures/grass.bmp")
    );
}

GroundComponent::~GroundComponent()
{
    glDeleteVertexArrays(1, &_landsVao);
    glDeleteBuffers(_LANDS_NB_BUFFS, _landsBuffs);
    glDeleteVertexArrays(1, &_roadsVao);
    glDeleteBuffers(_ROADS_NB_BUFFS, _roadsBuffs);
    glDeleteVertexArrays(1, &_trianglesVao);
    glDeleteBuffers(_TRIANGLES_NB_BUFFS, _trianglesBuffs);

    GlToolkit::deleteTextureId(_groundTex);
}

void GroundComponent::setup()
{
    setupLands();
    setupRoads();
    setupTriangles();
}

void GroundComponent::setupLands()
{
    vector<Vec3f> positions;
    vector<Vec3f> normals;
    vector<Vec2f> texCoords;

    float roadHalfWidth = _description.roadWidth * 0.5f;

    for(int j=0; j<_city.size().y(); ++j)
    {
        for(int i=0; i<_city.size().x(); ++i)
        {
            if(_city.lands().get(i, j)->type() != Land::GRASS)
                continue;

            if(_city.junctions().get(i, j)->type() != Junction::GRASS)
                positions.push_back(Vec3f(i+roadHalfWidth, j+roadHalfWidth, _ground.heightAt(i, j)));
            else
                positions.push_back(Vec3f(i, j, _ground.heightAt(i, j)));

            if(_city.junctions().get(i+1, j)->type() != Junction::GRASS)
                positions.push_back(Vec3f(i+1-roadHalfWidth, j+roadHalfWidth, _ground.heightAt(i+1, j)));
            else
                positions.push_back(Vec3f(i+1, j, _ground.heightAt(i+1, j)));

            if(_city.junctions().get(i+1, j+1)->type() != Junction::GRASS)
                positions.push_back(Vec3f(i+1-roadHalfWidth, j+1-roadHalfWidth, _ground.heightAt(i+1, j+1)));
            else
                positions.push_back(Vec3f(i+1, j+1, _ground.heightAt(i+1, j+1)));

            if(_city.junctions().get(i+1, j+1)->type() != Junction::GRASS)
                positions.push_back(Vec3f(i+1-roadHalfWidth, j+1-roadHalfWidth, _ground.heightAt(i+1, j+1)));
            else
                positions.push_back(Vec3f(i+1, j+1, _ground.heightAt(i+1, j+1)));

            if(_city.junctions().get(i, j+1)->type() != Junction::GRASS)
                positions.push_back(Vec3f(i+roadHalfWidth, j+1-roadHalfWidth, _ground.heightAt(i, j+1)));
            else
                positions.push_back(Vec3f(i, j+1, _ground.heightAt(i, j+1)));

            if(_city.junctions().get(i, j)->type() != Junction::GRASS)
                positions.push_back(Vec3f(i+roadHalfWidth, j+roadHalfWidth, _ground.heightAt(i, j)));
            else
                positions.push_back(Vec3f(i, j, _ground.heightAt(i, j)));

            texCoords.push_back(Vec2f(0, 0));
            texCoords.push_back(Vec2f(1, 0));
            texCoords.push_back(Vec2f(1, 1));
            texCoords.push_back(Vec2f(1, 1));
            texCoords.push_back(Vec2f(0, 1));
            texCoords.push_back(Vec2f(0, 0));

            normals.push_back(_ground.normalAt(i,   j));
            normals.push_back(_ground.normalAt(i+1, j));
            normals.push_back(_ground.normalAt(i+1, j+1));
            normals.push_back(_ground.normalAt(i+1, j+1));
            normals.push_back(_ground.normalAt(i,   j+1));
            normals.push_back(_ground.normalAt(i,   j));
        }
    }

    _landsNbElems = positions.size();


    // Ground VAO setup
    glBindVertexArray(_landsVao);

    int position_loc = _shader.getAttributeLocation("position_att");
    int normal_loc   = _shader.getAttributeLocation("normal_att");
    int texCoord_loc = _shader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, _landsBuffs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _landsBuffs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _landsBuffs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void GroundComponent::setupRoads()
{
    vector<Vec3f> positions;
    vector<Vec3f> normals;
    vector<Vec2f> texCoords;

    float roadHalfWidth = _description.roadWidth * 0.5f;

    for(int j=0; j<_city.size().y(); ++j)
    {
        for(int i=0; i<_city.size().x(); ++i)
        {
            if( isGrassRoad(i, j, EAST) )
            {
                positions.push_back(Vec3f(i+roadHalfWidth,   j-roadHalfWidth, _ground.heightAt(i,   j)));
                positions.push_back(Vec3f(i+1-roadHalfWidth, j-roadHalfWidth, _ground.heightAt(i+1, j)));
                positions.push_back(Vec3f(i+1-roadHalfWidth, j+roadHalfWidth, _ground.heightAt(i+1, j)));
                positions.push_back(Vec3f(i+1-roadHalfWidth, j+roadHalfWidth, _ground.heightAt(i+1, j)));
                positions.push_back(Vec3f(i+roadHalfWidth,   j+roadHalfWidth, _ground.heightAt(i,   j)));
                positions.push_back(Vec3f(i+roadHalfWidth,   j-roadHalfWidth, _ground.heightAt(i,   j)));

                texCoords.push_back(Vec2f(0, -roadHalfWidth));
                texCoords.push_back(Vec2f(1, -roadHalfWidth));
                texCoords.push_back(Vec2f(1, roadHalfWidth));
                texCoords.push_back(Vec2f(1, roadHalfWidth));
                texCoords.push_back(Vec2f(0, roadHalfWidth));
                texCoords.push_back(Vec2f(0, -roadHalfWidth));

                normals.push_back(_ground.normalAt(i,   j));
                normals.push_back(_ground.normalAt(i+1, j));
                normals.push_back(_ground.normalAt(i+1, j));
                normals.push_back(_ground.normalAt(i+1, j));
                normals.push_back(_ground.normalAt(i,   j));
                normals.push_back(_ground.normalAt(i,   j));
            }

            if( isGrassRoad(i, j, NORTH) )
            {
                positions.push_back(Vec3f(i-roadHalfWidth, j+roadHalfWidth,   _ground.heightAt(i, j)));
                positions.push_back(Vec3f(i+roadHalfWidth, j+roadHalfWidth,   _ground.heightAt(i, j)));
                positions.push_back(Vec3f(i+roadHalfWidth, j+1-roadHalfWidth, _ground.heightAt(i, j+1)));
                positions.push_back(Vec3f(i+roadHalfWidth, j+1-roadHalfWidth, _ground.heightAt(i, j+1)));
                positions.push_back(Vec3f(i-roadHalfWidth, j+1-roadHalfWidth, _ground.heightAt(i, j+1)));
                positions.push_back(Vec3f(i-roadHalfWidth, j+roadHalfWidth,   _ground.heightAt(i, j)));

                texCoords.push_back(Vec2f(-roadHalfWidth, 0));
                texCoords.push_back(Vec2f(roadHalfWidth,  0));
                texCoords.push_back(Vec2f(roadHalfWidth,  1));
                texCoords.push_back(Vec2f(roadHalfWidth,  1));
                texCoords.push_back(Vec2f(-roadHalfWidth, 1));
                texCoords.push_back(Vec2f(-roadHalfWidth, 0));

                normals.push_back(_ground.normalAt(i, j));
                normals.push_back(_ground.normalAt(i, j));
                normals.push_back(_ground.normalAt(i, j+1));
                normals.push_back(_ground.normalAt(i, j+1));
                normals.push_back(_ground.normalAt(i, j+1));
                normals.push_back(_ground.normalAt(i, j));
            }
        }
    }

    _roadsNbElems = positions.size();


    // Ground VAO setup
    glBindVertexArray(_roadsVao);

    int position_loc = _shader.getAttributeLocation("position_att");
    int normal_loc   = _shader.getAttributeLocation("normal_att");
    int texCoord_loc = _shader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, _roadsBuffs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _roadsBuffs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _roadsBuffs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), normals.data(), GL_STATIC_DRAW);
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
    vector<Vec3f> positions;
    vector<Vec3f> normals;
    vector<Vec2f> texCoords;

    float roadHalfWidth = _description.roadWidth * 0.5f;

    // East
    for(int j=0; j<_city.size().y()+1; ++j)
    {
        for(int i=0; i<_city.size().x(); ++i)
        {
            if(_city.junctions().get(i, j)->type() != Junction::GRASS)
            if(_city.junctions().get(i+1, j)->type() == Junction::GRASS)
            {
                float curJuncHeight = _ground.heightAt(i, j);

                positions.push_back(Vec3f(i+roadHalfWidth, j+roadHalfWidth, curJuncHeight));
                positions.push_back(Vec3f(i+roadHalfWidth, j-roadHalfWidth, curJuncHeight));
                positions.push_back(Vec3f(i+1, j, _ground.heightAt(i+1, j)));

                texCoords.push_back(Vec2f(0, -roadHalfWidth));
                texCoords.push_back(Vec2f(0, +roadHalfWidth));
                texCoords.push_back(Vec2f(1 - roadHalfWidth, 0));

                normals.push_back(_ground.normalAt(i,   j));
                normals.push_back(_ground.normalAt(i,   j));
                normals.push_back(_ground.normalAt(i+1, j));
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

                positions.push_back(Vec3f(i-roadHalfWidth, j+roadHalfWidth, curJuncHeight));
                positions.push_back(Vec3f(i+roadHalfWidth, j+roadHalfWidth, curJuncHeight));
                positions.push_back(Vec3f(i, j+1, _ground.heightAt(i, j+1)));

                texCoords.push_back(Vec2f(0, -roadHalfWidth));
                texCoords.push_back(Vec2f(0, +roadHalfWidth));
                texCoords.push_back(Vec2f(1 - roadHalfWidth, 0));

                normals.push_back(_ground.normalAt(i, j));
                normals.push_back(_ground.normalAt(i, j));
                normals.push_back(_ground.normalAt(i, j+1));
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

                positions.push_back(Vec3f(i-roadHalfWidth, j-roadHalfWidth, curJuncHeight));
                positions.push_back(Vec3f(i-roadHalfWidth, j+roadHalfWidth, curJuncHeight));
                positions.push_back(Vec3f(i-1, j, _ground.heightAt(i-1, j)));

                texCoords.push_back(Vec2f(0, -roadHalfWidth));
                texCoords.push_back(Vec2f(0, +roadHalfWidth));
                texCoords.push_back(Vec2f(1 - roadHalfWidth, 0));

                normals.push_back(_ground.normalAt(i,   j));
                normals.push_back(_ground.normalAt(i,   j));
                normals.push_back(_ground.normalAt(i-1, j));
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

                positions.push_back(Vec3f(i+roadHalfWidth, j-roadHalfWidth, curJuncHeight));
                positions.push_back(Vec3f(i-roadHalfWidth, j-roadHalfWidth, curJuncHeight));
                positions.push_back(Vec3f(i, j-1, _ground.heightAt(i, j-1)));

                texCoords.push_back(Vec2f(0, -roadHalfWidth));
                texCoords.push_back(Vec2f(0, +roadHalfWidth));
                texCoords.push_back(Vec2f(1 - roadHalfWidth, 0));

                normals.push_back(_ground.normalAt(i, j));
                normals.push_back(_ground.normalAt(i, j));
                normals.push_back(_ground.normalAt(i, j-1));
            }
        }
    }

    _trianglesNbElems = positions.size();

    // Ground VAO setup
    glBindVertexArray(_trianglesVao);

    int position_loc = _shader.getAttributeLocation("position_att");
    int normal_loc   = _shader.getAttributeLocation("normal_att");
    int texCoord_loc = _shader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, _trianglesBuffs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _trianglesBuffs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords[0]) * texCoords.size(), texCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _trianglesBuffs[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals[0]) * normals.size(), normals.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void GroundComponent::draw()
{
    _shader.pushProgram();

    glBindTexture(GL_TEXTURE_2D, _groundTex);

    glBindVertexArray(_landsVao);
    glDrawArrays(GL_TRIANGLES, 0, _landsNbElems);

    glBindVertexArray(_roadsVao);
    glDrawArrays(GL_TRIANGLES, 0, _roadsNbElems);

    glBindVertexArray(_trianglesVao);
    glDrawArrays(GL_TRIANGLES, 0, _trianglesNbElems);

    _shader.popProgram();
}
