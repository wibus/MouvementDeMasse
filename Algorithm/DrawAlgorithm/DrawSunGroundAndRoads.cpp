#include "DrawSunGroundAndRoads.h"
#include "CityMap.h"

#include <GL/glew.h>

#include <Misc/CellarUtils.h>
#include <Graphics/GL/GLInOutProgramLocation.h>
using namespace cellar;


DrawSunGroundAndRoads::DrawSunGroundAndRoads() :
    _roadWidth(0.2f),
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
    glBindTexture(GL_TEXTURE_2D, _junctionsVao);
    glDrawArrays(GL_QUADS, 0, _junctionsNbElems);

    _roadShader.popProgram();
}

void DrawSunGroundAndRoads::drawStreets()
{
    _roadShader.pushThisProgram();
    glVertexAttrib3f(_roadShader.getAttributeLocation("normal_att"), 1, 0, 0);

    glBindVertexArray(_streetsVao);
    glBindTexture(GL_TEXTURE_2D, _streetsVao);
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
    for(unsigned int j=0; j<_mapSize.y(); ++j)
        for(unsigned int i=0; i<_mapSize.x(); ++i)
            if(_cityMap->junctions().get(i , j)->type() != Junction::GROUND)
                _junctionsNbElems += 4;

    // Collect junctions to draw
    Vec3f* junctionsPos = new Vec3f[_junctionsNbElems];
    int idx = -1;

    for(unsigned int j=0; j<_mapSize.y(); ++j)
    {
        for(unsigned int i=0; i<_mapSize.x(); ++i)
        {
            if(_cityMap->junctions().get(i , j)->type() != Junction::GROUND)
            {
                float height = junctionHeight(i, j);
                junctionsPos[++idx] = Vec3f(i-_roadWidth, j-_roadWidth, height);
                junctionsPos[++idx] = Vec3f(i+_roadWidth, j-_roadWidth, height);
                junctionsPos[++idx] = Vec3f(i+_roadWidth, j+_roadWidth, height);
                junctionsPos[++idx] = Vec3f(i-_roadWidth, j+_roadWidth, height);
            }
        }
    }


    // Setup Vao
    glGenVertexArrays(1, &_junctionsVao);
    glBindVertexArray( _junctionsVao );
    GLuint buffer;
    glGenBuffers(1, &buffer);

    int position_loc = _roadShader.getAttributeLocation("position_att");

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*junctionsPos) * _junctionsNbElems, junctionsPos, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );

    // Arrays sweaping
    delete [] junctionsPos;
}

void DrawSunGroundAndRoads::setupStreets()
{
    // Compute number of junction vertices
    _streetsNbElems = 0;
    for(unsigned int j=0; j<_mapSize.y(); ++j)
    {
        for(unsigned int i=0; i<_mapSize.x(); ++i)
        {
            if(_cityMap->junctions().get(i , j)->getStreet(EAST)!= 0x0)
                _streetsNbElems += 4;
            if(_cityMap->junctions().get(i , j)->getStreet(NORTH)!= 0x0)
                _streetsNbElems += 4;
        }
    }

    // Collect junctions to draw
    Vec3f* streetsPos = new Vec3f[_streetsNbElems];
    int idx = -1;

    for(unsigned int j=0; j<_mapSize.y(); ++j)
    {
        for(unsigned int i=0; i<_mapSize.x(); ++i)
        {
            if(_cityMap->junctions().get(i , j)->getStreet(EAST)!= 0x0)
            {
                float startHeight = junctionHeight(i, j);
                float endHeight = junctionHeight(i+1, j);
                streetsPos[++idx] = Vec3f(i + _roadWidth, j-_roadWidth, startHeight);
                streetsPos[++idx] = Vec3f(i+1-_roadWidth, j-_roadWidth, endHeight);
                streetsPos[++idx] = Vec3f(i+1-_roadWidth, j+_roadWidth, endHeight);
                streetsPos[++idx] = Vec3f(i + _roadWidth, j+_roadWidth, startHeight);
            }
            if(_cityMap->junctions().get(i , j)->getStreet(NORTH)!= 0x0)
            {
                float startHeight = junctionHeight(i, j);
                float endHeight = junctionHeight(i, j+1);
                streetsPos[++idx] = Vec3f(i-_roadWidth, j + _roadWidth, startHeight);
                streetsPos[++idx] = Vec3f(i+_roadWidth, j + _roadWidth, startHeight);
                streetsPos[++idx] = Vec3f(i+_roadWidth, j+1-_roadWidth, endHeight);
                streetsPos[++idx] = Vec3f(i-_roadWidth, j+1-_roadWidth, endHeight);
            }
        }
    }


    // Setup Vao
    glGenVertexArrays(1, &_streetsVao);
    glBindVertexArray( _streetsVao );
    GLuint buffer;
    glGenBuffers(1, &buffer);

    int position_loc = _roadShader.getAttributeLocation("position_att");

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*streetsPos) * _streetsNbElems, streetsPos, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );

    // Arrays sweaping
    delete [] streetsPos;
}

float DrawSunGroundAndRoads::junctionHeight(unsigned int x, unsigned int y)
{
    return cellar::max(_cityMap->heightAt(Vec2f(x,              y)),             cellar::max(
                       _cityMap->heightAt(Vec2f(x - _roadWidth, y - _roadWidth)),cellar::max(
                       _cityMap->heightAt(Vec2f(x + _roadWidth, y - _roadWidth)),cellar::max(
                       _cityMap->heightAt(Vec2f(x + _roadWidth, y + _roadWidth)),
                       _cityMap->heightAt(Vec2f(x - _roadWidth, y + _roadWidth))))));
}
