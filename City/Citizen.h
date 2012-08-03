#ifndef CITIZEN_H
#define CITIZEN_H

#include <vector>
#include <list>

#include <DataStructure/Vector.h>
#include <DateAndTime/Calendar.h>


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
        Node(NodeType type, const cellar::Vec2i& pos) : type(type), pos(pos) {}
        NodeType type;
        cellar::Vec2i pos;
    };
    typedef std::vector< Node >     NodeContainer;
    typedef NodeContainer::iterator NodeIterator;
    typedef NodeContainer::reverse_iterator NodeRevIterator;

    Path();
    Path(const cellar::Vec2i& source, const cellar::Vec2i& destination);

    void gotoEnd();
    void gotoBegin();

    void gotoNextNode();
    bool isEndReached() const;

    void gotoRevNextNode();
    bool isRevEndReached() const;


    float           lenght;
    cellar::Vec2i   source;
    cellar::Vec2i   destination;
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
    cellar::Vec3f position;
    cellar::Vec3f direction;
    cellar::Vec2i homePos;      //Vec3i(MapX,  MapY)
    cellar::Vec2i workPos;      //Vec3i(MapX,  MapY)
    Path          homeToWorkPath;
    Schedule      schedule;

private:
    Id _id;
    static Id __assigneId();
    static Id __nextId;
};

#endif // CITIZEN_H
