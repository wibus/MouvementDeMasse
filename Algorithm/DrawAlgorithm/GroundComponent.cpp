#include "GroundComponent.h"
#include "DrawCityModule.h"
#include "Land/Land.h"

#include <GL/glew.h>
using namespace std;
#include <Misc/CellarUtils.h>
#include <Graphics/GL/GLToolkit.h>
#include <Graphics/ImageBank.h>
using namespace cellar;


GroundComponent::GroundComponent(DrawCityCommonData& common) :
    _common(common),
    _groundVao(0),
    _groundNbElems(0),
    _landsVao(0),
    _landsNbElems(0),
    _roadsVao(0),
    _roadsNbElems(0),
    _trianglesVao(0),
    _trianglesNbElems(0)
{
    _common.groundShader.pushThisProgram();
    _common.groundShader.setVec4f("sun.direction", _common.sunLight.direction);
    _common.groundShader.setVec4f("sun.ambient",   _common.sunLight.ambient);
    _common.groundShader.setVec4f("sun.diffuse",   _common.sunLight.diffuse);
    _common.groundShader.setVec4f("sun.specular",  _common.sunLight.specular);
    _common.groundShader.setFloat("Shininess",     _common.groundShininess);
    _common.groundShader.setFloat("WaterHeight",   _common.ground.waterHeight());
    _common.groundShader.setVec4f("GrassColor",    _common.grassColor);
    _common.groundShader.setVec4f("MudColor",      _common.mudColor);
    _common.groundShader.setVec4f("WaterColor",    _common.waterColor);    
    _common.groundShader.popProgram();
}

void GroundComponent::setup()
{
    setupLands();
    setupRoads();
    setupTriangles();

    _groundTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/grass.bmp", false)
    );

    /*

    /// Algorithm (pseudo code)
    /// ->
    /// idx = -1
    /// for j=0 to height-1
    /// {
    ///     vertex[++idx] = get(0, j)
    ///     for i=0 to width
    ///     {
    ///         vertex[++idx] = get(i, j)
    ///         vertex[++idx] = get(i, j+1)
    ///     }
    ///     vertex[++idx] = get(i, j+1)
    /// }
    /// <-
    /// nbElems = ((width + 1) * 2) * (height - 1)

    int gNbRows = _common.cityMap.size().y();
    int gElemByRow = (_common.cityMap.size().x() + 2) * 2;
    _groundNbElems = gElemByRow * gNbRows;
    Vec2f* gtexCoords = new Vec2f[_groundNbElems];
    Vec3f* gnormals   = new Vec3f[_groundNbElems];
    Vec3f* gpositions = new Vec3f[_groundNbElems];


    int idx = -1;
    for(int j=0; j < gNbRows; ++j)
    {
        computeGroundVertex(idx, gpositions, gnormals, gtexCoords, 0, j);

        for(int i=0; i <= _common.cityMap.size().x(); ++i)
        {
            computeGroundVertex(idx, gpositions, gnormals, gtexCoords, i, j);
            computeGroundVertex(idx, gpositions, gnormals, gtexCoords, i, j+1);
        }

        int lastx = _common.cityMap.size().x();
        computeGroundVertex(idx, gpositions, gnormals, gtexCoords, lastx, j+1);
    }


    // Ground VAO setup
    glGenVertexArrays(1, &_groundVao);
    glBindVertexArray(_groundVao);

    const int nbAttributes = 3;
    GLuint gBuffers[nbAttributes];
    glGenBuffers(nbAttributes, gBuffers);

    int position_loc = _common.groundShader.getAttributeLocation("position_att");
    int normal_loc = _common.groundShader.getAttributeLocation("normal_att");
    int texCoord_loc = _common.groundShader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*gnormals) * _groundNbElems, gnormals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*gpositions) * _groundNbElems, gpositions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*gtexCoords) * _groundNbElems, gtexCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );


    // Arrays sweaping
    delete [] gtexCoords;
    delete [] gnormals;
    delete [] gpositions;


    */
}

/*
void GroundComponent::computeGroundVertex(int& idx, cellar::Vec3f* pos, cellar::Vec3f* norm,
                                          cellar::Vec2f* tex, int i, int j)
{
    ++idx;
    norm[idx] = _common.ground.normalAt(i, j);
    pos[idx]  = Vec3f(i, j, _common.ground.heightAt(i, j));
    tex[idx]  = Vec2f(i, j);
}
*/

