#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <set>
#include <utility>
#include <unordered_map>
#include <random>    // For std::default_random_engine
#include "Leak.h"
#include "Table.h"

class Ship {
private:
    int dimensions;
    std::vector<std::vector<int>> grid;
    Table distanceTable;
    std::vector<Leak> leaks;
    std::default_random_engine rng;

    std::vector<std::pair<int, int>> getNeighbors(int x, int y) const;
    std::vector<std::pair<int, int>> getOpenNeighbors(int x, int y);
    void normalizeGrid();
    bool is_dead_end(int x, int y) const;
    std::set<std::pair<int, int>> get_dead_ends();
    std::vector<std::pair<int, int>> get_closed_neighbors(int x, int y);
    void eliminate_dead_ends(std::set<std::pair<int, int>>& dead_ends);

public:
    explicit Ship(int size = 50);
    bool isPositionOpen(int i, int j, const std::vector<std::pair<int, int>>& open);
    void initializeGrid();
    void addLeak(const std::vector<std::pair<int, int>>& blockedPositions);
    bool canPlugLeak(const std::pair<int, int>& pos);
    bool plugLeak(const std::pair<int, int>& pos);
    bool hasLeaks();
    std::vector<Leak> getLeaks();
    bool firstRoundTest();

    std::vector<std::pair<int, int>> reset();
    std::vector<std::vector<int>>& getGrid();
    std::vector<std::pair<int, int>> getPositionOfLeaks();
    std::vector<std::pair<int, int>> getClosestReachable(const std::pair<int, int>& current, const std::vector<std::pair<int, int>>& open);
    int getDistanceFrom(const std::pair<int, int>& start, const std::pair<int, int>& goal);

    bool positionIsOpen(int i, int j);
    std::vector<std::pair<int, int>> getMostProbable(const std::pair<int, int>& current, std::vector<std::pair<int, int>>& mostProbable);
    std::vector<std::pair<int, int>> getShortestPath(std::pair<int, int> start, std::pair<int, int> goal);
    Table& getDistances();



    bool almostDone();

};



#endif // SHIP_H
