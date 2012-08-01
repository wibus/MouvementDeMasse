#ifndef LAND_H
#define LAND_H

#include <set>
#include <string>
#include "Citizen.h"


class Land
{
public:
    enum Type {GRASS, RESIDENTIAL, COMMERCIAL, NB_TYPES};

    Land();
    virtual ~Land();

    Type type() const;
    void setIslandIdentifier(int islandIdentifier);
    int getIslandIdentifier() const;
    int nbStories() const;
    int capacity() const;
    int nbResidents() const;
    bool isEmpty() const;
    bool isFull() const;

    void setType(Type type);
    void setNbStories(int nb);
    bool allocateRoom(Citizen::Id id);
    bool deallocateRoom(Citizen::Id id);

    static int  maxNbStories();
    static void setMaxNbStories(int nb);

    static const std::string TYPE_STRINGS[NB_TYPES];

private:    
    Type _type;    
    int _nbStories;
    int _capacity;
    int _islandIdentifier;
    std::set<Citizen::Id> _residents;

    static int _maxNbStories;
    static const int NB_ROOMS_BY_STORY;
};



// Implementation //
inline Land::Type Land::type() const
{
    return _type;
}

inline int Land::nbStories() const
{
    return _nbStories;
}

inline void Land::setIslandIdentifier(int islandIdentifier)
{
    _islandIdentifier = islandIdentifier;
}

inline int Land::getIslandIdentifier() const
{
    return _islandIdentifier;
}

inline int Land::capacity() const
{
    return NB_ROOMS_BY_STORY * _nbStories;
}

inline int Land::nbResidents() const
{
    return _residents.size();
}

inline bool Land::isEmpty() const
{
    return _residents.empty();
}

inline bool Land::isFull() const
{
    return (int)_residents.size() == capacity();
}

inline void Land::setType(Type type)
{
    _type = type;
}

inline void Land::setNbStories(int nb)
{
    _nbStories = nb;
}

inline bool Land::allocateRoom(Citizen::Id id)
{
    if(isFull()) return false;
    return _residents.insert(id).second;
}

inline bool Land::deallocateRoom(Citizen::Id id)
{
    return _residents.erase(id);
}


inline void Land::setMaxNbStories(int nb)
{
    _maxNbStories = nb;
}

inline int Land::maxNbStories()
{
    return _maxNbStories;
}


#endif // LAND_H
