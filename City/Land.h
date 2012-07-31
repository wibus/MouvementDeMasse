#ifndef LAND_H
#define LAND_H

#include <string>


class Land
{
public:
    enum Type {GRASS, RESIDENTIAL, COMMERCIAL, NB_TYPES};

    Land();
    virtual ~Land();

    void setType(Type type);
    Type type() const;

    void setIslandIdentifier(int islandIdentifier);
    int getIslandIdentifier() const;

    void setNbStories(int stories);
    int nbStories() const;
    static void setMaxNbStories(int nb);
    static int maxNbStories();

    void setCapacity(int capacity);
    int capacity() const;

    void increaseResidents();
    void decreaseResidents();
    int  nbResidents() const;

    static const std::string TYPE_STRINGS[NB_TYPES];

private:
    static int _maxNbStories;
    Type _type;
    int _islandIdentifier;
    int _nbStories;
    int _capacity;
    int _nbResidents;
};



// Implementation //
inline void Land::setType(Type type)
{
    _type = type;
}

inline Land::Type Land::type() const
{
    return _type;
}

inline void Land::setIslandIdentifier(int islandIdentifier)
{
    _islandIdentifier = islandIdentifier;
}

inline int Land::getIslandIdentifier() const
{
    return _islandIdentifier;
}

inline void Land::setNbStories(int stories)
{
    _nbStories = stories;
}

inline int Land::nbStories() const
{
    return _nbStories;
}

inline void Land::setMaxNbStories(int nb)
{
    _maxNbStories = nb;
}

inline int Land::maxNbStories()
{
    return _maxNbStories;
}

inline void Land::setCapacity(int capacity)
{
    _capacity = capacity;
}

inline int Land::capacity() const
{
    return _capacity;
}

inline void Land::increaseResidents()
{
    ++_nbResidents;
}

inline void Land::decreaseResidents()
{
    --_nbResidents;
}

inline int Land::nbResidents() const
{
    return _nbResidents;
}


#endif // LAND_H
