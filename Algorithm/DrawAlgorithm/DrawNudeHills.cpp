#include "DrawNudeHills.h"
#include "CityMap.h"
#include "Road/Junction.h"

using namespace std;

#include <Graphics/Colors/Colors.h>
using namespace cellar;

#include <GL/glew.h>


DrawNudeHills::DrawNudeHills() :
    _sunShader(),
    _hillsShader(),
    _sunRadius(8.0f),    
    _sunLight(),
    _sunRotation(),
    _nightSkyColor(0.02, 0.01, 0.05),
    _daySkyColor(0.4, 0.6, 0.8),
    _waterColor(0.03, 0.03, 0.3, 0.65)
{
    GLInOutProgramLocation sunLocations;
    sunLocations.setInput(0, "relPos_att");
     _sunShader.setInAndOutLocations(sunLocations);
     _sunShader.loadShadersFromFile("resources/shaders/sun.vert",
                                    "resources/shaders/sun.frag");
     _sunShader.pushThisProgram();
     _sunShader.setVec4f("Color", Vec4f(1.0, 0.9, 0.2, 1.0));
     _sunShader.setFloat("Radius", _sunRadius);
     _sunShader.popProgram();

    GLInOutProgramLocation locations;
    locations.setInput(0, "position_att");
    locations.setInput(1, "normal_att");
    locations.setInput(2, "color_att");
    _hillsShader.setInAndOutLocations(locations);
    _hillsShader.loadShadersFromFile("resources/shaders/nudeHills.vert",
                                     "resources/shaders/nudeHills.frag");

    Vec3f rotationAxis(-1.0, -1.0, 0.0);
    rotationAxis.normalize();
    _sunRotation.rotate(rotationAxis.x(), rotationAxis.y(), rotationAxis.z(), 0.008);
    _sunLight.direction(-1., -1.0, 2.00, 0.0).normalize();
    _sunLight.ambient( 0.08, 0.09, 0.12);
    _sunLight.diffuse( 0.62, 0.62, 0.60);
    _sunLight.specular(0.65, 0.50, 0.30);

    _hillsShader.pushThisProgram();
    _hillsShader.setVec4f("sun.direction", _sunLight.direction);
    _hillsShader.setVec3f("sun.ambient",   _sunLight.ambient);
    _hillsShader.setVec3f("sun.diffuse",   _sunLight.diffuse);
    _hillsShader.setVec3f("sun.specular",  _sunLight.specular);
    _hillsShader.setInt("Texture", 0);
    _hillsShader.popProgram();
}

void DrawNudeHills::notify(cellar::CameraMsg &msg)
{


    if(msg.change == CameraMsg::PROJECTION)
    {
        Matrix4x4<float> projection = msg.camera.projectionMatrix();

        _sunShader.pushThisProgram();
        _sunShader.setMatrix4x4("Projection", projection);
        _sunShader.popProgram();

        _hillsShader.pushThisProgram();
        _hillsShader.setMatrix4x4("Projection", projection);
        _hillsShader.popProgram();
    }
    else
    {
        _modelview = msg.camera.viewMatrix();
        Matrix3x3<float> modelviewSub3 = _modelview.subMat3();

        _sunShader.pushThisProgram();
        _sunShader.setMatrix3x3("Rotation",  modelviewSub3);
        _sunShader.popProgram();

        _hillsShader.pushThisProgram();
        _hillsShader.setMatrix4x4("ModelView",  _modelview);
        _hillsShader.setMatrix3x3("NormalMatrix", modelviewSub3);
        _hillsShader.popProgram();
    }
}

void DrawNudeHills::setup(CityMap& cityMap)
{
    // Premilinaries
    DrawAlgorithm::setup( cityMap );

    setupSun();
    setupGround();
    setupWater();
}

void DrawNudeHills::setupSun()
{
    _sNbElems = 4;
    unsigned int sBuffer;
    glGenVertexArrays(1, &_svao);
    glBindVertexArray(_svao);
    glGenBuffers(1, &sBuffer);

    int sRelPos_loc = _sunShader.getAttributeLocation("relPos_att");

    Vec2f srelPos[4];
    srelPos[0] = Vec2f(-_sunRadius, -_sunRadius);
    srelPos[1] = Vec2f(+_sunRadius, -_sunRadius);
    srelPos[2] = Vec2f(+_sunRadius, +_sunRadius);
    srelPos[3] = Vec2f(-_sunRadius, +_sunRadius);

    glBindBuffer(GL_ARRAY_BUFFER, sBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2f) * _sNbElems, srelPos, GL_STATIC_DRAW);
    glEnableVertexAttribArray(sRelPos_loc);
    glVertexAttribPointer(sRelPos_loc, 2, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void DrawNudeHills::setupGround()
{
    genGroundTexture();

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
    _gNbElems = gElemByRow * gNbRows;
    Vec3f* gnormals   = new Vec3f[_gNbElems];
    Vec3f* gpositions = new Vec3f[_gNbElems];


    int idx = -1;
    for(unsigned int j=0; j < gNbRows; ++j)
    {
        ++idx;
        gnormals[idx]   = derivate(Vec2ui(0, j));
        gpositions[idx] = Vec3f(0.0, j, _cityMap->junctions().get(0, j)->height());

        for(unsigned int i=0; i < _mapSize.x(); ++i)
        {
            ++idx;
            gnormals[idx]   = derivate(Vec2ui(i, j));
            gpositions[idx] = Vec3f(i, j, _cityMap->junctions().get(i, j)->height());


            ++idx;
            gnormals[idx]   = derivate(Vec2ui(i, j+1));
            gpositions[idx] = Vec3f(i, j+1, _cityMap->junctions().get(i, j+1)->height());
        }

        ++idx;
        int lastx = _mapSize.x() - 1;
        gnormals[idx]   = derivate(Vec2ui(lastx, j+1));
        gpositions[idx] = Vec3f(lastx, j+1, _cityMap->junctions().get(lastx, j+1)->height());
    }


    // Ground VAO setup
    glGenVertexArrays(1, &_gvao);
    glBindVertexArray(_gvao);

    const int nbAttributes = 2;
    GLuint gBuffers[nbAttributes];
    glGenBuffers(nbAttributes, gBuffers);

    int position_loc = _hillsShader.getAttributeLocation("position_att");
    int normal_loc = _hillsShader.getAttributeLocation("normal_att");

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3f) * _gNbElems, gnormals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3f) * _gNbElems, gpositions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );


    // Arrays sweaping
    delete [] gnormals;
    delete [] gpositions;

    _hillsShader.pushThisProgram();
    _hillsShader.setFloat("HillsAmplitude",
                          max(abs(_minHeight), abs(_maxHeight)));
    _hillsShader.popProgram();
}