void GroundComponent::setupLands()
{

    _landsNbElems = 0;
    for(int j=0; j<_common.cityMap.size().y(); ++j)
        for(int i=0; i<_common.cityMap.size().x(); ++i)
            if(_common.cityMap.lands().get(i, j)->type() == Land::GRASS)
                _landsNbElems += 4;

    Vec2f* texCoords = new Vec2f[_landsNbElems];
    Vec3f* normals   = new Vec3f[_landsNbElems];
    Vec3f* positions = new Vec3f[_landsNbElems];


    int idx = 0;
    for(int j=0; j<_common.cityMap.size().y(); ++j)
    {
        for(int i=0; i<_common.cityMap.size().x(); ++i)
        {
            if(_common.cityMap.lands().get(i, j)->type() != Land::GRASS)
                continue;

            if(_common.cityMap.junctions().get(i, j)->type() != Junction::GRASS)
                positions[idx](i+_common.roadWidth, j+_common.roadWidth, _common.ground.heightAt(i, j));
            else
                positions[idx](i, j, _common.ground.heightAt(i, j));

            if(_common.cityMap.junctions().get(i+1, j)->type() != Junction::GRASS)
                positions[idx+1](i+1-_common.roadWidth, j+_common.roadWidth, _common.ground.heightAt(i+1, j));
            else
                positions[idx+1](i+1, j, _common.ground.heightAt(i+1, j));

            if(_common.cityMap.junctions().get(i+1, j+1)->type() != Junction::GRASS)
                positions[idx+2](i+1-_common.roadWidth, j+1-_common.roadWidth, _common.ground.heightAt(i+1, j+1));
            else
                positions[idx+2](i+1, j+1, _common.ground.heightAt(i+1, j+1));

            if(_common.cityMap.junctions().get(i, j+1)->type() != Junction::GRASS)
                positions[idx+3](i+_common.roadWidth, j+1-_common.roadWidth, _common.ground.heightAt(i, j+1));
            else
                positions[idx+3](i, j+1, _common.ground.heightAt(i, j+1));

            texCoords[idx] = Vec2f(0, 0);
            texCoords[idx+1] = Vec2f(1, 0);
            texCoords[idx+2] = Vec2f(1, 1);
            texCoords[idx+3] = Vec2f(0, 1);

            normals[idx] = _common.ground.normalAt(i,   j);
            normals[idx+1] = _common.ground.normalAt(i+1, j);
            normals[idx+2] = _common.ground.normalAt(i+1, j+1);
            normals[idx+3] = _common.ground.normalAt(i,   j+1);

            idx += 4;
        }
    }

    // Ground VAO setup
    glGenVertexArrays(1, &_landsVao);
    glBindVertexArray(_landsVao);

    const int nbAttributes = 3;
    GLuint gBuffers[nbAttributes];
    glGenBuffers(nbAttributes, gBuffers);

    int position_loc = _common.groundShader.getAttributeLocation("position_att");
    int normal_loc = _common.groundShader.getAttributeLocation("normal_att");
    int texCoord_loc = _common.groundShader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*positions) * _landsNbElems, positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*texCoords) * _landsNbElems, texCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*normals) * _landsNbElems, normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );


    // Release resources
    delete [] texCoords;
    delete [] normals;
    delete [] positions;
}

