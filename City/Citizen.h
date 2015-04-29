#ifndef CITIZEN_H
#define CITIZEN_H

#include <vector>
#include <list>

#include <GLM/glm.hpp>

#include <CellarWorkbench/DateAndTime/Calendar.h>


enum CitizenState {CITIZEN_GOTO_HOME, CITIZEN_AT_HOME,
                   CITIZEN_GOTO_WORK, CITIZEN_AT_WORK, NB_CITIZEN_STATES};

const std::string CITIZEN_STATE_STRINGS[NB_CITIZEN_STATES] = {
    "CITIZEN_GOTO_HOME", "CITIZEN_AT_HOME",
    "CITIZEN_GOTO_WORK", "CITIZEN_AT_WORK"
};


class Path
{
public:
    enum NodeType {LAND, JUNCTION, BRIDGE_END, METRO, BUS_STOP, NB_NODE_TYPES};
    struct Node
    {
        Node(NodeType type, const glm::ivec2& pos) : type(type), pos(pos) {}
        NodeType type;
        glm::ivec2 pos;
    };
    typedef std::vector< Node >     NodeContainer;
    typedef NodeContainer::iterator NodeIterator;
    typedef NodeContainer::reverse_iterator NodeRevIterator;

    Path();
    Path(const glm::ivec2& source, const glm::ivec2& destination);

    void gotoEnd();
    void gotoBegin();

    void gotoNextNode();
    bool isBeginReached() const;
    bool isEndReached() const;

    void gotoRevNextNode();
    bool isRevBeginReached() const;
    bool isRevEndReached() const;


    float           lenght;
    glm::ivec2   source;
    glm::ivec2   destination;
    NodeContainer   nodes;
    NodeIterator    curNode;
    NodeIterator    nextNode;
    NodeRevIterator revCurNode;
    NodeRevIterator    revNextNode;
    float           nodeProgession;

    static const std::string NODE_TYPE_STRINGS[NB_NODE_TYPES];
};


class Schedule
{
public :
    struct Event
    {
        Event() : state(CITIZEN_AT_HOME), time() {}
        Event(CitizenState state, const cellar::Time& time) : state(state), time(time) {}
        inline bool operator< (const Event& e) {return time < e.time;}

        CitizenState state;
        cellar::Time time;
    };

    Schedule();

    bool addEvent(const Event& e);
    bool deleteEvent(const cellar::Time& time);
    void clearEvents();
    Event currentEvent(const cellar::Time& time) const;
    Event nextEvent(const cellar::Time& time) const;

    void setDayShift(const cellar::Time &workHomeTravelTime);
    void setAfternoonShift(const cellar::Time &workHomeTravelTime);
    void setNightShift(const cellar::Time &workHomeTravelTime);

    static cellar::Time dayShiftBegin;
    static cellar::Time afternoonShiftBegin;
    static cellar::Time nightShiftBegin;

    std::list<Event> events;
    typedef std::list<Event>::iterator EventIterator;
    typedef std::list<Event>::const_iterator EventConstIterator;
};


class Citizen
{
public:
    typedef int Id;
    static const Id NO_ID = -1;

    Citizen();

    Id            id()          const {return _id;}
    CitizenState  curState;
    float         walkingSpeed;
    glm::vec3 position;
    glm::vec3 direction;
    glm::ivec2 homePos;
    glm::ivec2 workPos;
    Path          homeToWorkPath;
    Schedule      schedule;

private:
    Id _id;
    static Id __assigneId();
    static Id __nextId;
};

#endif // CITIZEN_H
