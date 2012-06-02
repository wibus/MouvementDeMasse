#include "DrawSunAndGround.h"
#include "CityMap.h"

#include <GL/glew.h>

using namespace std;
using namespace cellar;


DrawSunAndGround::DrawSunAndGround() :
    _groundShader(),
    _groundVao(0),
    _groundTex(0),
    _groundNbElems(0),
    _groundShininess(20.0f),
    _waterVao(0),
    _waterTex(0),
    _waterNbElems(4),
    _waterColor(0.03, 0.03, 0.30, 0.65),
    _waterShininess(100.0f)
{
    // Ground
    GLInOutProgramLocation groundLocations;
    groundLocations.setInput(0, "position_att");
    groundLocations.setInput(1, "normal_att");
    _groundShader.setInAndOutLocations(groundLocations);
    _groundShader.loadShadersFromFile("resources/shaders/ground.vert",
                                      "resources/shaders/ground.frag");
    _groundShader.pushThisProgram();
    _groundShader.setVec4f("sun.direction", _sunLight.direction);
    _groundShader.setVec3f("sun.ambient",   _sunLight.ambient);
    _groundShader.setVec3f("sun.diffuse",   _sunLight.diffuse);
    _groundShader.setVec3f("sun.specular",  _sunLight.specular);
    _groundShader.setInt("TextureUnit", 0);
    _groundShader.popProgram();
}

void DrawSunAndGround::updateViewMatrix(const cellar::Matrix4x4<float>& view)
{
    DrawSun::updateViewMatrix( view );

    _groundShader.pushThisProgram();
    _groundShader.setMatrix4x4("ModelViewMatrix",  _viewMat);
    _groundShader.setMatrix3x3("NormalMatrix", _viewSubMat3);
    _groundShader.popProgram();
}

void DrawSunAndGround::updateProjectionMatrix(const cellar::Matrix4x4<float>& proj)
{
    DrawSun::updateProjectionMatrix( proj );

    _groundShader.pushThisProgram();
    _groundShader.setMatrix4x4("ProjectionMatrix", _projMat);
    _groundShader.popProgram();
}

void DrawSunAndGround::draw()
{
    drawSky();
    drawSun();
    drawGround();
    drawWater();
}

void DrawSunAndGround::drawGround()
{
    _groundShader.pushThisProgram();
    _groundShader.setVec4f("sun.direction", _viewedSunDirection);
    _groundShader.setFloat("Shininess",     _groundShininess);

    glBindVertexArray(_groundVao);
    glBindTexture(GL_TEXTURE_1D, _groundTex);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _groundNbElems);

    _groundShader.popProgram();
}

void DrawSunAndGround::drawWater()
{
    _groundShader.pushThisProgram();
    _groundShader.setVec4f("sun.direction", _viewedSunDirection);
    _groundShader.setFloat("Shininess",     _waterShininess);
    glVertexAttrib3f(_groundShader.getAttributeLocation("normal_att"), 0, 0, 1);

    glEnable(GL_BLEND);
    glBindVertexArray(_waterVao);
    glBindTexture(GL_TEXTURE_1D, _waterTex);
    glDrawArrays(GL_TRIANGLE_FAN, 0, _waterNbElems);
    glDisable(GL_BLEND);

    _groundShader.popProgram();
}

void DrawSunAndGround::setup(CityMap& cityMap)
{
    DrawSun::setup( cityMap );

    setupGround();
    setupWater();
}

void DrawSunAndGround::setupGround()
{
    genGroundTex();

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

    unsigned int gNbRows = (_mapSize.y() - 1);
    unsigned int gElemByRow = (_mapSize.x() + 1) * 2;
    _groundNbElems = gElemByRow * gNbRows;
    Vec3f* gnormals   = new Vec3f[_groundNbElems];
    Vec3f* gpositions = new Vec3f[_groundNbElems];


    int idx = -1;
    for(unsigned int j=0; j < gNbRows; ++j)
    {
        computeGroundVertex(idx, gpositions, gnormals, 0, j);

        for(unsigned int i=0; i < _mapSize.x(); ++i)
        {
            computeGroundVertex(idx, gpositions, gnormals, i, j);
            computeGroundVertex(idx, gpositions, gnormals, i, j+1);
        }

        int lastx = _mapSize.x() - 1;
        computeGroundVertex(idx, gpositions, gnormals, lastx, j+1);
    }


    // Ground VAO setup
    glGenVertexArrays(1, &_groundVao);
    glBindVertexArray(_groundVao);

    const int nbAttributes = 2;
    GLuint gBuffers[nbAttributes];
    glGenBuffers(nbAttributes, gBuffers);

    int position_loc = _groundShader.getAttributeLocation("position_att");
    int normal_loc = _groundShader.getAttributeLocation("normal_att");

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*gnormals) * _groundNbElems, gnormals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*gnormals) * _groundNbElems, gpositions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );


    // Arrays sweaping
    delete [] gnormals;
    delete [] gpositions;

    _groundShader.pushThisProgram();
    _groundShader.setFloat("HillsAmplitude", max(abs(_minHeight), abs(_maxHeight)));
    _groundShader.popProgram();
}

