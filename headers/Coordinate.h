#ifndef COORDINATE_H
#define COORDINATE_H

#include <functional>
#include <utility>


struct Coordinate {
    int x, y;

    bool operator==(const Coordinate& other) const {
        return x == other.x && y == other.y;
    }
    bool operator!=(const Coordinate& other) const{
        return !(*this == other);
    }
};

struct distance_hash {
    int GRID_SIZE = 50;
    inline std::size_t operator()(const std::pair<Coordinate, Coordinate>& v) const {
        return v.first.x * 31 * 31 + v.first.y * 31 + v.second.x * GRID_SIZE + v.second.y;
    }
};

struct single_hash{
    std::size_t operator()(Coordinate p) const {
        const std::size_t prime = 31;
        std::size_t hash1 = std::hash<int>()(p.x);
        std::size_t hash2 = std::hash<int>()(p.y);
        return hash1 * prime + hash2;
    }
};


#endif // COORDINATE_H
