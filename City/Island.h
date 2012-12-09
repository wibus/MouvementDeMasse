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

    void addBridge (int islandBridgedTo, cellar::Vec2i bridgePosition);
    std::multimap<int, cellar::Vec2i>::const_iterator bridgesForTravel (int islandToReachId) const;

private:
    int _islandId;
    std::vector<cellar::Vec2i> _junctions;
    std::multimap<int, cellar::Vec2i> _bridges;
};


// Implementation //
inline void Island::setIslandId(int id)
{
    _islandId = id;
}

inline int Island::getIslandId() const
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

inline void Island::addBridge (int islandBridgedTo, cellar::Vec2i bridgePosition)
{
    _bridges.insert(std::pair<int, cellar::Vec2i>(islandBridgedTo, bridgePosition));
}

inline std::multimap<int, cellar::Vec2i>::const_iterator Island::bridgesForTravel (int islandToReachId) const
{
    return _bridges.find(islandToReachId);
}

#endif // ISLANDS_H
