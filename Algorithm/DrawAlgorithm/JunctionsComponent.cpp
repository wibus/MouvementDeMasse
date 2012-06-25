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
    _common.infrastructShader.pushThisProgram();
    _common.infrastructShader.setVec4f("sun.direction", _common.sunLight.direction);
    _common.infrastructShader.setVec4f("sun.ambient",   _common.sunLight.ambient);
    _common.infrastructShader.setVec4f("sun.diffuse",   _common.sunLight.diffuse);
    _common.infrastructShader.setVec4f("sun.specular",  _common.sunLight.specular);
    _common.infrastructShader.setFloat("Shininess",     128.0f);
    _common.infrastructShader.setInt("TexUnit",  0);
    _common.infrastructShader.setInt("SpecUnit", 1);
    _common.infrastructShader.popProgram();

    // Texture
    _junctionsTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/junction.bmp", false)
    );
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
    int position_loc = _common.infrastructShader.getAttributeLocation("position_att");
    int texCoord_loc = _common.infrastructShader.getAttributeLocation("texCoord_att");

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
}

void JunctionsComponent::draw()
{
    _common.infrastructShader.pushThisProgram();
    _common.infrastructShader.setVec3f("Translation", Vec3f());
    _common.infrastructShader.setFloat("StructureHeight", 1.0f);
    glVertexAttrib3f(_common.infrastructShader.getAttributeLocation("normal_att"), 0,0,1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _junctionsTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _junctionsTex);

    glBindVertexArray(_junctionsVao);    
    glDrawArrays(GL_QUADS, 0, _junctionsNbElems);

    _common.infrastructShader.popProgram();
}

void JunctionsComponent::update()
{
    _common.infrastructShader.pushThisProgram();
    _common.infrastructShader.setVec4f("sun.direction", _common.viewedSunDirection);
    _common.infrastructShader.setVec4f("sun.ambient",   _common.sunLight.ambient);
    _common.infrastructShader.popProgram();
}

void JunctionsComponent::updateProjectionMatrix()
{
    _common.infrastructShader.pushThisProgram();
    _common.infrastructShader.setMatrix4x4("ProjectionMatrix", _common.projMat);
    _common.infrastructShader.popProgram();
}

void JunctionsComponent::updateModelViewMatrix()
{
    _common.infrastructShader.pushThisProgram();
    _common.infrastructShader.setMatrix4x4("ModelViewMatrix", _common.viewMat);
    _common.infrastructShader.setMatrix3x3("NormalMatrix",    _common.normalMat);
    _common.infrastructShader.popProgram();
}
