#ifndef CITY_H
#define CITY_H

#include <Characters/AbstractCharacter.h>

namespace prop {class AbstractStage;}


class City : public scaena::AbstractCharacter
{
public:
    City(scaena::AbstractStage& stage);
    virtual ~City();

    virtual void enterStage();
    virtual void draw(const scaena::StageTime &time);
    virtual void beginStep(const scaena::StageTime &time);
    virtual void exitStage();

    virtual bool keyPressEvent(const scaena::KeyboardEvent &event);
    virtual bool mousePressEvent(const scaena::MouseEvent &event);
    virtual bool mouseMoveEvent(const scaena::MouseEvent &event);

private:
};

#endif // CITY_H
