#ifndef ABSTRACTCOMPONENT_H
#define ABSTRACTCOMPONENT_H

struct DrawCityCommonData;


class AbstractComponent
{
public:
    AbstractComponent(DrawCityCommonData& common) : _common(common) {}

    virtual void setup() = 0;
    virtual void draw() = 0;

protected:
    DrawCityCommonData& _common;
};

#endif // ABSTRACTCOMPONENT_H