void DrawNudeHills::setupWater()
{
    genWaterTexture();

    _wNbElems = 4;

    // Positions
    const float waterHeight = 0;
    Vec3f wpositions[4];
    wpositions[0] = Vec3f(0.0,          0.0,          waterHeight);
    wpositions[1] = Vec3f(_mapSize.x(), 0.0,          waterHeight);
    wpositions[2] = Vec3f(_mapSize.x(), _mapSize.y(), waterHeight);
    wpositions[3] = Vec3f(0.0,          _mapSize.y(), waterHeight);


    // Water VAO setup
    glGenVertexArrays(1, &_wvao);
    glBindVertexArray( _wvao );
    GLuint wBuffer;
    glGenBuffers(1, &wBuffer);

    int position_loc = _hillsShader.getAttributeLocation("position_att");

    glBindBuffer(GL_ARRAY_BUFFER, wBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(*wpositions) * _wNbElems, wpositions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);


    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void DrawNudeHills::draw()
{
    _sunLight.direction = _sunRotation * _sunLight.direction;
    Vec4f viewedSunDirection = _modelview * _sunLight.direction;

    // Sky Color
    const double skyCoefCorection = 0.1;
    double skyColorCoef = pow( max(_sunLight.direction.normalized() * Vec4f(0, 0, -1, 0) +skyCoefCorection,
                                   0.0), 0.75) / (skyCoefCorection + 1.0);
    _skyColor = _nightSkyColor * (1 - skyColorCoef) + _daySkyColor * skyColorCoef;
    glClearColor(_skyColor.x(), _skyColor.y(), _skyColor.z(), _skyColor.w());


    // Draw Hills
    _hillsShader.pushThisProgram();
    _hillsShader.setVec4f("sun.direction", viewedSunDirection);

    glBindVertexArray(_gvao);
    glBindTexture(GL_TEXTURE_1D, _gTex);
    _hillsShader.setFloat("Shininess", 20.0f);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _gNbElems);    

    glEnable(GL_BLEND);
    glBindVertexArray(_wvao);
    glBindTexture(GL_TEXTURE_1D, _wTex);
    _hillsShader.setFloat("Shininess", 100.0f);
    glVertexAttrib3fv(_hillsShader.getAttributeLocation("normal_att"), Vec3f(0.0, 0.0, 1.0).asArray());
    glDrawArrays(GL_TRIANGLE_FAN, 0, _wNbElems);
    glDisable(GL_BLEND);

    _hillsShader.popProgram();


    // Draw Sun
    _sunShader.pushThisProgram();
    _sunShader.setVec4f("SunDirection", _sunLight.direction);

    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBindVertexArray(_svao);
    glDrawArrays(GL_TRIANGLE_FAN, 0, _sNbElems);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    _sunShader.popProgram();
}

cellar::Vec3f DrawNudeHills::derivate(cellar::Vec2ui pos)
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

void DrawNudeHills::genGroundTexture()
{
    const int size = 128;
    const int heightZero = size/2;
    RGBAColor texels[size];

    Vec4f bottomSea(0.05, 0.03, 0.12, 1.0);
    Vec4f topSea(   0.10, 0.12, 0.40, 1.0);

    for(int i=0; i<heightZero; ++i)
    {
        float coef = (float)i / heightZero;
        Vec4f vecTexel = (coef*topSea + (1-coef)*bottomSea) * 255;
        texels[i] = RGBAColor(vecTexel.x(), vecTexel.y(), vecTexel.z(), vecTexel.w());
    }


    Vec4f bottomAir(0.20, 0.28, 0.03, 1.0);
    Vec4f topAir(   0.00, 1.00, 0.00, 1.0);

    for(int i=heightZero; i<size; ++i)
    {
        float coef = (float)(i-heightZero) / size;
        Vec4f vecTexel = (coef*topAir + (1-coef)*bottomAir) * 255;
        texels[i] = RGBAColor(vecTexel.x(), vecTexel.y(), vecTexel.z(), vecTexel.w());
    }

    glGenTextures(1, &_gTex);
    glBindTexture(GL_TEXTURE_1D, _gTex);
    glTexImage1D( GL_TEXTURE_1D, 0, 4, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, texels);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_1D, 0);
}

void DrawNudeHills::genWaterTexture()
{
    RGBAColor rgbaWater(_waterColor.x()*255, _waterColor.y()*255, _waterColor.z()*255, _waterColor.w()*255);

    glGenTextures(1, &_wTex);
    glBindTexture(GL_TEXTURE_1D, _wTex);
    glTexImage1D( GL_TEXTURE_1D, 0, 4, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &rgbaWater);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_1D, 0);
}
