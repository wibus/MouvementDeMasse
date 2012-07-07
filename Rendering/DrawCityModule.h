#ifndef DRAWCITYMODULE_H
#define DRAWCITYMODULE_H

#include <vector>
#include <Graphics/GL/GLShaderProgram.h>

class City;
class Ground;
class Visual;
class AbstractComponent;
class SkyComponent;
class GroundComponent;
class JunctionsComponent;
class StreetsComponent;
class BuildingsComponent;
class WaterComponent;


class DrawCityModule
{
public:
    DrawCityModule(City& city);
    ~DrawCityModule();

    void setup();
    void draw();
    void update();

    void updateProjectionMatrix(const cellar::Matrix4x4<float>& proj);
    void updateModelViewMatrix(const cellar::Matrix4x4<float>& view);

protected:
    void updateShaders();
    void updateShadersProjectionMatrix();
    void updateShadersModelViewMatrix();

    City&   _city;
    Ground& _ground;
    Visual& _visual;

    // Shaders
    cellar::GLShaderProgram _skyShader;
    cellar::GLShaderProgram _groundShader;
    cellar::GLShaderProgram _infrastructShader;
    cellar::GLShaderProgram _waterShader;

    std::vector<AbstractComponent*> _components;
    SkyComponent*       _skyComponent;
    GroundComponent*    _groundComponent;
    JunctionsComponent* _junctionsComponent;
    StreetsComponent*   _streetsComponent;
    BuildingsComponent* _buildingsComponent;
    WaterComponent*     _waterComponent;    
};



#endif // DRAWCITYMODULE_H
