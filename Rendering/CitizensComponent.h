#ifndef CITIZENSCOMPONENT_H
#define CITIZENSCOMPONENT_H

#include <vector>

#include "AbstractComponent.h"


class CitizensComponent : public AbstractComponent
{
public:
    CitizensComponent(City& city, cellar::GlProgram& shader);
    ~CitizensComponent();

    void setup();
    void draw();
    void update();

protected:
    static const int _CITIZEN_NB_BUFFS = 2;
    unsigned int _citizennBuffs[_CITIZEN_NB_BUFFS];
    unsigned int _citizenVao;
    unsigned int _citizenTex;
    int _citizenNbElems;
    std::vector<glm::mat4> _modelMats;
};

#endif // CITIZENSCOMPONENT_H
