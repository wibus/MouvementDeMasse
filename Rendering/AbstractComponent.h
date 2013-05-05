#ifndef ABSTRACTCOMPONENT_H
#define ABSTRACTCOMPONENT_H

#include <GL/GlProgram.h>
#include <City/City.h>


class AbstractComponent
{
public:
    AbstractComponent(City& city, media::GlProgram& shader) :
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
    media::GlProgram& _shader;
};

#endif // ABSTRACTCOMPONENT_H
