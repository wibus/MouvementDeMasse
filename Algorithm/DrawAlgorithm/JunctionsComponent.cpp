#include "JunctionsComponent.h"
#include "DrawCityModule.h"

#include <GL/glew.h>
using namespace std;
#include <Misc/CellarUtils.h>
#include <Graphics/GL/GLToolkit.h>
#include <Graphics/ImageBank.h>
using namespace cellar;


JunctionsComponent::JunctionsComponent(DrawCityCommonData& common) :
    _common(common),
    _junctionsShader(),
    _junctionsVao(0),
    _junctionsTex(0),
    _junctionsNbElems(0)
{
    GLInOutProgramLocation locations;
    locations.setInput(0, "position_att");
    locations.setInput(1, "normal_att");
    locations.setInput(2, "texCoord_att");
    _junctionsShader.setInAndOutLocations(locations);
    _junctionsShader.loadShadersFromFile("resources/shaders/roads.vert",
                                    "resources/shaders/roads.frag");
    _junctionsShader.pushThisProgram();
    _junctionsShader.setInt("TextureUnit", 0);
    _junctionsShader.popProgram();
}

void JunctionsComponent::setup()
{
    // Compute number of junction vertices
    _junctionsNbElems = 0;
    for(int j=0; j<_common.cityMap.size().y()+1; ++j)
        for(int i=0; i<_common.cityMap.size().x()+1; ++i)
            if(_common.cityMap.junctions().get(i , j)->type() != Junction::GROUND)
                _junctionsNbElems += 4;

    // Collect junctions to draw
    Vec3f* junctionsPos = new Vec3f[_junctionsNbElems];
    Vec2f* junctionsTex = new Vec2f[_junctionsNbElems];
    int idx = -1;

    for(int j=0; j<_common.cityMap.size().y()+1; ++j)
    {
        for(int i=0; i<_common.cityMap.size().x()+1; ++i)
        {
            if(_common.cityMap.junctions().get(i , j)->type() != Junction::GROUND)
            {
                float height = junctionHeight(i, j);
                junctionsPos[++idx] = Vec3f(i-_common.roadWidth, j-_common.roadWidth, height);
                junctionsTex[idx] = Vec2f(0, 0);
                junctionsPos[++idx] = Vec3f(i+_common.roadWidth, j-_common.roadWidth, height);
                junctionsTex[idx] = Vec2f(1, 0);
                junctionsPos[++idx] = Vec3f(i+_common.roadWidth, j+_common.roadWidth, height);
                junctionsTex[idx] = Vec2f(1, 1);
                junctionsPos[++idx] = Vec3f(i-_common.roadWidth, j+_common.roadWidth, height);
                junctionsTex[idx] = Vec2f(0, 1);
            }
        }
    }


    // Setup Vao
    int position_loc = _junctionsShader.getAttributeLocation("position_att");
    int texCoord_loc = _junctionsShader.getAttributeLocation("texCoord_att");

    glGenVertexArrays(1, &_junctionsVao);
    glBindVertexArray( _junctionsVao );

    GLuint buffers[2];
    glGenBuffers(2, buffers);

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

void JunctionsComponent::draw()
{
    _junctionsShader.pushThisProgram();
    glVertexAttrib3f(_junctionsShader.getAttributeLocation("normal_att"), 0, 1, 1);

    glBindVertexArray(_junctionsVao);
    glBindTexture(GL_TEXTURE_2D, _junctionsTex);
    glDrawArrays(GL_QUADS, 0, _junctionsNbElems);

    _junctionsShader.popProgram();
}

void JunctionsComponent::update()
{
    _junctionsShader.pushThisProgram();
    _junctionsShader.setVec4f("sun.direction",   _common.viewedSunDirection);
    _junctionsShader.popProgram();
}

void JunctionsComponent::updateProjectionMatrix()
{
    _junctionsShader.pushThisProgram();
    _junctionsShader.setMatrix4x4("ProjectionMatrix", _common.projMat);
    _junctionsShader.popProgram();
}

void JunctionsComponent::updateModelViewMatrix()
{
    _junctionsShader.pushThisProgram();
    _junctionsShader.setMatrix4x4("ModelViewMatrix", _common.viewMat);
    _junctionsShader.setMatrix3x3("NormalMatrix",    _common.normalMat);
    _junctionsShader.popProgram();
}

float JunctionsComponent::junctionHeight(int x, int y)
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
