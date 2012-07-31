#ifndef ISLAND_H
#define ISLAND_H

#include <vector>
#include <map>
#include <DataStructure/Vector.h>

class Island
{
public:
    Island();
    ~Island();

    void setIslandId(int id);
    int getIslandId() const;

    void addJunction(cellar::Vec2i junction);
    const std::vector<cellar::Vec2i>& getJunctions() const;

    cellar::Vec2i bridgeForTravel (int islandToReachId) const;

private:
    int _islandId;
    std::vector<cellar::Vec2i> _junctions;
    std::map<int, cellar::Vec2i> _bridges;
};


// Implementation //
inline void Island::setIslandId(int id)
{
    _islandId = id;
}

inline int Island::getIslandId()
{
    return _islandId;
}

inline void Island::addJunction(cellar::Vec2i junction)
{
    _junctions.push_back(junction);
}

inline const std::vector<cellar::Vec2i>& Island::getJunctions() const
{
    return _junctions;
}

#endif // ISLANDS_H
