#ifndef SHIP_H
#define SHIP_H

#include <vector>
#include <set>
#include <utility>
#include <unordered_map>
#include <random>    // For std::default_random_engine
#include "Leak.h"
#include "Table.h"
#include "Probability.h"

struct ShipHash {
    size_t operator()(const std::pair<int, int>& p) const {
        const size_t prime = 31;
        size_t hash1 = std::hash<int>()(p.first);
        size_t hash2 = std::hash<int>()(p.second);
        return hash1 * prime + hash2;
    }
};

class Ship {
private:
    int dimensions;
    std::vector<std::vector<int>> grid;
    Table distanceTable;
    Probability bayesian_network;
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
    bool firstRoundTest();

    std::vector<std::pair<int, int>> reset();
    std::vector<std::vector<int>>& getGrid();
    std::vector<std::pair<int, int>> getPositionOfLeaks();
    std::vector<std::pair<int, int>> getClosestReachable(const std::pair<int, int>& current, const std::vector<std::pair<int, int>>& open);
    int getDistanceFrom(const std::pair<int, int>& start, const std::pair<int, int>& goal);
    std::vector<std::pair<int, int>> bot5Correction();
    void markAsSeen(const std::pair<int, int>& start, int range);

    bool positionIsOpen(int i, int j);
    void scaleGridPosition(std::pair<int, int>& pos, float modifier);
    float getProbabilityAt(std::pair<int, int>& pos);
    void normalizeProbabilities(float modifier);
    std::vector<std::pair<int, int>> getMostProbable(std::pair<int, int> current, std::vector<std::pair<int, int>>& open);
    void probabilisticStart(const std::vector<std::pair<int, int>>& open, std::string id);
    std::vector<std::vector<float>> getProbabilities();
    void updateProbabilities(std::pair<int, int>& pos, float norm);
    std::vector<std::pair<int, int>> getHighestProbabilityList();
    std::vector<std::pair<int, int>> getShortestPath(std::pair<int, int> start, std::pair<int, int> goal);


    void scaleBayesianPair(const std::pair<int,int>& x, const std::pair<int, int>& y, float mod);
    void normalizeNetwork(const std::vector<std::pair<int, int>>& open, float factor);
    bool almostDone();
    std::vector<std::pair<int, int>> getIntelligentStep(std::pair<int, int> current);
    void removePossibility(const std::pair<int, int>& current);
    void normalizePairs(const std::vector<std::pair<int, int>>& open);
    float getPairProbabilityAt(const std::pair<int, int>& i, const std::pair<int, int>& j);
    void removeInversePossibility(const std::pair<int, int>& current);
};



#endif // SHIP_H
