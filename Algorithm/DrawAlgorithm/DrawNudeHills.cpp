#include "DrawNudeHills.h"
#include "CityMap.h"
#include "Road/Junction.h"
using namespace cellar;

using namespace std;

#include <GL/glew.h>


DrawNudeHills::DrawNudeHills() :
    _sunShader(),
    _sunRadius(8.0f),
    _hillsShader(),
    _sunLight(),
    _sunRotation()
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

    Vec3f rotationAxis(-1.0, -1.0, 1.0);
    rotationAxis.normalize();
    _sunRotation.rotate(rotationAxis.x(), rotationAxis.y(), rotationAxis.z(), 0.008);
    _sunLight.direction(-1.0, -1.0, -1.0, 0.0).normalize();
    _sunLight.ambient( 0.16, 0.18, 0.24);
    _sunLight.diffuse( 0.62, 0.62, 0.60);
    _sunLight.specular(0.65, 0.50, 0.30);

    _hillsShader.pushThisProgram();
    _hillsShader.setVec4f("sun.direction", _sunLight.direction);
    _hillsShader.setVec3f("sun.ambient",   _sunLight.ambient);
    _hillsShader.setVec3f("sun.diffuse",   _sunLight.diffuse);
    _hillsShader.setVec3f("sun.specular",  _sunLight.specular);
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

        _sunShader.pushThisProgram();
        _sunShader.setMatrix4x4("ModelView",  _modelview);
        _sunShader.popProgram();

        _hillsShader.pushThisProgram();
        _hillsShader.setMatrix4x4("ModelView",  _modelview);
        _hillsShader.setMatrix3x3("NormalMatrix", _modelview.subMat3());
        _hillsShader.popProgram();
    }
}

void DrawNudeHills::setup(CityMap& cityMap)
{
    // Premilinaries
    DrawAlgorithm::setup( cityMap );

    // Sun
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



    // Ground Algorithm //

    int position_loc = _hillsShader.getAttributeLocation("position_att");
    int normal_loc = _hillsShader.getAttributeLocation("normal_att");
    //int color_loc = _shader.getAttributeLocation("color_att");

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
        gpositions[idx] = Vec3f(0.0, j, cityMap.junctions().get(0, j)->height());

        for(unsigned int i=0; i < _mapSize.x(); ++i)
        {
            ++idx;
            gnormals[idx]   = derivate(Vec2ui(i, j));
            gpositions[idx] = Vec3f(i, j, cityMap.junctions().get(i, j)->height());


            ++idx;
            gnormals[idx]   = derivate(Vec2ui(i, j+1));
            gpositions[idx] = Vec3f(i, j+1, cityMap.junctions().get(i, j+1)->height());
        }

        ++idx;
        int lastx = _mapSize.x() - 1;
        gnormals[idx]   = derivate(Vec2ui(lastx, j+1));
        gpositions[idx] = Vec3f(lastx, j+1, cityMap.junctions().get(lastx, j+1)->height());
    }

    // Ground VAO setup
    glGenVertexArrays(1, &_gvao);
    glBindVertexArray(_gvao);

    const int nbAttributes = 2;
    GLuint gBuffers[nbAttributes];
    glGenBuffers(nbAttributes, gBuffers);


    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3f) * _gNbElems, gnormals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, gBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3f) * _gNbElems, gpositions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    // Arrays sweaping
    delete [] gnormals;
    delete [] gpositions;


    // Water //
    _wNbElems = 4;

    // Positions
    const float waterHeight = 0;
    Vec3f* wpositions = new Vec3f[_wNbElems];
    wpositions[0] = Vec3f(0.0,          0.0,          waterHeight);
    wpositions[1] = Vec3f(_mapSize.x(), 0.0,          waterHeight);
    wpositions[2] = Vec3f(_mapSize.x(), _mapSize.y(), waterHeight);
    wpositions[3] = Vec3f(0.0,          _mapSize.y(), waterHeight);


    // Water VAO setup
    glGenVertexArrays(1, &_wvao);
    glBindVertexArray( _wvao );

    GLuint wBuffer;
    glGenBuffers(1, &wBuffer);

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

    _hillsShader.pushThisProgram();
    _hillsShader.setVec4f("sun.direction", viewedSunDirection);

    glBindVertexArray(_gvao);
    glVertexAttrib4fv(_hillsShader.getAttributeLocation("color_att"), Vec4f(1.0, 1.0, 1.0, 1.0).asArray());
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _gNbElems);    

    glEnable(GL_BLEND);
    glBindVertexArray(_wvao);
    glVertexAttrib4fv(_hillsShader.getAttributeLocation("color_att"), Vec4f(0.03, 0.03, 0.3, 0.4).asArray());
    glVertexAttrib3fv(_hillsShader.getAttributeLocation("normal_att"), Vec3f(0.0, 0.0, 1.0).asArray());
    glDrawArrays(GL_TRIANGLE_FAN, 0, _wNbElems);
    glDisable(GL_BLEND);

    _hillsShader.popProgram();


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
