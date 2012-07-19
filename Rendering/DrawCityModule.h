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
class WaterComponent;
class CitizensComponent;
class JunctionsComponent;
class StreetsComponent;
class BuildingsComponent;
class BridgesComponent;


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
    cellar::GLShaderProgram _minimalistShader;
    cellar::GLShaderProgram _skyShader;
    cellar::GLShaderProgram _groundShader;
    cellar::GLShaderProgram _waterShader;
    cellar::GLShaderProgram _infrastructShader;

    std::vector<AbstractComponent*> _components;
    SkyComponent*       _skyComponent;
    GroundComponent*    _groundComponent;
    WaterComponent*     _waterComponent;
    CitizensComponent*  _citizensComponent;
    JunctionsComponent* _junctionsComponent;
    StreetsComponent*   _streetsComponent;
    BuildingsComponent* _buildingsComponent;
    BridgesComponent*   _bridgesComponent;
};



#endif // DRAWCITYMODULE_H
