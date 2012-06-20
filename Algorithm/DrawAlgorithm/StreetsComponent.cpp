#include "StreetsComponent.h"
#include "DrawCityModule.h"

#include <GL/glew.h>
using namespace std;
#include <Misc/CellarUtils.h>
#include <Graphics/GL/GLToolkit.h>
#include <Graphics/ImageBank.h>
using namespace cellar;


StreetsComponent::StreetsComponent(DrawCityCommonData& common) :
    _common(common),
    _streetsVao(0),
    _streetsTex(0),
    _streetsNbElems(0)
{

}

void StreetsComponent::setup()
{
    // Compute number of street vertices
    _streetsNbElems = 0;
    for(int j=0; j<_common.cityMap.size().y()+1; ++j)
    {
        for(int i=0; i<_common.cityMap.size().x()+1; ++i)
        {
            if(_common.cityMap.junctions().get(i , j)->getStreet(EAST)!= 0x0)
                _streetsNbElems += 4;
            if(_common.cityMap.junctions().get(i , j)->getStreet(NORTH)!= 0x0)
                _streetsNbElems += 4;
        }
    }

    // Collect streets to draw
    Vec3f* streetsPos = new Vec3f[_streetsNbElems];
    Vec2f* streetsTex = new Vec2f[_streetsNbElems];
    int idx = -1;

    float lengthRatio = (1-2*_common.roadWidth) / _common.roadWidth;

    for(int j=0; j<_common.cityMap.size().y()+1; ++j)
    {
        for(int i=0; i<_common.cityMap.size().x()+1; ++i)
        {
            if(_common.cityMap.junctions().get(i , j)->getStreet(EAST)!= 0x0)
            {
                float startHeight = junctionHeight(i, j);
                float endHeight   = junctionHeight(i+1, j);
                streetsPos[++idx] = Vec3f(i + _common.roadWidth, j-_common.roadWidth, startHeight);
                streetsTex[idx] = Vec2f(0, 0);
                streetsPos[++idx] = Vec3f(i+1-_common.roadWidth, j-_common.roadWidth, endHeight);
                streetsTex[idx] = Vec2f(lengthRatio, 0);
                streetsPos[++idx] = Vec3f(i+1-_common.roadWidth, j+_common.roadWidth, endHeight);
                streetsTex[idx] = Vec2f(lengthRatio, 1);
                streetsPos[++idx] = Vec3f(i + _common.roadWidth, j+_common.roadWidth, startHeight);
                streetsTex[idx] = Vec2f(0, 1);
            }
            if(_common.cityMap.junctions().get(i , j)->getStreet(NORTH)!= 0x0)
            {
                float startHeight = junctionHeight(i, j);
                float endHeight = junctionHeight(i, j+1);
                streetsPos[++idx] = Vec3f(i-_common.roadWidth, j + _common.roadWidth, startHeight);
                streetsTex[idx] = Vec2f(0, 1);
                streetsPos[++idx] = Vec3f(i+_common.roadWidth, j + _common.roadWidth, startHeight);
                streetsTex[idx] = Vec2f(0, 0);
                streetsPos[++idx] = Vec3f(i+_common.roadWidth, j+1-_common.roadWidth, endHeight);
                streetsTex[idx] = Vec2f(lengthRatio, 0);
                streetsPos[++idx] = Vec3f(i-_common.roadWidth, j+1-_common.roadWidth, endHeight);
                streetsTex[idx] = Vec2f(lengthRatio, 1);
            }
        }
    }

    // Setup Vao
    glGenVertexArrays(1, &_streetsVao);
    glBindVertexArray( _streetsVao );

    GLuint buffers[2];
    glGenBuffers(2, buffers);

    int position_loc = _common.roadsShader.getAttributeLocation("position_att");
    int texCoord_loc = _common.roadsShader.getAttributeLocation("texCoord_att");

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
    _streetsTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/street.bmp", false)
    );
}

void StreetsComponent::draw()
{
    _common.roadsShader.pushThisProgram();
    glVertexAttrib3f(_common.roadsShader.getAttributeLocation("normal_att"), 1, 0, 0);

    glBindVertexArray(_streetsVao);
    glBindTexture(GL_TEXTURE_2D, _streetsTex);
    glDrawArrays(GL_QUADS, 0, _streetsNbElems);

    _common.roadsShader.popProgram();
}

void StreetsComponent::update()
{
}

void StreetsComponent::updateProjectionMatrix()
{
}

void StreetsComponent::updateModelViewMatrix()
{
}

float StreetsComponent::junctionHeight(int x, int y)
{
    float maxSide = cellar::max(_common.ground.heightAt(Vec2f(x - _common.roadWidth, y)),cellar::max(
                                _common.ground.heightAt(Vec2f(x + _common.roadWidth, y)),cellar::max(
                                _common.ground.heightAt(Vec2f(x,                     y - _common.roadWidth)),
                                _common.ground.heightAt(Vec2f(x,                     y + _common.roadWidth)))));

    float maxCorner = cellar::max(_common.ground.heightAt(Vec2f(x - _common.roadWidth, y - _common.roadWidth)),cellar::max(
                                  _common.ground.heightAt(Vec2f(x + _common.roadWidth, y - _common.roadWidth)),cellar::max(
                                  _common.ground.heightAt(Vec2f(x + _common.roadWidth, y + _common.roadWidth)),
                                  _common.ground.heightAt(Vec2f(x - _common.roadWidth, y + _common.roadWidth)))));

    return cellar::max(_common.ground.heightAt(Vec2i(x, y)), cellar::max(maxSide, maxCorner));
}
