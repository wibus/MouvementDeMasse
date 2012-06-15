#ifndef STREETSCOMPONENT_H
#define STREETSCOMPONENT_H

struct DrawCityCommonData;


class StreetsComponent
{
public:
    StreetsComponent(DrawCityCommonData& common);

    void setup();
    void draw();
    void update();

    void updateProjectionMatrix();
    void updateModelViewMatrix();

protected:
    float junctionHeight(int x, int y);

    DrawCityCommonData& _common;
    unsigned int _streetsVao;
    unsigned int _streetsTex;
    int          _streetsNbElems;
};

#endif // STREETSCOMPONENT_H
