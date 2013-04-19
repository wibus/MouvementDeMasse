#include "CitizensComponent.h"

#include <GL3/gl3w.h>
using namespace std;

#include <Graphics/GL/GlToolkit.h>
using namespace cellar;


CitizensComponent::CitizensComponent(City &city, cellar::GlProgram &shader) :
    AbstractComponent(city, shader),
    _citizennBuffs(),
    _citizenVao(0),
    _citizenTex(0),
    _citizenNbElems(0),
    _modelMats()
{
    glGenVertexArrays(1, &_citizenVao);
    glGenBuffers(_CITIZEN_NB_BUFFS, _citizennBuffs);
}

CitizensComponent::~CitizensComponent()
{
    glDeleteVertexArrays(1, &_citizenVao);
    glDeleteBuffers(_CITIZEN_NB_BUFFS, _citizennBuffs);
    GlToolkit::deleteTextureId(_citizenTex);
}

void CitizensComponent::setup()
{
    // Vertices colors
    vector<Vec4f> colors;
    colors.push_back(Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    colors.push_back(Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
    colors.push_back(Vec4f(0.0f, 0.0f, 0.0f, 1.0f));
    colors.push_back(Vec4f(0.0f, 0.0f, 1.0f, 1.0f));
    colors.push_back(Vec4f(1.0f, 1.0f, 1.0f, 1.0f));
    colors.push_back(Vec4f(0.0f, 0.0f, 0.0f, 1.0f));

    // Vertices positions
    vector<Vec3f> positions;
    Vec2f halfBase(1.0f, 0.0f);
    float angle = -PI*2.0f/3.0f;
    positions.push_back(Vec3f(halfBase, 0.0f));
    positions.push_back(Vec3f(halfBase = rotate(halfBase, angle ), 0.0f));
    positions.push_back(Vec3f(halfBase = rotate(halfBase, angle ), 0.0f));
    positions.push_back(Vec3f(0.0f, 0.0f, 1.0f));
    positions.push_back(Vec3f(halfBase = rotate(halfBase, angle ), 0.0f));
    positions.push_back(Vec3f(halfBase = rotate(halfBase, angle ), 0.0f));
    _citizenNbElems = static_cast<int>(positions.size());

    // Scale
    for(size_t i=0; i<positions.size(); ++i)
        positions[i] *= _description.citizensHeight;


    // Setup Vao
    int position_loc = _shader.getAttributeLocation("position_att");
    int color_loc    = _shader.getAttributeLocation("color_att");

    glBindVertexArray( _citizenVao );

    glBindBuffer(GL_ARRAY_BUFFER, _citizennBuffs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions[0]) * positions.size(), positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(position_loc);
    glVertexAttribPointer(position_loc, 3, GL_FLOAT, 0, 0, 0);

    glBindBuffer(GL_ARRAY_BUFFER, _citizennBuffs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors[0]) * colors.size(), colors.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(color_loc);
    glVertexAttribPointer(color_loc, 4, GL_FLOAT, 0, 0, 0);

    // Clearage
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray( 0 );
}

void CitizensComponent::draw()
{
    _shader.pushProgram();

    glBindVertexArray(_citizenVao);

    for(size_t i=0; i<_modelMats.size(); ++i)
    {
        _shader.setMat4f("ModelMatrix", _modelMats[i]);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, _citizenNbElems);
    }

    _shader.popProgram();
}

void CitizensComponent::update()
{
    _modelMats.clear();
    _modelMats.reserve( _city.citizens().size() );

    Mat4f mat;

    for(CitizenIterator cit = _city.citizens().begin();
        cit != _city.citizens().end();
        ++cit)
    {
        Citizen& citizen = (*cit).second;

        mat.loadIdentity();
        mat *= translate(citizen.position.x(),
                         citizen.position.y(),
                         citizen.position.z());
        mat *= rotate(
            0.0f, 0.0f, 1.0f,
            atan2(citizen.direction.y(), citizen.direction.x()));

        _modelMats.push_back( mat );
    }
}
