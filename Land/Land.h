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

private:
    Type _type;
    int _nbStories;
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

#endif // LAND_H
