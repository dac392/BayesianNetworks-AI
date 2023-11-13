#ifndef UTILITY_H
#define UTILITY_H

#include <string>
#include <vector>
#include <utility>


class Utility {
public:
    static std::pair<int, int> generateCoordinate();
    static std::string hashFunction(const std::pair<int, int>& p);
    static int heuristic(std::pair<int, int>& a, std::pair<int, int>& b);
    static std::pair<int, int> shufflePositions(std::vector<std::pair<int, int>>& positions);
    static void removePosition(std::vector<std::pair<int, int>>& vec, const std::pair<int, int>& elem);
};

// PairHash definition
struct PairHash {
    size_t operator()(const std::pair<int, int>& p) const {
        auto hash1 = std::hash<int>{}(p.first);
        auto hash2 = std::hash<int>{}(p.second);
        return hash1 ^ (hash2 << 1);
    }
};

#endif // UTILITY_H
