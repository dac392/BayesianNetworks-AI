#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <set>
#include <utility>
#include "Leak.h"

class Ship {
private:
    int dimensions;
    std::vector<std::vector<int>> grid;
    std::vector<Leak> leaks;
    std::default_random_engine rng; // Include the required header for this in Ship.cpp

    std::vector<std::pair<int, int>> getNeighbors(int x, int y);
    std::vector<std::pair<int, int>> getOpenNeighbors(int x, int y);

public:
    explicit Ship(int size = 50);
    ~Ship();
    void initializeGrid();
    void placeLeak(int x, int y);
    bool isLeakAt(int x, int y);
    void plugLeakAt(int x, int y);
    std::vector<std::vector<int>> getGrid();
    int getDimensions();
};

#endif // SHIP_H