void GroundComponent::setupRoads()
{
    _roadsNbElems = 0;

    for(int j=0; j<_common.cityMap.size().y(); ++j)
    {
        for(int i=0; i<_common.cityMap.size().x(); ++i)
        {
            if( isGrassRoad(i, j, EAST) )
                _roadsNbElems += 4;

            if( isGrassRoad(i, j, NORTH) )
                _roadsNbElems += 4;
        }
    }

    Vec2f* texCoords = new Vec2f[_roadsNbElems];
    Vec3f* normals   = new Vec3f[_roadsNbElems];
    Vec3f* positions = new Vec3f[_roadsNbElems];

    int idx = 0;
    for(int j=0; j<_common.cityMap.size().y(); ++j)
    {
        for(int i=0; i<_common.cityMap.size().x(); ++i)
        {
            if( isGrassRoad(i, j, EAST) )
            {
                positions[idx](i-_common.roadWidth, j+_common.roadWidth, _common.ground.heightAt(i, j));
                positions[idx+1](i+_common.roadWidth, j+_common.roadWidth, _common.ground.heightAt(i, j));
                positions[idx+2](i+_common.roadWidth, j+1-_common.roadWidth, _common.ground.heightAt(i, j+1));
                positions[idx+3](i-_common.roadWidth, j+1-_common.roadWidth, _common.ground.heightAt(i, j+1));

                texCoords[idx]   = Vec2f(0, 0);
                texCoords[idx+1] = Vec2f(1, 0);
                texCoords[idx+2] = Vec2f(1, 1);
                texCoords[idx+3] = Vec2f(0, 1);

                (normals[idx]   = _common.ground.normalAt(i,   j)).setY(0);
                (normals[idx+1] = _common.ground.normalAt(i+1, j)).setY(0);
                (normals[idx+2] = _common.ground.normalAt(i+1, j+1)).setY(0);
                (normals[idx+3] = _common.ground.normalAt(i,   j+1)).setY(0);

                idx += 4;
            }

            if( isGrassRoad(i, j, NORTH) )
            {
                positions[idx]  (i+_common.roadWidth,   j-_common.roadWidth, _common.ground.heightAt(i, j));
                positions[idx+1](i+1-_common.roadWidth, j-_common.roadWidth, _common.ground.heightAt(i+1, j));
                positions[idx+2](i+1-_common.roadWidth, j+_common.roadWidth, _common.ground.heightAt(i+1, j));
                positions[idx+3](i+_common.roadWidth,   j+_common.roadWidth, _common.ground.heightAt(i, j));

                texCoords[idx]   = Vec2f(0, 0);
                texCoords[idx+1] = Vec2f(1, 0);
                texCoords[idx+2] = Vec2f(1, 1);
                texCoords[idx+3] = Vec2f(0, 1);

                (normals[idx]   = _common.ground.normalAt(i,   j)).setX(0);
                (normals[idx+1] = _common.ground.normalAt(i+1, j)).setX(0);
                (normals[idx+2] = _common.ground.normalAt(i+1, j+1)).setX(0);
                (normals[idx+3] = _common.ground.normalAt(i,   j+1)).setX(0);

                idx += 4;
            }
        }
    }

    // Ground VAO setup
    glGenVertexArrays(1, &_roadsVao);
    glBindVertexArray(_roadsVao);

    const int nbAttributes = 3;
    GLuint gBuffers[nbAttributes];
    glGenBuffers(nbAttributes, gBuffers);

    int position_loc = _common.groundShader.getAttributeLocation("position_att");
    int normal_loc = _common.groundShader.getAttributeLocation("normal_att");
    int texCoord_loc = _common.groundShader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*positions) * _roadsNbElems, positions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*texCoords) * _roadsNbElems, texCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*normals) * _roadsNbElems, normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );


    // Release resources
    delete [] texCoords;
    delete [] normals;
    delete [] positions;
}

bool GroundComponent::isGrassRoad(int i, int j, CardinalDirection dir)
{
    if(_common.cityMap.junctions().get(i,  j)->type() != Junction::GRASS &&
       _common.cityMap.junctions().get(i+1,j)->type() != Junction::GRASS &&
       _common.cityMap.junctions().get(i,  j)->getStreet(dir) != 0x0)
        return true;

    return false;
}

void GroundComponent::setupTriangles()
{
}

void GroundComponent::draw()
{
    _common.groundShader.pushThisProgram();

    glBindTexture(GL_TEXTURE_2D, _groundTex);

    glBindVertexArray(_landsVao);
    glDrawArrays(GL_QUADS, 0, _landsNbElems);

    glBindVertexArray(_roadsVao);
    glDrawArrays(GL_QUADS, 0, _roadsNbElems);

    glBindVertexArray(_trianglesVao);
    glDrawArrays(GL_TRIANGLES, 0, _trianglesNbElems);

    _common.groundShader.popProgram();
}

void GroundComponent::update()
{
    _common.groundShader.pushThisProgram();
    _common.groundShader.setVec4f("sun.direction",   _common.viewedSunDirection);
    _common.groundShader.popProgram();
}

void GroundComponent::updateProjectionMatrix()
{
    _common.groundShader.pushThisProgram();
    _common.groundShader.setMatrix4x4("ProjectionMatrix", _common.projMat);
    _common.groundShader.popProgram();
}

void GroundComponent::updateModelViewMatrix()
{
    _common.groundShader.pushThisProgram();
    _common.groundShader.setMatrix4x4("ModelViewMatrix", _common.viewMat);
    _common.groundShader.setMatrix3x3("NormalMatrix",    _common.normalMat);
    _common.groundShader.popProgram();
}
