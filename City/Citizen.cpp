#include "Citizen.h"

using namespace cellar;


const std::string Path::NODE_TYPE_STRINGS[Path::NB_NODE_TYPES] = {
    "LAND", "JUNCTION", "BRIDGE_END", "METRO", "BUS_STOP"
};

Path::Path() :
    lenght(0.0f),
    source(),
    destination(),
    nodes()
{
}

Path::Path(const cellar::Vec2i& source, const cellar::Vec2i& destination) :
    lenght(0.0f),
    source(source),
    destination(destination),
    nodes()
{
}


// Schedule
const std::string Schedule::Event::EVENT_STRINGS[NB_EVENTS]= {
    "GOTO_HOME", "AT_HOME", "GOTO_WORK", "AT_WORK"
};

Time Schedule::dayShiftBegin       = Time(8,  0, 0);
Time Schedule::afternoonShiftBegin = Time(16, 0, 0);
Time Schedule::nightShiftBegin     = Time(0,  0, 0);


Schedule::Schedule() :
    _events()
{
}


bool Schedule::addEvent(const Event& e)
{
    _events.push_back(e);
    _events.sort();
    return true;
}

bool Schedule::deleteEvent(const cellar::Time& time)
{
    EventIterator it = _events.begin();
    for(;it != _events.end(); ++it)
    {
        if(it->time == time)
        {
            _events.erase(it);
            return true;
        }
    }
    return false;
}

void Schedule::clearEvents()
{
    _events.clear();
}

Schedule::Event Schedule::currentEnvent(const cellar::Time& time) const
{
    Event e = _events.back();

    for(EventConstIterator it = _events.begin();it != _events.end(); ++it)
    {
        if(it->time < time)
            return e;
        e = *it;
    }

    return e;
}


void Schedule::setDayShift(const Time& workHomeTravelTime)
{
    clearEvents();
    addEvent(Event(Event::GOTO_WORK, dayShiftBegin - workHomeTravelTime));
    addEvent(Event(Event::AT_WORK,   dayShiftBegin));
    addEvent(Event(Event::GOTO_HOME, afternoonShiftBegin));
    addEvent(Event(Event::AT_HOME,   afternoonShiftBegin + workHomeTravelTime));
}

void Schedule::setAfternoonShift(const Time& workHomeTravelTime)
{
    clearEvents();
    addEvent(Event(Event::GOTO_WORK, afternoonShiftBegin - workHomeTravelTime));
    addEvent(Event(Event::AT_WORK,   afternoonShiftBegin));
    addEvent(Event(Event::GOTO_HOME, nightShiftBegin));
    addEvent(Event(Event::AT_HOME,   nightShiftBegin + workHomeTravelTime));
}

void Schedule::setNightShift(const Time& workHomeTravelTime)
{
    clearEvents();
    addEvent(Event(Event::GOTO_WORK, nightShiftBegin - workHomeTravelTime));
    addEvent(Event(Event::AT_WORK,   nightShiftBegin));
    addEvent(Event(Event::GOTO_HOME, dayShiftBegin));
    addEvent(Event(Event::AT_HOME,   dayShiftBegin + workHomeTravelTime));
}


// Citizens
int Citizen::__nextId = 0;
int Citizen::__assigneId() {return __nextId++;}

const std::string Citizen::STATE_STRINGS[Citizen::NB_STATES] = {
    "AT_HOME", "WORKING", "SHOPING", "MOVING"
};

Citizen::Citizen() :    
    state(Citizen::AT_HOME),
    walkingSpeed(1.0f),
    position(0.0f, 0.0f, 0.0f),
    direction(1.0f, 0.0f, 0.0f),
    homePos(0, 0),
    workPos(0, 0),
    homeToWorkPath(),
    schedule(),
    _id(__assigneId())
{
}
