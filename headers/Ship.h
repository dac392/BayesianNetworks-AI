#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <set>
#include <utility>
#include <random>    // For std::default_random_engine
#include "Leak.h"
#include "Utility.h"

class Ship {
private:
    int dimensions;
    std::vector<std::vector<int>> grid;
    std::vector<Leak> leaks;
    std::default_random_engine rng; // Include the required header for this in Ship.cpp

    std::vector<std::pair<int, int>> getNeighbors(int x, int y) const;
    std::vector<std::pair<int, int>> getOpenNeighbors(int x, int y);
    void normalizeGrid();
    bool is_dead_end(int x, int y) const;
    std::set<std::pair<int, int>> get_dead_ends();
    std::vector<std::pair<int, int>> get_closed_neighbors(int x, int y);
    void eliminate_dead_ends(std::set<std::pair<int, int>>& dead_ends);

public:
    explicit Ship(int size = 50);
    ~Ship();
    void initializeGrid();
    void addLeak(const std::vector<std::pair<int, int>>& blockedPositions);
    std::vector<std::pair<int, int>> Ship::reset();
    std::vector<std::vector<int>> getGrid() const;
    std::vector<std::pair<int, int>> getPositionOfLeaks();
};

#endif // SHIP_H
