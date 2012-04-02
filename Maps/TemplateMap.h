#ifndef TEMPLATEMAP_H
#define TEMPLATEMAP_H


template <typename T>
class TemplateMap
{
public:
    TemplateMap(unsigned int width, unsigned int height);

    void reset();

    unsigned int width() const;
    unsigned int height() const;

    T* operator()(unsigned int x, unsigned int y) const;
    T* at(unsigned int x, unsigned int y) const;
    T* set(unsigned int x, unsigned int y, T* street);

private:
    TemplateMap();
    TemplateMap(const TemplateMap& map);
    TemplateMap& operator=(const TemplateMap* map);

    unsigned int _width;
    unsigned int _height;
    T*** _elements;
};



// IMPLEMENTATION //
template <typename T>
inline unsigned int TemplateMap<T>::width() const
    {return _width;}

template <typename T>
inline unsigned int TemplateMap<T>::height() const
    {return _height;}

template <typename T>
TemplateMap<T>::TemplateMap(unsigned int width, unsigned int height):
    _elements(nullptr)
{
    _elements = new T**[_height];

    for(int j=0; j<_height; ++j)
    {
        _elements[j] = new T*[_width];

        for(int i=0; i<_width; ++i)
            _elements[j][i] = nullptr;
    }
}

template <typename T>
void TemplateMap<T>::reset()
{
    for(int j=0; j<_height; ++j)
        for(int i=0; i<_width; ++i)
        {
            delete _elements;
            _elements[j][i] = nullptr;
        }
}

template <typename T>
T* TemplateMap<T>::operator()(
        unsigned int x,
        unsigned int y) const
{
    if(x >= 0 && x < _width && y >= 0 && y < _height)
        return _elements[y][x];
    return nullptr;
}

template <typename T>
T* TemplateMap<T>::at(
        unsigned int x,
        unsigned int y) const
{
    if(x >= 0 && x < _width && y >= 0 && y < _height)
        return _elements[y][x];
    return nullptr;
}

template <typename T>
T* TemplateMap<T>::set(
        unsigned int x,
        unsigned int y,
        T* street)
{
    if(x >= 0 && x < _width && y >= 0 && y < _height)
    {
        T* old = _elements[y][x];
        _elements[y][x] = street;
        return old;
    }
    return nullptr;
}

#endif // TEMPLATEMAP_H
