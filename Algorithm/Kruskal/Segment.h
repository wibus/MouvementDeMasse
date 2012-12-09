#ifndef SEGMENT_H
#define SEGMENT_H

class Segment
{
public:
    Segment(int edgeA, int edgeB, double distance);

    int edgeA() const;
    int edgeB() const;
    double distance() const;

    bool operator<(const Segment& segment) const;

private:
    int _edgeA;
    int _edgeB;
    double _distance;
};

// Implementation //
inline int Segment::edgeA() const
{
    return _edgeA;
}

inline int Segment::edgeB() const
{
    return _edgeB;
}

inline double Segment::distance() const
{
    return _distance;
}

inline bool Segment::operator<(const Segment& segment) const
{
    return _distance < segment.distance();
}

#endif // SEGMENT_H
