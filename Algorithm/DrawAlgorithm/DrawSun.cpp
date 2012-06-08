#include "DrawSun.h"

#include <GL/glew.h>

using namespace std;
using namespace cellar;

DrawSun::DrawSun() :
    _sunShader(),
    _sunVao(0),
    _sunNbElems(4),
    _sunRadius(10.0f),
    _sunLight(),
    _sunColor(     1.00, 0.90, 0.20, 1),
    _nightSkyColor(0.02, 0.01, 0.05, 1),
    _daySkyColor(  0.40, 0.60, 0.80, 1),
    _skyColor(     _nightSkyColor     )
{
    // Shader specific
    GLInOutProgramLocation locations;
    locations.setInput(0, "relPos_att");
     _sunShader.setInAndOutLocations(locations);
     _sunShader.loadShadersFromFile("resources/shaders/sun.vert",
                                    "resources/shaders/sun.frag");
     _sunShader.pushThisProgram();
     _sunShader.setVec4f("Color", _sunColor);
     _sunShader.setFloat("Radius", _sunRadius);
     glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
     glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
     _sunShader.popProgram();

     // Sun description
     _sunLight.direction(0, 0, 0.0, 0.0).normalize();
     _sunLight.ambient( 0.08, 0.09, 0.12);
     _sunLight.diffuse( 0.62, 0.62, 0.60);
     _sunLight.specular(0.65, 0.50, 0.30);
}

// DrawAlgorithm interface
void DrawSun::setup(CityMap& cityMap)
{
    DrawAlgorithm::setup( cityMap );

    unsigned int buffer;
    glGenVertexArrays(1, &_sunVao);
    glBindVertexArray(_sunVao);
    glGenBuffers(1, &buffer);

    int relPos_loc = _sunShader.getAttributeLocation("relPos_att");

    Vec2f* relPos = new Vec2f[_sunNbElems];
    relPos[0] = Vec2f(-1.0, -1.0);
    relPos[1] = Vec2f(1.00, -1.0);
    relPos[2] = Vec2f(1.00, 1.00);
    relPos[3] = Vec2f(-1.0, 1.00);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2f) * _sunNbElems, relPos, GL_STATIC_DRAW);
    glEnableVertexAttribArray(relPos_loc);
    glVertexAttribPointer(relPos_loc, 2, GL_FLOAT, 0, 0, 0);

    // Clearage
    delete [] relPos;
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void DrawSun::draw()
{
    refresh();

    drawSky();
    drawSun();
}

void DrawSun::refresh()
{
    DrawAlgorithm::refresh();

    _sunShader.pushThisProgram();
    _sunShader.setVec4f("SunDirection", _sunLight.direction);
    _sunShader.popProgram();
}

void DrawSun::drawSky()
{
    const double skyCoefCorection = 0.1;
    double skyColorCoef =
            pow(
                cellar::max(_sunLight.direction.normalized() * Vec4f(0, 0, -1, 0) + skyCoefCorection,
                    0.0),
                0.75) / (skyCoefCorection + 1.0);

    _skyColor = _nightSkyColor * (1 - skyColorCoef) + _daySkyColor * skyColorCoef;
    glClearColor(_skyColor.x(), _skyColor.y(), _skyColor.z(), _skyColor.w());
}

void DrawSun::drawSun()
{
    _sunShader.pushThisProgram();

    glEnable(GL_BLEND);
    glDepthMask(false);
    glBindVertexArray(_sunVao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, _sunNbElems);
    glDepthMask(true);
    glDisable(GL_BLEND);

    _sunShader.popProgram();
}

void DrawSun::updateViewMatrix(const cellar::Matrix4x4<float>& view)
{
    DrawAlgorithm::updateViewMatrix( view );

    _sunShader.pushThisProgram();
    _sunShader.setMatrix3x3("Rotation",  _viewSubMat3);
    _sunShader.popProgram();
}

void DrawSun::updateProjectionMatrix(const cellar::Matrix4x4<float>& proj)
{
    DrawAlgorithm::updateProjectionMatrix( proj );

    _sunShader.pushThisProgram();
    _sunShader.setMatrix4x4("Projection", _projMat);
    _sunShader.popProgram();
}

void DrawSun::updateSunDirection(const cellar::Vec4f& direction)
{
    _sunLight.direction = direction;
    _viewedSunDirection = _viewMat * direction;
}
