#include "Citizen.h"

using namespace cellar;


const std::string Path::NODE_TYPE_STRINGS[Path::NB_NODE_TYPES] = {
    "LAND", "JUNCTION", "BRIDGE_END", "METRO", "BUS_STOP"
};

int Citizen::__nextId = 0;
int Citizen::__assigneId() {return __nextId++;}

Time Schedule::dayShiftBegin       = Time(8,  0, 0);
Time Schedule::afternoonShiftBegin = Time(16, 0, 0);
Time Schedule::nightShiftBegin     = Time(0,  0, 0);


// Citizens
Citizen::Citizen() :
    curState(CITIZEN_AT_HOME),
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


// Path
Path::Path() :
    lenght(0.0f),
    source(),
    destination(),
    nodes(),
    curNode(),
    nextNode(),
    revCurNode(),
    revNextNode(),
    nodeProgession(0.0f)
{
}

Path::Path(const glm::ivec2& source, const glm::ivec2& destination) :
    lenght(0.0f),
    source(source),
    destination(destination),
    nodes(),
    curNode(),
    nextNode(),
    revCurNode(),
    revNextNode(),
    nodeProgession(0.0f)
{
}

void Path::gotoEnd()
{
    nodeProgession = 0.0f;
    curNode     = nodes.end(); --curNode;
    nextNode    = nodes.end();
    revCurNode  = nodes.rbegin();
    revNextNode = nodes.rbegin(); ++revNextNode;
}

void Path::gotoBegin()
{
    nodeProgession = 0.0f;
    curNode     = nodes.begin();
    nextNode    = nodes.begin(); ++nextNode;
    revCurNode  = nodes.rend(); --revCurNode;
    revNextNode = nodes.rend();
}

void Path::gotoNextNode()
{
    nodeProgession = 0.0f;
    ++curNode;
    ++nextNode;
    --revCurNode;
    --revNextNode;
}

bool Path::isBeginReached() const
{
    return curNode == nodes.begin() || nodes.empty();
}

bool Path::isEndReached() const
{
    return nextNode == nodes.end() || nodes.empty();
}

void Path::gotoRevNextNode()
{
    nodeProgession = 0.0f;
    --curNode;
    --nextNode;
    ++revCurNode;
    ++revNextNode;
}

bool Path::isRevBeginReached() const
{
    return revCurNode == nodes.rbegin() || nodes.empty();
}

bool Path::isRevEndReached() const
{
    return revNextNode == nodes.rend() || nodes.empty();
}


// Schedule
Schedule::Schedule() :
    events()
{
}


bool Schedule::addEvent(const Event& e)
{
    events.push_back(e);
    events.sort();
    return true;
}

bool Schedule::deleteEvent(const cellar::Time& time)
{
    EventIterator it = events.begin();
    for(;it != events.end(); ++it)
    {
        if(it->time == time)
        {
            events.erase(it);
            return true;
        }
    }
    return false;
}

void Schedule::clearEvents()
{
    events.clear();
}

Schedule::Event Schedule::currentEvent(const Time& time) const
{
    Event e = events.back();

    for(EventConstIterator it = events.begin();it != events.end(); ++it)
    {
        if(time < it->time)
            return e;
        e = *it;
    }

    return e;
}

Schedule::Event Schedule::nextEvent(const Time& time) const
{
    Event e = events.front();

    for(EventConstIterator it = events.begin();it != events.end(); ++it)
    {
        if(time < it->time)
            return *it;
    }

    return e;
}

void Schedule::setDayShift(const Time& workHomeTravelTime)
{
    clearEvents();
    addEvent(Event(CITIZEN_GOTO_WORK, dayShiftBegin - workHomeTravelTime));
    addEvent(Event(CITIZEN_AT_WORK,   dayShiftBegin));
    addEvent(Event(CITIZEN_GOTO_HOME, afternoonShiftBegin));
    addEvent(Event(CITIZEN_AT_HOME,   afternoonShiftBegin + workHomeTravelTime));
}

void Schedule::setAfternoonShift(const Time& workHomeTravelTime)
{
    clearEvents();
    addEvent(Event(CITIZEN_GOTO_WORK, afternoonShiftBegin - workHomeTravelTime));
    addEvent(Event(CITIZEN_AT_WORK,   afternoonShiftBegin));
    addEvent(Event(CITIZEN_GOTO_HOME, nightShiftBegin));
    addEvent(Event(CITIZEN_AT_HOME,   nightShiftBegin + workHomeTravelTime));
}

void Schedule::setNightShift(const Time& workHomeTravelTime)
{
    clearEvents();
    addEvent(Event(CITIZEN_GOTO_WORK, nightShiftBegin - workHomeTravelTime));
    addEvent(Event(CITIZEN_AT_WORK,   nightShiftBegin));
    addEvent(Event(CITIZEN_GOTO_HOME, dayShiftBegin));
    addEvent(Event(CITIZEN_AT_HOME,   dayShiftBegin + workHomeTravelTime));
}
