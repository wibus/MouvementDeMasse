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

private:
    Type _type;
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

#endif // LAND_H
