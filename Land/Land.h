#ifndef LAND_H
#define LAND_H

class Land
{
public:
    enum Type {GRASS, RESIDENTIAL, COMMERCIAL, NB_TYPES};

    Land();
    virtual ~Land();

    void setType(Type type);
    Type type() const;

    void setNbStories(int stories);
    int nbStories() const;

    void setCapacity(int capacity);
    int capacity() const;

    void increasePeople();
    void decreasePeople();
    int nbOfPeople() const;

private:
    Type _type;
    int _nbStories;

    int _capacity;
    int _nbOfPeople;
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

inline void Land::setNbStories(int stories)
{
    _nbStories = stories;
}

inline int Land::nbStories() const
{
    return _nbStories;
}

inline void Land::setCapacity(int capacity)
{
    _capacity = capacity;
}

inline int Land::capacity() const
{
    return _capacity;
}

inline void Land::increasePeople()
{
    ++_nbOfPeople;
}

inline void Land::decreasePeople()
{
    --_nbOfPeople;
}

inline int Land::nbOfPeople() const
{
    return _nbOfPeople;
}


#endif // LAND_H
