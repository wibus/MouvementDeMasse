#include "JunctionsComponent.h"

#include <GL/glew.h>
using namespace std;
#include <Misc/CellarUtils.h>
#include <Graphics/GL/GLToolkit.h>
#include <Graphics/ImageBank.h>
using namespace cellar;


JunctionsComponent::JunctionsComponent(City &city, GLShaderProgram &shader) :
    AbstractComponent(city, shader),
    _junctionsVao(0),
    _junctionsTex(0),
    _junctionsNbElems(0)
{
    glGenVertexArrays(1, &_junctionsVao);

    _junctionsTex = GLToolkit::genTextureId(
        getImageBank().getImage("resources/textures/junction.bmp", false)
    );
}

JunctionsComponent::~JunctionsComponent()
{
    glDeleteVertexArrays(1, &_junctionsVao);
    GLToolkit::deleteTextureId(_junctionsTex);
}

void JunctionsComponent::setup()
{
    // Compute number of junction vertices
    _junctionsNbElems = 0;
    for(int j=0; j<_city.size().y()+1; ++j)
        for(int i=0; i<_city.size().x()+1; ++i)
            if(_city.junctions().get(i , j)->type() != Junction::GRASS)
                _junctionsNbElems += 4;

    // Collect junctions to draw
    Vec3f* junctionsPos = new Vec3f[_junctionsNbElems];
    Vec2f* junctionsTex = new Vec2f[_junctionsNbElems];
    int idx = -1;

    float roadHalfWidth = _description.roadWidth * 0.5f;

    for(int j=0; j<_city.size().y()+1; ++j)
    {
        for(int i=0; i<_city.size().x()+1; ++i)
        {
            if(_city.junctions().get(i , j)->type() != Junction::GRASS)
            {
                float height = _ground.heightAt(i, j);
                junctionsPos[++idx] = Vec3f(i-roadHalfWidth, j-roadHalfWidth, height);
                junctionsTex[idx] = Vec2f(0, 0);
                junctionsPos[++idx] = Vec3f(i+roadHalfWidth, j-roadHalfWidth, height);
                junctionsTex[idx] = Vec2f(1, 0);
                junctionsPos[++idx] = Vec3f(i+roadHalfWidth, j+roadHalfWidth, height);
                junctionsTex[idx] = Vec2f(1, 1);
                junctionsPos[++idx] = Vec3f(i-roadHalfWidth, j+roadHalfWidth, height);
                junctionsTex[idx] = Vec2f(0, 1);
            }
        }
    }


    // Setup Vao
    int position_loc = _shader.getAttributeLocation("position_att");
    int texCoord_loc = _shader.getAttributeLocation("texCoord_att");

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
    _shader.pushThisProgram();
    glVertexAttrib3f(_shader.getAttributeLocation("normal_att"), 0,0,1);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, _junctionsTex);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _junctionsTex);

    glBindVertexArray(_junctionsVao);    
    glDrawArrays(GL_QUADS, 0, _junctionsNbElems);

    _shader.popProgram();
}
