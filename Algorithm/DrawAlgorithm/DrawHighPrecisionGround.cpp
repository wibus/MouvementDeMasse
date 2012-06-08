#include "DrawHighPrecisionGround.h"
#include <CityMap.h>

#include <GL/glew.h>

using namespace cellar;

DrawHighPrecisionGround::DrawHighPrecisionGround() :
    _highPrecShader(),
    _highPrecvao(0)
{
    _highPrecShader.loadShadersFromFile("resources/shaders/highPrecGround.vert",
                                        "resources/shaders/highPrecGround.frag");
}

void DrawHighPrecisionGround::setup(CityMap& cityMap)
{
    DrawSunAndGround::setup( cityMap );

    const int pointPerUnit = 20;
    const int radius = 10;

    Vec2i blCorner(_mapSize.x() / 2 - radius, _mapSize.y() / 2 - radius);
    Vec2i urCorner(_mapSize.x() / 2 + radius, _mapSize.y() / 2 + radius);
    Vec2i size(urCorner.x() - blCorner.x(), urCorner.y() - blCorner.y());
    Vec2i realSize(size * pointPerUnit);
    _highPrecNbElems = realSize.x()* realSize.y();

    Vec3f* highPrecPos = new Vec3f[_highPrecNbElems];

    for(int j=0; j<realSize.y(); ++j)
    {
        for(int i=0; i<realSize.x(); ++i)
        {
            Vec3f realPos(blCorner.x() + (float)i/pointPerUnit,
                          blCorner.y() + (float)j/pointPerUnit, 0);
            highPrecPos[j*realSize.x() + i] = Vec3f(realPos.x(), realPos.y(),
                _ground->heightAt(realPos.x(), realPos.y()));
            realPos.setZ(highPrecPos[j*realSize.x() + i].z());
            realPos.setX(0);
        }
    }


    // VAO setup
    glGenVertexArrays(1, &_highPrecvao);
    glBindVertexArray( _highPrecvao );
    GLuint wBuffer;
    glGenBuffers(1, &wBuffer);

    int position_loc = _highPrecShader.getAttributeLocation("position_att");

    glBindBuffer(GL_ARRAY_BUFFER, wBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*highPrecPos) * _highPrecNbElems, highPrecPos, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );

    delete [] highPrecPos;
}

void DrawHighPrecisionGround::draw()
{
    //drawGround();

    _highPrecShader.pushThisProgram();

    glBindVertexArray(_highPrecvao);
    glDrawArrays(GL_LINE_STRIP, 0, _highPrecNbElems);

    _highPrecShader.popProgram();

    drawWater();
}

void DrawHighPrecisionGround::updateViewMatrix(const cellar::Matrix4x4<float>& view)
{
    DrawSunAndGround::updateViewMatrix( view );

    _highPrecShader.pushThisProgram();
    _highPrecShader.setMatrix4x4("ModelViewMatrix",  _viewMat);
    _highPrecShader.popProgram();
}

void DrawHighPrecisionGround::updateProjectionMatrix(const cellar::Matrix4x4<float>& proj)
{
    DrawSunAndGround::updateProjectionMatrix( proj );

    _highPrecShader.pushThisProgram();
    _highPrecShader.setMatrix4x4("ProjectionMatrix", _projMat);
    _highPrecShader.popProgram();
}
