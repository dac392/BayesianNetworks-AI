#ifndef TABLE_H
#define TABLE_H

#include <utility>
#include <unordered_map>
#include <vector>

#include "Coordinate.h"

#define GRID_SIZE 50

/**
 * handles distance related matters
*/
class Table
{
private:
    std::unordered_map<std::pair<Coordinate, Coordinate>, int, distance_hash> hash_map;

public:
    Table();
    void init(const std::vector<std::vector<int>>& grid);
    void bfs(const std::vector<std::vector<int>>& grid, Coordinate start);
    int getDistance(const std::pair<int, int>& start, const std::pair<int, int>& goal);
    std::vector<std::pair<int, int>> getClosestPositions(const std::pair<int, int>& start, const std::vector<std::pair<int, int>>& open);
    std::vector<std::pair<int, int>> getPositionsInRange(const std::pair<int, int>& start, int range);

};

#endif // TABLE_H
