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
    _junctionsVao(0),
    _junctionsTex(0),
    _junctionsNbElems(0)
{
    _common.roadsShader.pushThisProgram();
    _common.roadsShader.setInt("TextureUnit", 0);
    _common.roadsShader.popProgram();
}

void JunctionsComponent::setup()
{
    // Compute number of junction vertices
    _junctionsNbElems = 0;
    for(int j=0; j<_common.cityMap.size().y()+1; ++j)
        for(int i=0; i<_common.cityMap.size().x()+1; ++i)
            if(_common.cityMap.junctions().get(i , j)->type() != Junction::GRASS)
                _junctionsNbElems += 4;

    // Collect junctions to draw
    Vec3f* junctionsPos = new Vec3f[_junctionsNbElems];
    Vec2f* junctionsTex = new Vec2f[_junctionsNbElems];
    int idx = -1;

    for(int j=0; j<_common.cityMap.size().y()+1; ++j)
    {
        for(int i=0; i<_common.cityMap.size().x()+1; ++i)
        {
            if(_common.cityMap.junctions().get(i , j)->type() != Junction::GRASS)
            {
                float height = _common.ground.heightAt(i, j);
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
    int position_loc = _common.roadsShader.getAttributeLocation("position_att");
    int texCoord_loc = _common.roadsShader.getAttributeLocation("texCoord_att");

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
    _junctionsTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/junction.bmp", false)
    );
}

void JunctionsComponent::draw()
{
    _common.roadsShader.pushThisProgram();
    glVertexAttrib3f(_common.roadsShader.getAttributeLocation("normal_att"), 0, 1, 1);

    glBindVertexArray(_junctionsVao);
    glBindTexture(GL_TEXTURE_2D, _junctionsTex);
    glDrawArrays(GL_QUADS, 0, _junctionsNbElems);

    _common.roadsShader.popProgram();
}

void JunctionsComponent::update()
{
    _common.roadsShader.pushThisProgram();
    _common.roadsShader.setVec4f("sun.direction",   _common.viewedSunDirection);
    _common.roadsShader.popProgram();
}

void JunctionsComponent::updateProjectionMatrix()
{
    _common.roadsShader.pushThisProgram();
    _common.roadsShader.setMatrix4x4("ProjectionMatrix", _common.projMat);
    _common.roadsShader.popProgram();
}

void JunctionsComponent::updateModelViewMatrix()
{
    _common.roadsShader.pushThisProgram();
    _common.roadsShader.setMatrix4x4("ModelViewMatrix", _common.viewMat);
    _common.roadsShader.setMatrix3x3("NormalMatrix",    _common.normalMat);
    _common.roadsShader.popProgram();
}
