#ifndef CITIZEN_H
#define CITIZEN_H

#include <vector>
#include <list>

#include <DataStructure/Vector.h>
#include <DateAndTime/Calendar.h>


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
    typedef std::vector< Node >  NodeVector;
    typedef NodeVector::iterator NodeIterator;

    Path();
    Path(const cellar::Vec2i& source, const cellar::Vec2i& destination);

    cellar::Vec2i source;
    cellar::Vec2i destination;    
    NodeVector    nodes;

    static const std::string NODE_TYPE_STRINGS[NB_NODE_TYPES];
};


class Schedule
{
public :
    struct Event
    {
        enum Type {GOTO_HOME, AT_HOME, GOTO_WORK, AT_WORK, NB_EVENTS};

        Event() : type(AT_HOME), time() {}
        Event(Type type, const cellar::Calendar::Time& time) : type(type), time(time) {}
        inline bool operator< (const Event& e) {return time < e.time;}

        Type type;
        cellar::Calendar::Time time;

        const static std::string EVENT_STRINGS[NB_EVENTS];
    };

    Schedule();

    bool addEvent(const Event& e);
    bool deleteEvent(const cellar::Calendar::Time& time);
    void clearEvents();
    Event currentEnvent(const cellar::Calendar::Time& time) const;

    void setDayShift(const cellar::Calendar::Time &workHomeTravelTime);
    void setAfternoonShift(const cellar::Calendar::Time &workHomeTravelTime);
    void setNightShift(const cellar::Calendar::Time &workHomeTravelTime);

    static cellar::Calendar::Time dayShiftBegin;
    static cellar::Calendar::Time afternoonShiftBegin;
    static cellar::Calendar::Time nightShiftBegin;

private:
    std::list<Event> _events;
    typedef std::list<Event>::iterator EventIterator;
    typedef std::list<Event>::const_iterator EventConstIterator;
};


class Citizen
{
public:
    enum State {AT_HOME, WORKING, SHOPING, MOVING, NB_STATES};
    typedef int Id;
    static const Id NO_ID = -1;

    Citizen();

    Id            id()          const {return _id;}
    State         state;
    float         walkingSpeed;
    cellar::Vec3f position;
    cellar::Vec3f direction;
    cellar::Vec2i homePos;      //Vec3i(MapX,  MapY)
    cellar::Vec2i workPos;      //Vec3i(MapX,  MapY)
    Path          homeToWorkPath;
    Schedule      schedule;


    const static std::string STATE_STRINGS[NB_STATES];

private:
    Id _id;
    static Id __assigneId();
    static Id __nextId;
};

#endif // CITIZEN_H
