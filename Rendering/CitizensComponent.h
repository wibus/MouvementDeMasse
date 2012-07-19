#ifndef CITIZENSCOMPONENT_H
#define CITIZENSCOMPONENT_H

#include <vector>

#include <MathsAndPhysics/Matrix.h>

#include "AbstractComponent.h"


class CitizensComponent : public AbstractComponent
{
public:
    CitizensComponent(City& city, cellar::GLShaderProgram& shader);
    ~CitizensComponent();

    void setup();
    void draw();
    void update();

protected:
    unsigned int _citizenVao;
    unsigned int _citizenTex;
    int _citizenNbElems;
    std::vector<cellar::Mat4f> _modelMats;
};

#endif // CITIZENSCOMPONENT_H
