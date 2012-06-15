#include "DrawSunGroundAndRoads.h"
#include "City/CityMap.h"

#include <GL/glew.h>

#include <Misc/CellarUtils.h>
#include <Graphics/ImageBank.h>
#include <Graphics/GL/GLToolkit.h>
#include <Graphics/GL/GLInOutProgramLocation.h>
using namespace cellar;


DrawSunGroundAndRoads::DrawSunGroundAndRoads() :
    _roadWidth(0.125f),
    _roadShader(),
    _junctionsVao(0),
    _junctionsTex(0),
    _junctionsNbElems(0),
    _streetsVao(0),
    _streetsTex(0),
    _streetsNbElems(0)
{
    GLInOutProgramLocation locations;
    locations.setInput(0, "position_att");
    locations.setInput(1, "normal_att");
    locations.setInput(2, "texCoord_att");
    _roadShader.setInAndOutLocations(locations);
    _roadShader.loadShadersFromFile("resources/shaders/roads.vert",
                                    "resources/shaders/roads.frag");
    _roadShader.pushThisProgram();
    _roadShader.setInt("TextureUnit", 0);
    _roadShader.popProgram();
}

void DrawSunGroundAndRoads::updateViewMatrix(const cellar::Matrix4x4<float>& view)
{
    DrawSunAndGround::updateViewMatrix( view );

    _roadShader.pushThisProgram();
    _roadShader.setMatrix4x4("ModelViewMatrix",  _viewMat);
    _roadShader.setMatrix3x3("NormalMatrix", _viewSubMat3);
    _roadShader.popProgram();
}

void DrawSunGroundAndRoads::updateProjectionMatrix(const cellar::Matrix4x4<float>& proj)
{
    DrawSunAndGround::updateProjectionMatrix( proj );

    _roadShader.pushThisProgram();
    _roadShader.setMatrix4x4("ProjectionMatrix", _projMat);
    _roadShader.popProgram();
}

void DrawSunGroundAndRoads::draw()
{
    refresh();

    drawSky();
    drawSun();
    drawGround();
    drawJunctions();
    drawStreets();
    drawWater();
}

void DrawSunGroundAndRoads::drawJunctions()
{
    _roadShader.pushThisProgram();
    glVertexAttrib3f(_roadShader.getAttributeLocation("normal_att"), 0, 1, 1);

    glBindVertexArray(_junctionsVao);
    glBindTexture(GL_TEXTURE_2D, _junctionsTex);
    glDrawArrays(GL_QUADS, 0, _junctionsNbElems);

    _roadShader.popProgram();
}

void DrawSunGroundAndRoads::drawStreets()
{
    _roadShader.pushThisProgram();
    glVertexAttrib3f(_roadShader.getAttributeLocation("normal_att"), 1, 0, 0);

    glBindVertexArray(_streetsVao);
    glBindTexture(GL_TEXTURE_2D, _streetsTex);
    glDrawArrays(GL_QUADS, 0, _streetsNbElems);

    _roadShader.popProgram();
}

void DrawSunGroundAndRoads::setup(CityMap& cityMap)
{
    DrawSunAndGround::setup( cityMap );

    setupJunctions();
    setupStreets();
}

void DrawSunGroundAndRoads::setupJunctions()
{
    // Compute number of junction vertices
    _junctionsNbElems = 0;
    for(int j=0; j<_mapSize.y(); ++j)
        for(int i=0; i<_mapSize.x(); ++i)
            if(_cityMap->junctions().get(i , j)->type() != Junction::GROUND)
                _junctionsNbElems += 4;

    // Collect junctions to draw
    Vec3f* junctionsPos = new Vec3f[_junctionsNbElems];
    Vec2f* junctionsTex = new Vec2f[_junctionsNbElems];
    int idx = -1;

    for(int j=0; j<_mapSize.y(); ++j)
    {
        for(int i=0; i<_mapSize.x(); ++i)
        {
            if(_cityMap->junctions().get(i , j)->type() != Junction::GROUND)
            {
                float height = junctionHeight(i, j);
                junctionsPos[++idx] = Vec3f(i-_roadWidth, j-_roadWidth, height);
                junctionsTex[idx] = Vec2f(0, 0);
                junctionsPos[++idx] = Vec3f(i+_roadWidth, j-_roadWidth, height);
                junctionsTex[idx] = Vec2f(1, 0);
                junctionsPos[++idx] = Vec3f(i+_roadWidth, j+_roadWidth, height);
                junctionsTex[idx] = Vec2f(1, 1);
                junctionsPos[++idx] = Vec3f(i-_roadWidth, j+_roadWidth, height);
                junctionsTex[idx] = Vec2f(0, 1);
            }
        }
    }


    // Setup Vao
    glGenVertexArrays(1, &_junctionsVao);
    glBindVertexArray( _junctionsVao );
    GLuint buffers[2];
    glGenBuffers(2, buffers);

    int position_loc = _roadShader.getAttributeLocation("position_att");
    int texCoord_loc = _roadShader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*junctionsPos) * _junctionsNbElems, junctionsPos, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*junctionsTex) * _junctionsNbElems, junctionsTex, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );

    // Arrays sweaping
    delete [] junctionsPos;
    delete [] junctionsTex;


    // Texture
    _junctionsTex = GLToolkit::genTextureId( getImageBank().getImage("resources/textures/junction.bmp", false) );
}

