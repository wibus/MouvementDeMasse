#include "CitizensComponent.h"

#include <GL/glew.h>
using namespace std;

#include <Graphics/GL/GLToolkit.h>
using namespace cellar;


CitizensComponent::CitizensComponent(City &city, cellar::GLShaderProgram &shader) :
    AbstractComponent(city, shader),
    _citizenVao(0),
    _citizenTex(0),
    _citizenNbElems(0),
    _modelMats()
{
    glGenVertexArrays(1, &_citizenVao);
}

CitizensComponent::~CitizensComponent()
{
    glDeleteVertexArrays(1, &_citizenVao);
    GLToolkit::deleteTextureId(_citizenTex);
}

void CitizensComponent::setup()
{
    vector<Vec3f> positions;
    vector<Vec4f> colors;

    // Construct
    Vec2f halfBase(1.0f, 0.0f);
    float angle = -PI*2.0f/3.0f;
    positions.push_back(vec3(halfBase, 0.0f));                  colors.push_back(Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    positions.push_back(vec3(halfBase.rotate( angle ), 0.0f));  colors.push_back(Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
    positions.push_back(vec3(halfBase.rotate( angle ), 0.0f));  colors.push_back(Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
    positions.push_back(Vec3f(0.0f, 0.0f, 1.0f));               colors.push_back(Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
    positions.push_back(vec3(halfBase.rotate( angle ), 0.0f));  colors.push_back(Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    positions.push_back(vec3(halfBase.rotate( angle ), 0.0f));  colors.push_back(Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
    _citizenNbElems = positions.size();

    // Scale
    for(size_t i=0; i<positions.size(); ++i)
        positions[i] *= _visual.citizensHeight;


    // Setup Vao
    int position_loc = _shader.getAttributeLocation("position_att");
    int color_loc    = _shader.getAttributeLocation("color_att");

    glBindVertexArray( _citizenVao );

    const int NB_BUFFERS = 2;
    GLuint buffers[NB_BUFFERS];
    glGenBuffers(NB_BUFFERS, buffers);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * _citizenNbElems, positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors[0]) * _citizenNbElems, colors.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(color_loc);
    glVertexAttribPointer(color_loc, 4, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void CitizensComponent::draw()
{
    _shader.pushThisProgram();

    glBindVertexArray(_citizenVao);

    for(size_t i=0; i<_modelMats.size(); ++i)
    {
        _shader.setMatrix4x4("ModelMatrix", _modelMats[i]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, _citizenNbElems);
    }

    _shader.popProgram();
}

void CitizensComponent::update()
{
    _modelMats.clear();
    _modelMats.reserve( _city.citizens().size() );

    Mat4f mat;

    for(size_t i=0; i<_city.citizens().size(); ++i)
    {
        Citizen& citizen = _city.citizens()[i];

        mat.loadIdentity();
        mat.translate(citizen.position.x(), citizen.position.y(), citizen.position.z() + 0.05f);
        mat.rotate(0.0f, 0.0f, 1.0f, atan2(citizen.direction.y(), citizen.direction.x()));

        _modelMats.push_back( mat );
    }
}
