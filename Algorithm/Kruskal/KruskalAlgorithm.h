#ifndef KRUSKALALGORITHM_H
#define KRUSKALALGORITHM_H

#include <set>
#include <list>
#include "Segment.h"

typedef std::multiset<Segment> Graph;
typedef std::list<std::set<int> > SetsList;

class KruskalAlgorithm
{
public:
    KruskalAlgorithm();

    void addSegment(Segment seg);
    void addSegment(int edgeA, int edgeB, double distance);

    void process();
    const Graph& getCheapestGraph();

private:
    SetsList::iterator findSet(int edge);
    SetsList::iterator createNewSet(int edge);
    void processList();
    void handleMerges(Graph::iterator itr);

    bool _processed;
    Graph _fullGraph;
    Graph _cheapestGraph;
    SetsList _linkedIslands;
};

#endif // KRUSKALALGORITHM_H
