#ifndef ABSTRACTCOMPONENT_H
#define ABSTRACTCOMPONENT_H

#include <Graphics/GL/GlProgram.h>
#include <City/City.h>


class AbstractComponent
{
public:
    AbstractComponent(City& city, cellar::GlProgram& shader) :
        _city(city),                      _ground(city.ground()),
        _description(city.description()), _shader(shader)
    {}
    virtual ~AbstractComponent() {}

    virtual void setup() = 0;
    virtual void draw() = 0;
    virtual void update() {}

protected:
    City&   _city;
    Ground& _ground;
    Description& _description;
    cellar::GlProgram& _shader;
};

#endif // ABSTRACTCOMPONENT_H