void DrawSunGroundAndRoads::setupStreets()
{
    // Compute number of street vertices
    _streetsNbElems = 0;
    for(int j=0; j<_mapSize.y(); ++j)
    {
        for(int i=0; i<_mapSize.x(); ++i)
        {
            if(_cityMap->junctions().get(i , j)->getStreet(EAST)!= 0x0)
                _streetsNbElems += 4;
            if(_cityMap->junctions().get(i , j)->getStreet(NORTH)!= 0x0)
                _streetsNbElems += 4;
        }
    }

    // Collect streets to draw
    Vec3f* streetsPos = new Vec3f[_streetsNbElems];
    Vec2f* streetsTex = new Vec2f[_streetsNbElems];
    int idx = -1;

    float lengthRatio = (1-2*_roadWidth) / _roadWidth;

    for(int j=0; j<_mapSize.y(); ++j)
    {
        for(int i=0; i<_mapSize.x(); ++i)
        {
            if(_cityMap->junctions().get(i , j)->getStreet(EAST)!= 0x0)
            {
                float startHeight = junctionHeight(i, j);
                float endHeight = junctionHeight(i+1, j);
                streetsPos[++idx] = Vec3f(i + _roadWidth, j-_roadWidth, startHeight);
                streetsTex[idx] = Vec2f(0, 0);
                streetsPos[++idx] = Vec3f(i+1-_roadWidth, j-_roadWidth, endHeight);
                streetsTex[idx] = Vec2f(lengthRatio, 0);
                streetsPos[++idx] = Vec3f(i+1-_roadWidth, j+_roadWidth, endHeight);
                streetsTex[idx] = Vec2f(lengthRatio, 1);
                streetsPos[++idx] = Vec3f(i + _roadWidth, j+_roadWidth, startHeight);
                streetsTex[idx] = Vec2f(0, 1);
            }
            if(_cityMap->junctions().get(i , j)->getStreet(NORTH)!= 0x0)
            {
                float startHeight = junctionHeight(i, j);
                float endHeight = junctionHeight(i, j+1);
                streetsPos[++idx] = Vec3f(i-_roadWidth, j + _roadWidth, startHeight);
                streetsTex[idx] = Vec2f(0, 1);
                streetsPos[++idx] = Vec3f(i+_roadWidth, j + _roadWidth, startHeight);
                streetsTex[idx] = Vec2f(0, 0);
                streetsPos[++idx] = Vec3f(i+_roadWidth, j+1-_roadWidth, endHeight);
                streetsTex[idx] = Vec2f(lengthRatio, 0);
                streetsPos[++idx] = Vec3f(i-_roadWidth, j+1-_roadWidth, endHeight);
                streetsTex[idx] = Vec2f(lengthRatio, 1);
            }
        }
    }

    // Setup Vao
    glGenVertexArrays(1, &_streetsVao);
    glBindVertexArray( _streetsVao );
    GLuint buffers[2];
    glGenBuffers(2, buffers);

    int position_loc = _roadShader.getAttributeLocation("position_att");
    int texCoord_loc = _roadShader.getAttributeLocation("texCoord_att");

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*streetsPos) * _streetsNbElems, streetsPos, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*streetsTex) * _streetsNbElems, streetsTex, GL_STATIC_DRAW);
    glEnableVertexAttribArray(texCoord_loc);
    glVertexAttribPointer(texCoord_loc, 2, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );

    // Arrays sweaping
    delete [] streetsPos;
    delete [] streetsTex;


    // Texture
    _streetsTex = GLToolkit::genTextureId( getImageBank().getImage("resources/textures/street.bmp", false) );
}

float DrawSunGroundAndRoads::junctionHeight(int x, int y)
{
    float maxSide = cellar::max(_ground->heightAt(Vec2f(x - _roadWidth, y)),cellar::max(
                                _ground->heightAt(Vec2f(x + _roadWidth, y)),cellar::max(
                                _ground->heightAt(Vec2f(x, y - _roadWidth)),
                                _ground->heightAt(Vec2f(x, y + _roadWidth)))));

    float maxCorner = cellar::max(_ground->heightAt(Vec2f(x - _roadWidth, y - _roadWidth)),cellar::max(
                                  _ground->heightAt(Vec2f(x + _roadWidth, y - _roadWidth)),cellar::max(
                                  _ground->heightAt(Vec2f(x + _roadWidth, y + _roadWidth)),
                                  _ground->heightAt(Vec2f(x - _roadWidth, y + _roadWidth)))));

    return cellar::max(_ground->heightAt(Vec2i(x, y)), cellar::max(maxSide, maxCorner));
}
