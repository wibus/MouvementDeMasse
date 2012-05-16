#include "DrawNudeHills.h"
#include "CityMap.h"
#include "Road/Junction.h"
using namespace cellar;

#include <GL/glew.h>


DrawNudeHills::DrawNudeHills() :
    _shader(),
    _sizeRatio(1.0f)
{
    _shader.loadShaders("resources/shaders/nudeHills.vert",
                        "resources/shaders/nudeHills.frag");
}

void DrawNudeHills::notify(cellar::CameraMsg &msg)
{
    _shader.pushThisProgram();

    if(msg.change == CameraMsg::PROJECTION)
    {
        Matrix4x4<float> projection = msg.camera.projectionMatrix();

        _shader.setMatrix4x4("Projection", projection);

        glMatrixMode( GL_PROJECTION );
        glLoadMatrixf( projection.transposed().asArray() );
        glMatrixMode( GL_MODELVIEW );
    }
    else
    {
        Matrix4x4<float> view = msg.camera.viewMatrix();

        _shader.setMatrix4x4("ModelView",  view);
        _shader.setMatrix3x3("NormalMatrix", view.subMat3());

        glLoadMatrixf( view.transposed().asArray() );
    }

    _shader.popProgram();
}

void DrawNudeHills::setup(CityMap& cityMap)
{
    // Premilinaries
    DrawAlgorithm::setup( cityMap );

    int position_loc = _shader.getAttributeLocation("position_att");
    int normal_loc = _shader.getAttributeLocation("normal_att");


    // Ground Algorithm //

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
        gpositions[idx] = Vec3f(0.0, (j)*_sizeRatio, cityMap.junctions().get(0, j)->height());

        for(unsigned int i=0; i < _mapSize.x(); ++i)
        {
            ++idx;
            gnormals[idx]   = derivate(Vec2ui(i, j));
            gpositions[idx] = Vec3f((i)*_sizeRatio, (j)*_sizeRatio, cityMap.junctions().get(i, j)->height());


            ++idx;
            gnormals[idx]   = derivate(Vec2ui(i, j+1));
            gpositions[idx] = Vec3f((i)*_sizeRatio, (j+1)*_sizeRatio, cityMap.junctions().get(i, j+1)->height());
        }

        ++idx;
        int lastx = _mapSize.x() - 1;
        gnormals[idx]   = derivate(Vec2ui(lastx, j+1));
        gpositions[idx] = Vec3f((lastx)*_sizeRatio, (j+1)*_sizeRatio, cityMap.junctions().get(lastx, j+1)->height());
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

/*
    // Water //
    _gNbElems = 4;

    // Colors
    Vec4f* wcolors = new Vec4f[_wNbElems];
    for(int i=0; i<4; ++i) wcolors[i](0.03, 0.03, 0.3, 0.4);

    // Normals
    Vec3f* wnormals = new Vec3f[_wNbElems];
    for(int i=0; i<4; ++i) wnormals[i](0.0, 0.0, 1.0);

    // Positions
    float waterHeight = 0;
    Vec3f* wpositions = new Vec3f[_wNbElems];
    wpositions[0] = Vec3f(0.0,                      0.0,                       waterHeight);
    wpositions[1] = Vec3f(_mapSize.x()*_widthRatio, 0.03,                      waterHeight);
    wpositions[2] = Vec3f(_mapSize.x()*_widthRatio, _mapSize.y()*_heightRatio, waterHeight);
    wpositions[3] = Vec3f(0.03,                     _mapSize.y()*_heightRatio, waterHeight);


    // Water VAO setup
    glGenVertexArrays(1, &_wvao);
    glBindVertexArray( _wvao );

    GLuint wBuffers[3];
    glGenBuffers(3, wBuffers);

    glBindBuffer(GL_ARRAY_BUFFER, wBuffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wcolors), wcolors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(color_loc);
    glVertexAttribPointer(color_loc, 4, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, wBuffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wnormals), wnormals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(normal_loc);
    glVertexAttribPointer(normal_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, wBuffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wpositions), wpositions, GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);



    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
*/
}

void DrawNudeHills::process()
{
    _shader.pushThisProgram();

    glEnable(GL_BLEND);
    glBindVertexArray(_gvao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, _gNbElems);
    glDisable(GL_BLEND);

    _shader.popProgram();
}

void DrawNudeHills::setSizeRatio(float ratio)
{
    _sizeRatio = ratio;
}

float DrawNudeHills::sizeRatio() const
{
    return _sizeRatio;
}

cellar::Vec3f DrawNudeHills::derivate(cellar::Vec2ui pos)
{
    Vec3f derivate(0, 0, _sizeRatio);
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

    return derivate;
}