void DrawSunAndGround::computeGroundVertex(int& idx, cellar::Vec3f* pos, cellar::Vec3f* norm, unsigned int i, unsigned j)
{
    ++idx;
    norm[idx]   = derivate(Vec2ui(i, j));
    pos[idx] = Vec3f(i, j, _cityMap->junctions().get(i, j)->height());
}

cellar::Vec3f DrawSunAndGround::derivate(const cellar::Vec2ui& pos)
{
    Vec3f derivate(0, 0, -1);
    float currentHeight =_cityMap->junctions().get(pos.x(), pos.y())->height();

    if (pos.x() == 0)
    {
        derivate.setX(_cityMap->junctions().get(1, pos.y())->height() - currentHeight);
    }
    else if (pos.x() == _mapSize.x() - 1)
    {
        derivate.setX(currentHeight - _cityMap->junctions().get(_mapSize.x() - 2, pos.y())->height());
    }
    else
    {
        float prev = currentHeight - _cityMap->junctions().get(pos.x() - 1, pos.y())->height();
        float next = _cityMap->junctions().get(pos.x() + 1, pos.y())->height() - currentHeight;

        derivate.setX((prev + next) / 2);
    }
    if (pos.y() == 0)
    {
        derivate.setY(_cityMap->junctions().get(pos.x(), 1)->height() - currentHeight);
    }
    else if (pos.y() == _mapSize.y() - 1)
    {
        derivate.setY(currentHeight - _cityMap->junctions().get(pos.x(), _mapSize.y() - 2)->height());
    }
    else
    {
        float prev = currentHeight - _cityMap->junctions().get(pos.x(), pos.y() - 1)->height();
        float next = _cityMap->junctions().get(pos.x(), pos.y() + 1)->height() - currentHeight;

        derivate.setY((prev + next) / 2);
    }

    return -derivate;
}

void DrawSunAndGround::setupWater()
{
    genWaterTex();

    // Positions
    const float waterHeight = 0;
    Vec3f* wpositions = new Vec3f[_waterNbElems];
    wpositions[0] = Vec3f(0.0,          0.0,          waterHeight);
    wpositions[1] = Vec3f(_mapSize.x(), 0.0,          waterHeight);
    wpositions[2] = Vec3f(_mapSize.x(), _mapSize.y(), waterHeight);
    wpositions[3] = Vec3f(0.0,          _mapSize.y(), waterHeight);


    // Water VAO setup
    glGenVertexArrays(1, &_waterVao);
    glBindVertexArray( _waterVao );
    GLuint wBuffer;
    glGenBuffers(1, &wBuffer);

    int position_loc = _groundShader.getAttributeLocation("position_att");

    glBindBuffer(GL_ARRAY_BUFFER, wBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*wpositions) * _waterNbElems, wpositions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );

    // Arrays sweaping
    delete [] wpositions;
}

void DrawSunAndGround::genGroundTex()
{
    const int size = 128;
    const int heightZero = size/2;
    Vec4ub texels[size];

    Vec4f bottomSeaColor(0.05, 0.03, 0.12, 1.0);
    Vec4f topSeaColor(   0.10, 0.12, 0.40, 1.0);

    for(int i=0; i<heightZero; ++i)
    {
        float coef = (float)i / heightZero;
        Vec4f vecTexel = (coef*topSeaColor + (1-coef)*bottomSeaColor) * 255;
        texels[i] = Vec4ub(vecTexel.x(), vecTexel.y(), vecTexel.z(), vecTexel.w());
    }


    Vec4f bottomAirColor(0.20, 0.28, 0.03, 1.0);
    Vec4f topAirColor(   0.00, 1.00, 0.00, 1.0);

    for(int i=heightZero; i<size; ++i)
    {
        float coef = (float)(i-heightZero) / size;
        Vec4f vecTexel = (coef*topAirColor + (1-coef)*bottomAirColor) * 255;
        texels[i] = Vec4ub(vecTexel.x(), vecTexel.y(), vecTexel.z(), vecTexel.w());
    }

    glGenTextures(1, &_groundTex);
    glBindTexture(GL_TEXTURE_1D, _groundTex);
    glTexImage1D( GL_TEXTURE_1D, 0, 4, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_1D, 0);
}

void DrawSunAndGround::genWaterTex()
{
    Vec4ub rgbaWater(_waterColor.x()*255, _waterColor.y()*255,
                     _waterColor.z()*255, _waterColor.w()*255);

    glGenTextures(1, &_waterTex);
    glBindTexture(GL_TEXTURE_1D, _waterTex);
    glTexImage1D( GL_TEXTURE_1D, 0, 4, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &rgbaWater);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_1D, 0);
}
